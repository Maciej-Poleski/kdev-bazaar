#include "bzrannotatejob.h"

#include <functional>

#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QDir>

#include <kdevplatform/vcs/dvcs/dvcsjob.h>
#include <kdevplatform/vcs/vcsannotation.h>
#include <kdevplatform/vcs/vcsrevision.h>
#include <kdevplatform/interfaces/iplugin.h>

BzrAnnotateJob::BzrAnnotateJob(const QDir& workingDir, const QString& revisionSpec, const KUrl& localLocation, KDevelop::IPlugin* parent, KDevelop::OutputJob::OutputJobVerbosity verbosity)
    : VcsJob(parent, verbosity), _workingDir(workingDir), _revisionSpec(revisionSpec), _localLocation(localLocation), _vcsPlugin(parent), _status(KDevelop::VcsJob::JobNotStarted)
{
    setType(JobType::Annotate);
    setCapabilities(Killable);
}

bool BzrAnnotateJob::doKill()
{
    _status = KDevelop::VcsJob::JobCanceled;
    if (_job)
        return _job->kill(KJob::Quietly);
    else
        return true;
}

void BzrAnnotateJob::start()
{
    if (_status != KDevelop::VcsJob::JobNotStarted)
        return;
    KDevelop::DVcsJob* job = new KDevelop::DVcsJob(_workingDir, _vcsPlugin, KDevelop::OutputJob::Silent);
    *job << "bzr" << "annotate" << "--all" << _revisionSpec << _localLocation;
    connect(job, SIGNAL(readyForParsing(KDevelop::DVcsJob*)), this, SLOT(parseBzrAnnotateOutput(KDevelop::DVcsJob*)));
    _status = KDevelop::VcsJob::JobRunning;
    _job = job;
    job->start();
}

void BzrAnnotateJob::parseBzrAnnotateOutput(KDevelop::DVcsJob* job)
{
    _outputLines = job->output().split('\n');
    _currentLine = 0;
    if (_status == KDevelop::VcsJob::JobRunning)
        QTimer::singleShot(0, this, SLOT(parseNextLine()));
}

void BzrAnnotateJob::parseNextLine()
{
    if (_currentLine == static_cast<decltype(_currentLine)>(_outputLines.size())) {
        _status = KDevelop::VcsJob::JobSucceeded;
        emitResult();
        emit resultsReady(this);
        return;
    }
    QString currentLine = _outputLines[_currentLine];
    if (currentLine.isEmpty()) {
        ++_currentLine;
        QTimer::singleShot(0, this, SLOT(parseNextLine()));
        return;
    }
    bool revOk;
    auto revision = currentLine.left(currentLine.indexOf(' ')).toULong(&revOk);
    if (!revOk) {
        // Future compatibility - not a revision yet
        ++_currentLine;
        QTimer::singleShot(0, this, SLOT(parseNextLine()));
        return;
    }
    auto i = _commits.find(revision);
    if (i != _commits.end()) {
        KDevelop::VcsAnnotationLine line;
        line.setAuthor(i->second.author());
        line.setCommitMessage(i->second.message());
        line.setDate(i->second.date());
        line.setLineNumber(_currentLine);
        line.setRevision(i->second.revision());
        _results.append(QVariant::fromValue(line));
        ++_currentLine;
        QTimer::singleShot(0, this, SLOT(parseNextLine()));
    } else {
        prepareCommitInfo(revision);
    }
}

void BzrAnnotateJob::prepareCommitInfo(std::size_t revision)
{
    if (_status != KDevelop::VcsJob::JobRunning)
        return;
    KDevelop::DVcsJob* job = new KDevelop::DVcsJob(_workingDir, _vcsPlugin, KDevelop::OutputJob::Silent);
    job->setType(KDevelop::VcsJob::Log);
    *job << "bzr" << "log" << "--long" << "-r" << QString::number(revision);
    connect(job, SIGNAL(readyForParsing(KDevelop::DVcsJob*)), this, SLOT(parseBzrLog(KDevelop::DVcsJob*)));
    _job = job;
    job->start();
}

void BzrAnnotateJob::parseBzrLog(KDevelop::DVcsJob* job)
{
    QStringList outputLines = job->output().split('\n');
    KDevelop::VcsEvent commitInfo;
    std::size_t revision;
    bool atMessage = false;
    QString message;
    for (QString line : outputLines) {
        if (!atMessage) {
            if (line.startsWith("revno")) {
                QString revno = line.mid(QString("revno: ").length());
                revno = revno.left(revno.indexOf(' '));
                revision = revno.toULong();
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
        } else {
            message += line.trimmed() + "\n";
        }
    }
    if (atMessage)
        commitInfo.setMessage(message.trimmed());
    _commits[revision] = commitInfo;
    QTimer::singleShot(0, this, SLOT(parseNextLine()));
}

QVariant BzrAnnotateJob::fetchResults()
{
    return _results;
}

KDevelop::VcsJob::JobStatus BzrAnnotateJob::status() const
{
    return _status;
}

KDevelop::IPlugin* BzrAnnotateJob::vcsPlugin() const
{
    return _vcsPlugin;
}

#include "bzrannotatejob.moc"
