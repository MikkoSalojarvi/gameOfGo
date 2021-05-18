#ifndef LAUTA_H
#define LAUTA_H

#include <QDialog>
#include <QLayoutItem>
#include <QPushButton>
#include <QVector>
#include <QDebug>
#include <QPixmap>
#include <QGenericMatrix>

namespace Ui {
class lauta;
}

class lauta : public QDialog
{
    Q_OBJECT

public:
    explicit lauta(QWidget *parent = nullptr);
    ~lauta();
    void createLayout(int);
    void kivenAsetus();
    bool naapurit(QPushButton*);
    void naapurit2(QPushButton*);
    bool kaappaus(QPushButton*);
    void vaihdaPelaaja(QPushButton*);
    bool kasaKaappaus();

protected:

private slots:
    void on_btnQuit_clicked();

    void on_btnPass_clicked();

private:
    Ui::lauta *ui;
    QVector<QPushButton *> paikat;
    //QVector<QPushButton *> buttons;
    QString pelaaja="musta";
    QString vastustaja="valkea";
    QPushButton *button0;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QString pelattu;
    int boardSize;
    QVector<QString> kasa;
    int viereisetKivet[4];
    int counter=0;
};

#endif // LAUTA_H
