#ifndef LAUTA_H
#define LAUTA_H

#include <QDialog>
#include <QLayoutItem>
#include <QPushButton>
#include <QVector>
#include <QDebug>
#include <QPixmap>
#include <QKeyEvent>
#include <QGridLayout>

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
    void passTurn();
    bool omatKivet();
    void paintEvent(QPaintEvent *);

protected:
void keyPressEvent(QKeyEvent *e);

private slots:
    void on_btnQuit_clicked();

    void on_btnPass_clicked();

private:
    Ui::lauta *ui;
    QVector<QPushButton *> paikat;
    QString pelaaja="musta";        // voisi vaihtaa pelaajan boolean, 0 valkea, 1 musta
    QString vastustaja="valkea";    // jolloin etsittäessä vastustajan kiviä haetaan vain (!pelaaja)
    QString pelattu;
    int boardSize;
    QVector<QString> kasa;
    QVector<QString> omaKasa;
    int viereisetKivet[4];
    QPixmap tausta;
};

#endif // LAUTA_H
