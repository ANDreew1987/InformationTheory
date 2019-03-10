#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QProgressBar>

#include "Matrix.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_lbPSNR(new QLabel(this))
{
    ui->setupUi(this);
    ui->statusBar->addWidget(m_lbPSNR);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open Image"),
            "",
            tr("Image Files (*.png *.jpg *.bmp)"));

        if(fileName != ""){
            QImage tmpImage(fileName);
            m_Original = tmpImage.convertToFormat(QImage::Format_Grayscale8);
            ui->cvOrigin->setImage(&m_Original);
            ui->cvOrigin->repaint();
            ProcessDCT(ui->spbQuality->value());
        }
}

void MainWindow::on_spbQuality_valueChanged(int arg1)
{
    ProcessDCT(arg1);
    //double psnr = PSNR(&m_Original, &m_Decoded);
    //m_lbPSNR->setText("PSNR = " + QString::number(psnr) + " dB");
}

void MainWindow::ProcessDCT(int q)
{
    int cw = m_Original.width() / 8;
    int ch = m_Original.height() / 8;
    int w = cw * 8;
    int h = ch * 8;
    m_Decoded = QImage(QSize(w, h), QImage::Format_Grayscale8);

    uchar *dataSrc = m_Original.bits();
    uchar *dataDst = m_Decoded.bits();

    Matrix8x8ui8 qMatrix = BuildQuantizationMatrix(q);

    int strideSrc = m_Original.width();
    int strideDst = m_Decoded.width();

    for(int y = 0; y < ch; y++){
        for(int x = 0; x < cw; x++){
            Matrix8x8ui8 src;
            uchar *blockSrc = &dataSrc[y * 8 * strideSrc + x * 8];
            for(int y1 = 0; y1 < 8; y1++){
                for(int x1 = 0; x1 < 8; x1++){
                    src[x1][y1] = blockSrc[y1 * strideSrc + x1];
                }
            }
            Matrix8x8i8 encodedMat = Encode(src, qMatrix);
            Matrix8x8ui8 dst = Decode(encodedMat, qMatrix);
            uchar *blockDst = &dataDst[y * 8 * strideDst + x * 8];
            for(int y1 = 0; y1 < 8; y1++){
                for(int x1 = 0; x1 < 8; x1++){
                    blockDst[y1 * strideDst + x1] = dst[x1][y1];
                }
            }
        }
    }

    ui->cvDecoded->setImage(&m_Decoded);
    ui->cvDecoded->repaint();
}
