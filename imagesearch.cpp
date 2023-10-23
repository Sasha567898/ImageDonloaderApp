#include "imagesearcher.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QRegularExpression>
#include <QLabel>
#include <QVector>
#include <QString>
#include <QXmlStreamReader>
#include "imagecache.h"
ImageSearcher::ImageSearcher(QObject *parent) : QObject(parent) {
    networkManager = new QNetworkAccessManager(this);
    query="";
    searchCoutn=0;

}


void ImageSearcher::performImageSearch(const QString &query) {

    if(this->query==query){searchCoutn++;}
    else
    {
    this->query = query;
        searchCoutn=0;
    imgs.clear();
    }

    QString baseUrl = "https://www.google.com/search?tbm=isch&q=" + query;
    int resultsPerPage = 20; // Кількість результатів на сторінку
    int numPages = 5;
     int start = searchCoutn*resultsPerPage;


    QString pageUrl = baseUrl + "&start=" + QString::number(start);
    QNetworkRequest pageRequest(pageUrl);
    QNetworkReply *pageReply = networkManager->get(pageRequest);

    connect(pageReply, &QNetworkReply::finished, this, [this, pageReply]() {
        handleGoogleImageSearchReply(pageReply);
    });

}

void ImageSearcher::handleGoogleImageSearchReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error: " << reply->errorString();
        reply->deleteLater();
        return;
    }
    QByteArray responseData = reply->readAll();
    QString responseString(responseData);

    QRegularExpression pattern("<span class=\"fYyStc\">([^<]+)</span>");
    QRegularExpressionMatchIterator matchIter = pattern.globalMatch(responseString);
    QRegularExpression url("<img [^>]*src=['\"]([^'\"]+)[^>]*>");
    QRegularExpressionMatchIterator matchIterUrl = url.globalMatch(responseString);

    matchIterUrl.next();


    while (matchIter.hasNext() || matchIterUrl.hasNext()) {
        QRegularExpressionMatch match = matchIter.next();
        QString imageName = match.captured(1);
        match = matchIter.next();
        QString imageSite = match.captured(1);


         QRegularExpressionMatch matchUrl = matchIterUrl.next();
         QString imageUrl = matchUrl.captured(1);


         Img img;
         img.imgName = imageName;
         img.imgSite = imageSite;
         img.url = imageUrl;
         this->imgs.push_back(img);

    }



      emit imageSearchCompleted(imgs, searchCoutn);


    // Close the reply and clean up
    reply->deleteLater();
}


ImageSearcher::Img& ImageSearcher::Img::operator=(const Img& other) {
    if (this == &other)
         return *this;

    // Копіюємо поля з іншого об'єкту
    this->url = other.url;
    this->imgName = other.imgName;
    this->imgSite = other.imgSite;

    return *this;
}
QVector<ImageSearcher::Img>ImageSearcher:: getImgs(){
    return imgs;
}
