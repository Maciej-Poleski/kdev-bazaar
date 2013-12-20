#include "utilities.h"

#include <QtCore/QDebug>
#include <QtCore/QDateTime>

#include <KUrl>

#include <vcs/vcsrevision.h>
#include <vcs/vcsstatusinfo.h>
#include <vcs/vcsevent.h>

QDir toQDir(const KUrl& url)
{
    return QDir(url.toLocalFile());
}

QDir workingCopy(const KUrl& path)
{
    QDir dir = toQDir(path);
    while (!dir.exists(".bzr") && dir.cdUp());

    return dir;
}

QString getRevisionSpec(const KDevelop::VcsRevision& revision)
{
    if (revision.revisionType() == KDevelop::VcsRevision::Special) {
        if (revision.specialType() == KDevelop::VcsRevision::Head)
            return "-rlast:1";
        else if (revision.specialType() == KDevelop::VcsRevision::Base)
            return "";  // Workaround strange KDevelop behaviour
        else if (revision.specialType() == KDevelop::VcsRevision::Working)
            return "";
        else if (revision.specialType() == KDevelop::VcsRevision::Start)
            return "-r1";
        else
            return "";  // Don't know how to handle this situation
    } else if (revision.revisionType() == KDevelop::VcsRevision::GlobalNumber)
        return QString("-r") + QString::number(revision.revisionValue().toLongLong());
    else
        return "";      // Don't know how to handle this situation
}

QString getRevisionSpecRange(const KDevelop::VcsRevision& end)
{
    QString revisionPart;
    if (end.revisionType() == KDevelop::VcsRevision::Special) {
        if (end.specialType() == KDevelop::VcsRevision::Head)
            revisionPart = "-r..last:1";
        else if (end.specialType() == KDevelop::VcsRevision::Base)
            revisionPart = "-r..last:1"; // Workaround strange KDevelop behaviour
        else if (end.specialType() == KDevelop::VcsRevision::Working)
            revisionPart = "";
        else if (end.specialType() == KDevelop::VcsRevision::Start)
            revisionPart = "-..r1";
        else
            revisionPart = ""; // Don't know how to handle this situation
    } else if (end.revisionType() == KDevelop::VcsRevision::GlobalNumber)
        revisionPart = QString("-r") + QString::number(end.revisionValue().toLongLong());
    else
        revisionPart = "";    // Don't know how to handle this situation
    return revisionPart;
}

QString getRevisionSpecRange(const KDevelop::VcsRevision& begin,
                             const KDevelop::VcsRevision& end)
{
    if (begin.revisionType() == KDevelop::VcsRevision::Special) {
        if (begin.specialType() == KDevelop::VcsRevision::Previous) {
            if (end.revisionType() == KDevelop::VcsRevision::Special) {
                if (end.specialType() == KDevelop::VcsRevision::Base ||
                        end.specialType() == KDevelop::VcsRevision::Head)
                    return "-rlast:2..last:1";
                else if (end.specialType() == KDevelop::VcsRevision::Working)
                    return "";
                else if (end.specialType() == KDevelop::VcsRevision::Start)
                    return "-r0..1";        // That's wrong revision range
            } else if (end.revisionType() == KDevelop::VcsRevision::GlobalNumber)
                return QString("-r") +
                       QString::number(end.revisionValue().toLongLong() - 1)
                       + ".." + QString::number(end.revisionValue().toLongLong());
            else
                return "";  // Don't know how to handle this situation
        } else if (begin.specialType() == KDevelop::VcsRevision::Base ||
                   begin.specialType() == KDevelop::VcsRevision::Head) {
            // Only one possibility: comparing working copy to last commit
            return "";
        }
    } else if (begin.revisionType() == KDevelop::VcsRevision::GlobalNumber) {
        if (end.revisionType() == KDevelop::VcsRevision::Special) {
            // Assuming working copy
            return QString("-r") + QString::number(begin.revisionValue().toLongLong());
        } else {
            return QString("-r") + QString::number(begin.revisionValue().toLongLong())
                   + ".." + QString::number(end.revisionValue().toLongLong());
        }
    }
    return "";      // Don't know how to handle this situation
}

bool isValidDirectory(const KUrl& dirPath)
{
    QDir dir = workingCopy(dirPath);

    return dir.cd(".bzr") && dir.exists("branch");
}

