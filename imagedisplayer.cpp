#include "imagedisplayer.h"
#include <QPainter>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QWebEngineView>
//#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QWebEngineSettings>

ImageDisplayer::ImageDisplayer(QWidget *parent) : QWidget(parent) {
    // Ініціалізація, якщо потрібно
    imagesCount=0;
}





void ImageDisplayer::displayImage(const QVector<ImageSearcher::Img> &imgs, QGridLayout *myGridLayout, QFrame *myFrame) {
    QGridLayout* gridLayout = myGridLayout;
    int rowCount=0;
    int columnCount=0;
     int frameHight=100;

    if (gridLayout->count() > 0) {
        rowCount = gridLayout->rowCount();
        columnCount = gridLayout->columnCount();
        rowCount-=1;
        columnCount;
       frameHight=myFrame->height();


    }else{
       imagesCount=0;
        this->checkBoxToImageMap.clear();
    }


 qDebug()<<imgs.size();

    for(; ; rowCount++){
         frameHight+=210;


        for(; columnCount<3; columnCount++){

            if(imagesCount>=imgs.size())
                 return;
                    myFrame->setFixedHeight(frameHight);
           QWebEngineView* webView1 = new QWebEngineView();
           QString imageUrl = imgs[imagesCount].url;


           QString htmlContent = "<html><body><img src='" + imageUrl + "'></body></html>";

           webView1->setHtml(htmlContent);
           webView1->resize(100, 100);

            QVBoxLayout *verticalLayout = new QVBoxLayout();

            QLabel *label1 = new QLabel(imgs[imagesCount].imgName);
           // QLabel *label2 = new QLabel(imgs[imagesCount].imgSite);


            verticalLayout->addWidget(webView1);
            verticalLayout->addWidget(label1);
            //verticalLayout->addWidget(label2);
            QCheckBox* checkBox = new QCheckBox(this);
            verticalLayout->addWidget(checkBox);

            checkBoxToImageMap[checkBox]=imgs[imagesCount];



            gridLayout->addLayout(verticalLayout, rowCount, columnCount);


            imagesCount++;

        }
        columnCount=0;

    }
}
void ImageDisplayer::clearGrid(QGridLayout *myGridLayout, QFrame *myFrame)
{
    QGridLayout* gridLayout = myGridLayout;


    for (int i = 0; i < gridLayout->rowCount(); ++i) {
        for (int j = 0; j < gridLayout->columnCount(); ++j) {
            QLayoutItem *layoutItem = gridLayout->itemAtPosition(i, j);
            if (layoutItem) {
                // Check if the item is a layout (verticalLayout)
                QVBoxLayout *verticalLayout = qobject_cast<QVBoxLayout*>(layoutItem->layout());
                if (verticalLayout) {
                    // Iterate through the items in the verticalLayout and delete them
                    QLayoutItem *item;
                    while ((item = verticalLayout->takeAt(0))) {
                        if (QWidget *widget = item->widget()) {
                            delete widget;
                        }
                        delete item;
                    }

                    // Remove the verticalLayout from the gridLayout
                    delete verticalLayout;
                    gridLayout->removeItem(layoutItem);
                }
            }
        }
    }

    myFrame->setFixedHeight(500);

}

