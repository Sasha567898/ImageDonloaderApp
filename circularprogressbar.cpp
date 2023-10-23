#include "circularprogressbar.h"
#include <QPainter>

CircularProgressBar::CircularProgressBar(QWidget *parent)
    : QProgressBar(parent)
{
    setTextVisible(false);
    setMinimum(0);
    setMaximum(100);
    setFixedSize(40, 40);  // Розмір кільця
}

void CircularProgressBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    int margin = 10;
    int radius = (side - margin) / 2;

    // Визначаємо прямокутник для кола
    QRectF rectangle(width() / 2 - radius, height() / 2 - radius, radius * 2, radius * 2);

    int startAngle = 90 * 16;  // 90 градусів в 16-ричній системі
    int spanAngle = -(360 * 16) * value() / maximum();  // Кут прогресу
    QPen pen;
    pen.setColor(Qt::green);  // Колір ліній
    pen.setWidth(10);
    // Зображуємо кільце
     painter.setPen(pen);
    painter.drawArc(rectangle, startAngle, spanAngle);
}