KDevelop::VcsStatusInfo parseVcsStatusInfoLine(const QString& line)
{
    QStringList tokens = line.split(' ', QString::SkipEmptyParts);
    KDevelop::VcsStatusInfo result;
    if (tokens.size() < 2) // Don't know how to handle this situation (it is an error)
        return result;
    result.setUrl(tokens.back());
    if (tokens[0] == "M") {
        result.setState(KDevelop::VcsStatusInfo::ItemModified);
    } else if (tokens[0] == "C") {
        result.setState(KDevelop::VcsStatusInfo::ItemHasConflicts);
    } else if (tokens[0] == "+N") {
        result.setState(KDevelop::VcsStatusInfo::ItemAdded);
    } else if (tokens[0] == "?") {
        result.setState(KDevelop::VcsStatusInfo::ItemUnknown);
    } else if (tokens[0] == "D") {
        result.setState(KDevelop::VcsStatusInfo::ItemDeleted);
    } else {
        result.setState(KDevelop::VcsStatusInfo::ItemUserState);
        qWarning() << "Unsupported status: " << tokens[0];
    }
    return result;
}

QString concatenatePath(const QDir& workingCopy, const KUrl& pathInWorkingCopy)
{
    return QFileInfo(workingCopy.absolutePath() + QDir::separator()
                     + pathInWorkingCopy.toLocalFile()).absoluteFilePath();
}

KDevelop::VcsEvent parseBzrLogPart(const QString& output)
{
    const QStringList outputLines = output.split('\n');
    KDevelop::VcsEvent commitInfo;
    bool atMessage = false;
    QString message;
    bool afterMessage = false;
    QHash<QString, KDevelop::VcsItemEvent::Actions> fileToActionsMapping;
    KDevelop::VcsItemEvent::Action currentAction;
    for (QString line : outputLines) {
        if (!atMessage) {
            if (line.startsWith("revno")) {
                QString revno = line.mid(QString("revno: ").length());
                revno = revno.left(revno.indexOf(' '));
                KDevelop::VcsRevision revision;
                revision.setRevisionValue(revno.toLongLong(), KDevelop::VcsRevision::GlobalNumber);
                commitInfo.setRevision(revision);
            } else if (line.startsWith("committer: ")) {
                QString commiter = line.mid(QString("committer: ").length());
                commitInfo.setAuthor(commiter);     // Author goes after commiter, but only if is different
            } else if (line.startsWith("author")) {
                QString author = line.mid(QString("author: ").length());
                commitInfo.setAuthor(author);       // It may override commiter (In fact commiter is not supported by VcsEvent)
            } else if (line.startsWith("timestamp")) {
                const QString formatString = "yyyy-MM-dd hh:mm:ss";
                QString timestamp = line.mid(QString("timestamp: ddd ").length(), formatString.length());
                commitInfo.setDate(QDateTime::fromString(timestamp, formatString));
            } else if (line.startsWith("message")) {
                atMessage = true;
            }
        } else if (atMessage && !afterMessage) {
            if (!line.isEmpty() && line[0].isSpace()) {
                message += line.trimmed() + "\n";
            } else if (!line.isEmpty()) {
                afterMessage = true;
                // leave atMessage = true
                currentAction = parseActionDescription(line);
            } // if line is empty - ignore and get next
        } else if (afterMessage) {
            if (!line.isEmpty() && !line[0].isSpace()) {
                currentAction = parseActionDescription(line);
            } else if (!line.isEmpty()) {
                fileToActionsMapping[line.trimmed()] |= currentAction;
            } // if line is empty - ignore and get next
        }
    }
    if (atMessage)
        commitInfo.setMessage(message.trimmed());
    for (auto i = fileToActionsMapping.begin(); i != fileToActionsMapping.end(); ++i) {
        KDevelop::VcsItemEvent itemEvent;
        itemEvent.setRepositoryLocation(i.key());
        itemEvent.setActions(i.value());
        commitInfo.addItem(itemEvent);
    }
    return commitInfo;
}

KDevelop::VcsItemEvent::Action parseActionDescription(const QString& action)
{
    if (action == "added:") {
        return KDevelop::VcsItemEvent::Added;
    } else if (action == "modified:") {
        return KDevelop::VcsItemEvent::Modified;
    } else if (action == "removed:") {
        return KDevelop::VcsItemEvent::Deleted;
    } else if (action == "kind changed:") {
        return KDevelop::VcsItemEvent::Replaced; // Best approximation
    } else if (action.startsWith("renamed")) {
        return KDevelop::VcsItemEvent::Modified; // Best approximation
    } else {
        qCritical("Unsupported action: %s at line %{line} in file %{file}."
                  " Please report this.", action.toLocal8Bit().constData());
    }
}

KUrl::List handleRecursion(const KUrl::List& listOfUrls, KDevelop::IBasicVersionControl::RecursionMode recursion)
{
    if (recursion == KDevelop::IBasicVersionControl::Recursive) {
        return listOfUrls;      // Nothing to do
    } else {
        KUrl::List result;
        for (const auto url : listOfUrls) {
            if (url.isLocalFile() && QFileInfo(url.toLocalFile()).isFile()) {
                result.push_back(url);
            }
        }
        return result;
    }
}
