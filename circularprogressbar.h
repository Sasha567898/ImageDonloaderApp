#ifndef CIRCULARPROGRESSBAR_H
#define CIRCULARPROGRESSBAR_H


#include <QProgressBar>

class CircularProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit CircularProgressBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};
#endif
