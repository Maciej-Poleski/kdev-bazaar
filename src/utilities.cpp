#include "utilities.h"

#include <KUrl>

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