#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H
#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCheckBox>
#include "imagesearcher.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QMutex>


class ImageDownloader : public QObject
{
    Q_OBJECT

public:
    explicit ImageDownloader(QObject *parent = nullptr);

public slots:
   void downloadImages(const QVector<QUrl> &imageUrls, QHBoxLayout *myHBoxLayout, QVector<QString> imgsNameForDownload, const QString directoryName);

signals:
     void progressChanged(int progressBarIndex,int value);


private slots:
    void handleReply(QNetworkReply *reply, const QString directoryName, QUrl url);
    void onReplyFinished(QNetworkReply *reply, const QString directoryName, QUrl url);
    void progress(const QVector<QUrl> &imageUrls, QHBoxLayout *myHBoxLayoutt, QVector<QString> imgsNameForDownload);
    void saveFromCache(const QString directoryName, QUrl url);

private:
     static void downloadImageWrapper(ImageDownloader *instance, const QUrl &imageUrl);
     QNetworkAccessManager* networkManager;
     QVector<QProgressBar*> progressBars;
     QSemaphore semaphore;
     QMutex mutex;
      ImageCache cache;

};


#endif // IMAGEDOWNLOADER_H




