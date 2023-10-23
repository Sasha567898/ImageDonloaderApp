#include "history.h"
#include "ui_history.h"
#include <QPushButton>
#include <QVector>
#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>


History::History(const QVector<QString> &history, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::History)
{
    ui->setupUi(this);

     setWindowTitle("History");
    QListWidget* listWidget = new QListWidget(this);
    for (const QString& item : history) {
        QListWidgetItem* listItem = new QListWidgetItem(item);
        listWidget->addItem(listItem);
    }

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(listWidget);

}

History::~History()
{
    delete ui;
}
