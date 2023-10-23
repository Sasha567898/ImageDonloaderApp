#include "imagecache.h"
#include <QUrl>

ImageCache::ImageCache()
{

}

void ImageCache::cacheImage(QString& url,  QPixmap* image){
    imageCache.insert(url, image);
}
QPixmap ImageCache::getImage(const QString& url) {
        QPixmap *cachedImage = imageCache.object(url);
    if (cachedImage) {
        return *cachedImage;
    } else {
        return QPixmap();
    }

}
bool ImageCache::containsImage(const QString& url) {
    return imageCache.contains(url);

}
