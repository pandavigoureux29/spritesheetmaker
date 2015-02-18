#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <mainframe.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    MainFrame *mainFrame = new MainFrame(this);
    this->setCentralWidget(mainFrame);
    this->adjustSize();
}

MainWindow::~MainWindow()
{

}
