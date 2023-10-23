#ifndef IMAGEDISPLAYER_H
#define IMAGEDISPLAYER_H

#include <QWidget>
#include <QVector>
#include <QImage>
#include <QNetworkReply>
#include <QWebEngineView>
#include <QLayoutItem>
#include <QWebEngineView>
#include "imagesearcher.h"
#include <QLayout>
#include <QMap>
#include <QCheckBox>
#include <QFrame>
class ImageDisplayer : public QWidget {
    Q_OBJECT

public:
    explicit ImageDisplayer(QWidget *parent = nullptr);
    void displayImage(const QVector<ImageSearcher::Img> &imgs, QGridLayout *myGridLayout, QFrame *myFrame);
    QMap<QCheckBox*, ImageSearcher::Img> checkBoxToImageMap;
    void clearGrid(QGridLayout *myGridLayout, QFrame *myFrame);


protected:


private:

    int imagesCount;

};

#endif
