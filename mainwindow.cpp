#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtNetwork>
#include "imagesearcher.h"
#include <QWidget>
#include <QLayoutItem>
#include <iostream>
#include "imagedisplayer.h"
#include <QWebEngineView>
#include "imagedownloader.h"
#include<QMap>
#include <QThread>
#include <QFrame>
#include <QFileDialog>
#include <QCompleter>
#include <QLineEdit>
#include <QStandardItemModel>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
connect(&first, &ImageSearcher::imageSearchCompleted, this, &MainWindow::onImageSearchCompleted);

    ui->scrollArea_2->hide();
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_lineEdit_editingFinished()
{
    QString changedText = ui->lineEdit->text();
    auto it = std::find(historyOfSearch.begin(), historyOfSearch.end(), changedText);

    if (it != historyOfSearch.end()) {
        // If the item exists, remove it
        historyOfSearch.erase(it);
    }

    historyOfSearch.push_front(changedText);
    first.performImageSearch(changedText);
    ui->lineEdit->clear();

}


void MainWindow::onImageSearchCompleted(const QVector<ImageSearcher::Img>& imgs, int searchCout)
{

     if(searchCout==0){

         imgsForDownload.clear();
        imgsNameForDownload.clear();
         displayer.clearGrid(ui->gridLayout1, ui->frame);


     }


     displayer.displayImage(imgs, ui->gridLayout1, ui->frame);
     this->checkBoxToImageMap = displayer.checkBoxToImageMap;

     auto it = checkBoxToImageMap.begin();


     for (auto it = displayer.checkBoxToImageMap.begin(); it != displayer.checkBoxToImageMap.end(); ++it) {
         connect(it.key(), &QCheckBox::stateChanged, this, &MainWindow::handleCheckBoxStateChanged);
     }

}


void MainWindow::handleCheckBoxStateChanged(int state) {
     QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());  // Отримати відправника сигналу
     qDebug()<<checkBoxToImageMap.size();

     auto it = checkBoxToImageMap.find(checkBox);

     if (checkBox) {

         if (state == Qt::Checked) {
             imgsForDownload.push_back(it.value().url);
             imgsNameForDownload.push_back((it.value().imgName));
         } else {
             imgsForDownload.erase(std::remove(imgsForDownload.begin(), imgsForDownload.end(),it.value().url), imgsForDownload.end());
             imgsNameForDownload.erase(std::remove(imgsNameForDownload.begin(), imgsNameForDownload.end(),it.value().imgName), imgsNameForDownload.end());

         }
     }

     for(auto& url : imgsForDownload){
         qDebug() << it.value().url;
     }
}





QString MainWindow::chooseDownloadDirectory() {
          QString defaultDir = QDir::homePath();

          QString selectedDir = QFileDialog::getExistingDirectory(this, tr("Оберіть директорію для завантаження"), defaultDir);

          if (!selectedDir.isEmpty()) {

          } else {

         qDebug() << "Вибір директорії скасовано.";
          }

          return selectedDir;
}

void MainWindow::on_pushButton_3_clicked()
{
   first.performImageSearch(historyOfSearch.first());
          for (auto it = displayer.checkBoxToImageMap.begin(); it != displayer.checkBoxToImageMap.end(); ++it) {
         disconnect(it.key(), &QCheckBox::stateChanged, this, &MainWindow::handleCheckBoxStateChanged);
          }
}


void MainWindow::on_actionSave_triggered()
{
          ui->scrollArea_2->show();
          imageDownloader.downloadImages(imgsForDownload, ui->horizontalLayout, imgsNameForDownload,"");
}


void MainWindow::on_actionSave_as_triggered()
{
          ui->scrollArea_2->show();
          QString imageDirectory = chooseDownloadDirectory();
          imageDownloader.downloadImages(imgsForDownload, ui->horizontalLayout, imgsNameForDownload, imageDirectory);

}


void MainWindow::on_actionHistory_triggered()
{
     history = new History(historyOfSearch, this);
          history->show();
}

