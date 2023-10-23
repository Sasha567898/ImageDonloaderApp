#ifndef HISTORY_H
#define HISTORY_H

#include <QDialog>

namespace Ui {
class History;
}

class History : public QDialog
{
    Q_OBJECT

public:
    explicit History(const QVector<QString>& history, QWidget* parent = nullptr);
    ~History();

private:
    Ui::History *ui;
};

#endif
