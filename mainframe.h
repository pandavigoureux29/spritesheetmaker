#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QDebug>
#include <QImage>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QInputDialog>

#include <math.h>

class MainFrame : public QWidget{

    Q_OBJECT

    public : MainFrame(QWidget *parent = 0);

    private:
        QHBoxLayout * m_layout1;
        QGridLayout * m_imagesGridLayout;

        //buttons
        QPushButton * m_loadButton;
        QPushButton * m_clearButton;
        QPushButton * m_saveButton;

        QLabel * m_imageLabel;
        std::vector<QImage> m_images;
        std::vector< std::vector<QLabel*> > m_gridLabels;

        int m_maxPerRow;
        int m_cellW; int m_cellH;

        void rearrangeImages();
        QImage buildSpriteSheet();

    public slots:
        void openFileDialog();
        void buildGrid();
        void clearGrid();
        void saveImage();
        void clearAll();
};

#endif // MAINFRAME_H

