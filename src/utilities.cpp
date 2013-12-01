#include "utilities.h"

#include <KUrl>
#include <vcs/vcsrevision.h>

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
        if (revision.specialType() == KDevelop::VcsRevision::Base ||
                revision.specialType() == KDevelop::VcsRevision::Head)
            return "-rlast:1";
        else if (revision.specialType() == KDevelop::VcsRevision::Working)
            return "";
        else if (revision.specialType() == KDevelop::VcsRevision::Start)
            return "-r1";
        else
            return "";  // Don't know how to handle this situation
    } else if (revision.revisionType() == KDevelop::VcsRevision::GlobalNumber)
        return QString("-r") + revision.revisionValue().toLongLong();
    else
        return "";      // Don't know how to handle this situation
}

QString getRevisionSpacRange(const KDevelop::VcsRevision& begin,
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
    return "";
}

