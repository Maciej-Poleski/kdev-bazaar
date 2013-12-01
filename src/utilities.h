#ifndef UTILITIES_H
#define UTILITIES_H

#include <QtCore/QDir>

namespace KDevelop
{
class VcsRevision;
}

class KUrl;

QDir toQDir(const KUrl& url);

QDir workingCopy(const KUrl& path);

/**
 * Translate VcsRevision into Revision Identifier accepted by Bazaar.
 */
QString getRevisionSpec(const KDevelop::VcsRevision& revision);


QString getRevisionSpacRange(const KDevelop::VcsRevision& begin,
                             const KDevelop::VcsRevision& end);

#endif // UTILITIES_H
