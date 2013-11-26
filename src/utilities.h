#ifndef UTILITIES_H
#define UTILITIES_H


#include <QtCore/QDir>

class KUrl;

QDir toQDir(const KUrl& url);

QDir workingCopy(const KUrl& path);

#endif // UTILITIES_H
