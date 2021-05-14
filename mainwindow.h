#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDebug>

#include "lauta.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:


    void on_btnSmallBoard_clicked();

    void on_btnMediumBoard_clicked();

    void on_btnNormalBoard_clicked();

signals:
    void digitClicked(int);
private:
    Ui::MainWindow *ui;
    int boardSize;
    lauta * oLauta;
};
#endif // MAINWINDOW_H
