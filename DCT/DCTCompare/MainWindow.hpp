#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include <QImage>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_spbQuality_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QImage m_Original;
    QImage m_Decoded;
    QLabel *m_lbPSNR;

    void ProcessDCT(int q);
};

#endif // MAINWINDOW_HPP
