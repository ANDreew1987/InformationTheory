#ifndef QCANVAS_HPP
#define QCANVAS_HPP

#include <QWidget>
#include <QImage>

class QCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit QCanvas(QWidget *parent = nullptr);

    void setImage(QImage *image);

signals:

public slots:

protected:
    QImage *m_Image;

    void paintEvent(QPaintEvent *) override;
};

#endif // QCANVAS_HPP
