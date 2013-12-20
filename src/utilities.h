#ifndef UTILITIES_H
#define UTILITIES_H

#include <QtCore/QDir>

#include <KUrl>

#include <kdevplatform/vcs/vcsevent.h>
#include <kdevplatform/vcs/interfaces/ibasicversioncontrol.h>

namespace KDevelop
{
class VcsRevision;
class VcsStatusInfo;
class VcsEvent;
}

class KUrl;

QDir toQDir(const KUrl& url);

/**
 * @return working copy location of working copy which contains @p path. It
 * may be any location inside working copy.
 */
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

KDevelop::VcsItemEvent::Action parseActionDescription(const QString& action);

/**
 * Some methods in interface provides @p recursion parameter. In general
 * Bazaar don't support this (only part of interface has native recursion
 * handling support). This function removes directiories from list if
 * we are in NonRecursive mode (as directory for self is not versioned).
 */
KUrl::List handleRecursion(const KUrl::List& listOfUrls, KDevelop::IBasicVersionControl::RecursionMode recursion);

#endif // UTILITIES_H
