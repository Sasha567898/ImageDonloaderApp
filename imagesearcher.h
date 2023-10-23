#ifndef IMAGESEARCHER_H
#define IMAGESEARCHER_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QVector>
#include <QString>
#include <QSemaphore>
#include "imagecache.h"



class ImageSearcher : public QObject {
    Q_OBJECT
public:
    explicit ImageSearcher(QObject *parent = nullptr);
    struct Img {
        QString url;
        QString imgName;
        QString imgSite;
          Img& operator=(const Img& other);
    };




signals:
    void imageSearchCompleted(const QVector<Img>& imgs, int searchCount);

public slots:
    void performImageSearch(const QString &query);
    void handleGoogleImageSearchReply(QNetworkReply *reply);
    QVector<Img> getImgs();


private:
    QNetworkAccessManager *networkManager;
    QVector<Img> imgs;
    QString query;
    int searchCoutn;

};

#endif
