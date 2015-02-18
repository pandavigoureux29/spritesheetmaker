#include "mainframe.h"
#include <string>

MainFrame::MainFrame(QWidget *parent) : QWidget(parent)
{
    setFixedSize(600,400);
    m_maxPerRow = 3;
    m_cellW = 64; m_cellH = 64;
    m_images = std::vector<QImage>();

    m_imageLabel = new QLabel();

    m_layout1 = new QHBoxLayout();
    m_imagesGridLayout = new QGridLayout();
    m_imagesGridLayout->setSpacing(1);

    m_loadButton = new QPushButton("AddFiles");
    QObject::connect(m_loadButton, SIGNAL(clicked()), this, SLOT(openFileDialog())) ;

    m_saveButton = new QPushButton("Save");
    QObject:connect(m_saveButton,SIGNAL(clicked()),this, SLOT(saveImage()));

    m_clearButton = new QPushButton("Clear");
    QObject::connect(m_clearButton,SIGNAL(clicked()),this,SLOT(clearAll()));

    //Layout
    QVBoxLayout * leftPanel = new QVBoxLayout();
    leftPanel->addWidget(m_loadButton);
    leftPanel->addWidget(m_saveButton);
    leftPanel->addWidget(m_clearButton);
    leftPanel->addWidget(m_imageLabel);

    m_layout1->addLayout(leftPanel);
    m_layout1->addLayout(m_imagesGridLayout);

    this->setLayout(m_layout1);
}

void MainFrame::openFileDialog(){
   QStringList files = QFileDialog::getOpenFileNames(this,
                        tr("Open Image"),
                        "./",
                        tr("Image Files (*.png *.jpg *.bmp)"));
   if(!files.isEmpty()){
       //delete former pointer
       //delete[] m_images;
       for(int i=0; i < files.length(); i++){
           //build image from its path
           QImage img = QImage( files.at(i));
           //if null, alert the user
           if (img.isNull()) {
                QMessageBox::information(this, tr("Image Viewer"),
                                         tr("Cannot load %1.").arg(files.at(i)));
               return;
           //else push the image in the grid
           }else{
               m_images.push_back( img );
           }
       }
       buildGrid();
   }
}

void MainFrame::buildGrid(){
    clearGrid();
    m_gridLabels = std::vector< std::vector<QLabel*> >();
    int totalSize = m_images.size();
    m_maxPerRow =(int) round( sqrt(totalSize) );

    //creates a new array to store the images
    int row = 0;
    int col = 0;

    m_gridLabels.push_back( std::vector<QLabel*>());

    for(int i=0; i < m_images.size();i++){
        QImage * img = &m_images.at(i);
        //create new label, set the image and push in the grid
        QLabel * tmpLabel = new QLabel();
        tmpLabel->setFixedSize(img->width(),img->height());
        tmpLabel->setPixmap(QPixmap::fromImage(*img));

        m_gridLabels.at(row).push_back(tmpLabel);
        m_imagesGridLayout->addWidget(tmpLabel,row,col);

        if( col >= m_maxPerRow-1){
            row++;
            col = 0;
            m_gridLabels.push_back( std::vector<QLabel*>());
        }else{
            col ++;
        }
    }
}

void MainFrame::clearGrid(){
    //clear grid
    for(int i=0; i < m_gridLabels.size();i++){
        std::vector<QLabel*> v = m_gridLabels.at(i);
        for(int j=0; j < v.size(); j++){
            m_imagesGridLayout->removeWidget( v[j] );
            delete v[j];
        }
    }
}

void  MainFrame::clearAll(){
    //clear images
    m_images.erase(m_images.begin(),m_images.end());
    m_images = std::vector<QImage>();

    clearGrid();
}

void MainFrame::saveImage(){
    QString path =  QFileDialog::getSaveFileName(this, "Save as", "image", "PNG(*.png);; TIFF(*.tiff *.tif);; JPEG(*.jpg *.jpeg)");
    if(!path.isNull()){
        //build the spritesheet
        QImage  img = this->buildSpriteSheet();
        if( ! img.isNull()){
            img.save(path);
            m_imageLabel->setPixmap(QPixmap::fromImage(img));
        }
    }
}

QImage MainFrame::buildSpriteSheet(){
    if(m_images.size()==0){
        return QImage();
    }
    int totalWidth = m_cellW*m_maxPerRow;
    if( m_images.size() < m_maxPerRow)
        totalWidth = m_images.size() * m_cellW;
    int totalHeight = ( m_images.size()/m_maxPerRow + 1) * m_cellH;
    qDebug() << "Image : "<< totalWidth << "x" << totalHeight;
    if( totalHeight <= 0 || totalWidth <= 0)
        return QImage();
    int x,y;
    QImage img = QImage(totalWidth,totalHeight,QImage::Format_ARGB32);
    int row = 0; int col = 0;
    //add images to img
    for(int m=0; m < m_images.size(); m++){
        QImage *tmpImg = &( m_images.at(m) );
        x = col*m_cellH;
        y = row*m_cellW;

        for(int i=0; i < tmpImg->width();i++){
            for(int j=0; j< tmpImg->height();j++){
                QColor pxl( tmpImg->pixel(i,j) );
                img.setPixel(x+i,y+j,pxl.rgb());
                //qDebug() << i << " " << j << "" << pxl.red();
            }
        }
        //increment grid
        if( col >= m_maxPerRow-1){
            row++;
            col = 0;
        }else{
            col ++;
        }

    }
    //m_imageLabel->setPixmap(QPixmap::fromImage(img));
    return img;
}
