#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDebug>
#include <QGridLayout>

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

    //void map();
    void onClicked();
    void on_btnSmallBoard_clicked();

    void on_btnMediumBoard_clicked();

    void on_btnNormalBoard_clicked();

signals:
    void digitClicked(int);
private:
    Ui::MainWindow *ui;
    QVector<QPushButton *> paikat;
    QVector<QPushButton *> buttons;
    QString pelaaja="musta";
    QString vastustaja="valkea";
    short i;
    QString text;
    int boardSize=9;
    bool lautaLuotu=false;
    QPushButton *button0;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QString pelattu;
    lauta * oLauta;
};
#endif // MAINWINDOW_H
