#include "imagedownloader.h"
#include <QNetworkReply>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QSemaphore>
#include <QtConcurrent/QtConcurrentRun>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QFuture>
#include <QEventLoop>
#include <QProgressBar>
#include <QLayout>
#include <QLayoutItem>
#include <QVBoxLayout>
#include "circularprogressbar.h"
#include <QLabel>
#include <QWebEngineView>

ImageDownloader::ImageDownloader(QObject *parent) : QObject(parent) {
 semaphore.release(6);
}



void ImageDownloader::downloadImages(const QVector<QUrl> &imageUrls, QHBoxLayout *myHBoxLayoutt, QVector<QString> imgsNameForDownload, const QString directoryName)
{

 progressBars.clear();
    progress(imageUrls, myHBoxLayoutt, imgsNameForDownload);
    int progressBar=0;


    for (const QUrl &imageUrl : imageUrls) {
   networkManager = new QNetworkAccessManager;

        QString checkCache = imageUrl.toString();
    qDebug()<<cache.containsImage(checkCache);
   if(cache.containsImage(checkCache)){
            saveFromCache(directoryName, imageUrl);
       emit progressChanged(progressBar, 100);
            qDebug()<<"Cache";


             progressBar++;
   }else{

        QNetworkRequest request(imageUrl);
        QNetworkReply *reply = networkManager->get(request);

        connect(reply, &QNetworkReply::downloadProgress, this, [reply, progressBar, this](qint64 bytesReceived, qint64 bytesTotal) {
           emit progressChanged(progressBar, bytesReceived * 100 / bytesTotal);
        });

        connect(reply, &QNetworkReply::finished, this, [this, reply, directoryName, imageUrl](){
            handleReply(reply, directoryName, imageUrl);
        });


         progressBar++;
   }
}

}
void ImageDownloader::handleReply(QNetworkReply *reply,const QString directoryName, QUrl url){


semaphore.acquire();



QtConcurrent::run([this, reply, directoryName, url]() {

                onReplyFinished(reply, directoryName, url);
                   semaphore.release();
            });

}

void ImageDownloader::onReplyFinished(QNetworkReply *reply,const QString directoryName, QUrl url)
{

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
         QPixmap *image = new QPixmap;
            image->loadFromData(imageData);
         QString imageUrl = url.toString();
         mutex.lock();
         cache.cacheImage(imageUrl, image);
         mutex.unlock();

        QString fileName;
            QString filePath;
        if(directoryName==""){
            // Check if DefaultSave directory exists, create it if not
            QDir saveDir("DefaultSave");
            if (!saveDir.exists()) {
                if (!QDir().mkdir("DefaultSave")) {
                    qWarning() << "Failed to create DefaultSave directory.";
                    return;
                }
            }
            fileName = QString("DefaultSave/image_%1_%2.jpg").arg(QDateTime::currentMSecsSinceEpoch()).arg(quint64(QThread::currentThreadId()));
            filePath = QDir::toNativeSeparators(fileName);
        }
        else{



                fileName = (directoryName +  QString("/image_%1_%2.jpg").arg(QDateTime::currentMSecsSinceEpoch()).arg(quint64(QThread::currentThreadId())));
                filePath = QDir::toNativeSeparators(fileName);


        }

        // Save the image data to the file
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(imageData);
            file.close();
            reply->deleteLater();
            qDebug() << "Image downloaded and saved to:" << filePath;
        } else {
            qWarning() << "Failed to save image:" << file.errorString();
        }
        } else {
            qDebug() << "Error downloading image:" << reply->errorString();
        }


}

void ImageDownloader::saveFromCache(const QString directoryName, QUrl url){

        QString fileName;
        QString filePath;
        if(directoryName==""){
            // Check if DefaultSave directory exists, create it if not
            QDir saveDir("DefaultSave");
            if (!saveDir.exists()) {
            if (!QDir().mkdir("DefaultSave")) {
                qWarning() << "Failed to create DefaultSave directory.";
                return;
            }
            }
            fileName = QString("DefaultSave/image_%1_%2.jpg").arg(QDateTime::currentMSecsSinceEpoch()).arg(quint64(QThread::currentThreadId()));
            filePath = QDir::toNativeSeparators(fileName);
        }
        else{
            fileName = (directoryName +  QString("/image_%1_%2.jpg").arg(QDateTime::currentMSecsSinceEpoch()).arg(quint64(QThread::currentThreadId())));
            filePath = QDir::toNativeSeparators(fileName);
        }

        QString imageUrl = url.toString();
        QPixmap pixmap = cache.getImage(imageUrl);
        QImage image = pixmap.toImage();

        // Save the image data to the file
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
           image.save(&file, "JPG");
            file.close();
        } else {
            qWarning() << "Failed to save image:" << file.errorString();
        }
}




void ImageDownloader::progress(const QVector<QUrl> &imageUrls, QHBoxLayout *myHBoxLayoutt, QVector<QString> imgsNameForDownload)
{

        for(int i=0; i<imageUrls.size(); ++i){
        CircularProgressBar* progressBar = new CircularProgressBar;
        progressBar->setMinimum(0);
        progressBar->setMaximum(100);


       QHBoxLayout *horizontalLayout = new QHBoxLayout();
       QLabel *label1 = new QLabel(imgsNameForDownload[i]);
       horizontalLayout->addWidget(progressBar);
       horizontalLayout->addWidget(label1);
       myHBoxLayoutt->insertLayout(0, horizontalLayout);

        progressBars.push_back(progressBar);

        connect(this, &ImageDownloader::progressChanged, this, [this](int j, int value) {
            if (j >= 0 && j < progressBars.size()) {

                progressBars[j]->setValue(value);
            } else {

            }
        });
        }
}





