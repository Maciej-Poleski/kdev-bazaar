#ifndef UTILITIES_H
#define UTILITIES_H

#include <QtCore/QDir>

namespace KDevelop
{
class VcsRevision;
class VcsStatusInfo;
class VcsEvent;
}

class KUrl;

QDir toQDir(const KUrl& url);

QDir workingCopy(const KUrl& path);

/**
 * Translate VcsRevision into Revision Identifier accepted by Bazaar.
 */
QString getRevisionSpec(const KDevelop::VcsRevision& revision);

QString getRevisionSpecRange(const KDevelop::VcsRevision& end);

QString getRevisionSpecRange(const KDevelop::VcsRevision& begin,
                             const KDevelop::VcsRevision& end);

bool isValidDirectory(const KUrl& dirPath);

KDevelop::VcsStatusInfo parseVcsStatusInfoLine(const QString& line);

QString concatenatePath(const QDir& workingCopy, const KUrl& pathInWorkingCopy);

KDevelop::VcsEvent parseBzrLogPart(const QString& output);

#endif // UTILITIES_H
