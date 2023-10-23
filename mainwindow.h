#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagesearcher.h"
#include "imagedisplayer.h"
#include <QVector>
#include <QUrl>
#include "imagedownloader.h"
#include "history.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

 void imageDownloaderThread(QNetworkAccessManager *networkManager);
   void setupThread(QNetworkAccessManager* networkManager, QThread* thread);

private slots:
     QString chooseDownloadDirectory();
    void on_lineEdit_editingFinished();
    void onImageSearchCompleted(const QVector<ImageSearcher::Img>& imgs, int searchCout);
    void handleCheckBoxStateChanged(int state);
    void on_pushButton_3_clicked();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionHistory_triggered();

public slots:

private:
    History *history;
    ImageDisplayer displayer;
    ImageDownloader imageDownloader;
    Ui::MainWindow *ui;
    QMap<QCheckBox*, ImageSearcher::Img> checkBoxToImageMap;
    QVector<QUrl> imgsForDownload;
    QVector<QString> imgsNameForDownload;
    QVector<QString> historyOfSearch;
    ImageSearcher first;



signals:
    void startImageDownloading();

};
#endif
