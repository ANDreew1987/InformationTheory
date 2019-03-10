#include "QCanvas.hpp"

#include <QPainter>

QCanvas::QCanvas(QWidget *parent) :
    QWidget(parent),
    m_Image(nullptr)
{

}

void QCanvas::setImage(QImage *image)
{
    m_Image = image;
    setMinimumSize(image->size());
}

void QCanvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (m_Image)
        painter.drawImage(0, 0, *m_Image);
}
