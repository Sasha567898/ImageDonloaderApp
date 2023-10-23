#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include "QString"
#include "QPixmap"
#include "QCache"
#include <QUrl>
class ImageCache
{
public:
    ImageCache();
    void cacheImage(QString& url,  QPixmap *image);
    QPixmap getImage(const QString& url);
    bool containsImage(const QString& url);
private:
    QCache<QString, QPixmap> imageCache;
};

#endif
