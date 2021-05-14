#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSmallBoard_clicked()
{
    boardSize=9;
    oLauta = new lauta;
    oLauta->createLayout(boardSize);
    oLauta->exec();
    oLauta->deleteLater();
}

void MainWindow::on_btnMediumBoard_clicked()
{
    boardSize=13;
    oLauta = new lauta;
    oLauta->createLayout(boardSize);
    oLauta->exec();
    oLauta->deleteLater();
}

void MainWindow::on_btnNormalBoard_clicked()
{
    boardSize=19;
    oLauta = new lauta;
    oLauta->createLayout(boardSize);
    oLauta->exec();
    oLauta->deleteLater();
}
