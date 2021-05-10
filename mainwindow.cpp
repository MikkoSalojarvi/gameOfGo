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

void MainWindow::onClicked()
{
    this->close();
}


void MainWindow::kivenAsetus()
{
    QObject * sender = QObject::sender();
    QString paikka = sender->objectName();
    foreach(QPushButton * ptr, paikat)
    {
        if(ptr ->objectName() == paikka)
            {
            pelattu=ptr->objectName();
            naapurit(ptr);// hae naapurit findchild, nimet +1,-1,+9,-9
                if (ptr->text()!="musta"&&ptr->text()!="valkea")
                {
                    ptr->setText(pelaaja);
                    vaihdaPelaaja();
                }
            }
    }
}

void MainWindow::vaihdaPelaaja()
{
    if (pelaaja == "musta")
    {
       pelaaja= "valkea";
       vastustaja="musta";
    }
    else {
        pelaaja= "musta";
        vastustaja="valkea";
    }
}

void MainWindow::createLayout()
{
    int nimet=1;
        for(int i=1; i <= boardSize; i++){
            for(int j=1; j <= boardSize; j++){
                QString teksti=QString::number(i);
                teksti.append(" "+QString::number(j));
                ui->ristikko->addWidget(new QPushButton(teksti, this),i,j,Qt::Alignment(Qt::AlignLeft));
                QLayoutItem* item = ui->ristikko->itemAtPosition(i, j);
                QPushButton* btn = qobject_cast<QPushButton *>(item->widget());
                btn->QPushButton::setSizePolicy(QSizePolicy ::Expanding, QSizePolicy ::Expanding );
                btn->setObjectName(QString::number(nimet));
                btn->setMinimumWidth(9);
                connect(btn, &QPushButton::clicked, this, &MainWindow::kivenAsetus);
                ui->ristikko->setColumnMinimumWidth(j,10);
                paikat.append(btn);
                nimet++;
            }
        ui->ristikko->setRowMinimumHeight(i,10);
        }
    lautaLuotu=true;
}

void MainWindow::naapurit(QPushButton * kivi)   // hae naapurit findchild, nimet +1,-1,+9,-9
{
    QString nimi=kivi->objectName();
    bool olemassa[4]={false,false,false,false};
    if (nimi.toInt()%boardSize!=1){              // jos kivi ei ole vasemmassa reunassa
        button0 = this->findChild<QPushButton *>(QString::number(nimi.toInt()-1));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
        olemassa[0]=true;
        }
    if (nimi.toInt()%boardSize!=0){
        button1 = this->findChild<QPushButton *>(QString::number(nimi.toInt()+1));
        olemassa[1]=true;
        }
    if (nimi.toInt()>boardSize){
        button2 = this->findChild<QPushButton *>(QString::number(nimi.toInt()-boardSize));
        olemassa[2]=true;
        }
    if (nimi.toInt()<(boardSize*boardSize)-boardSize){
        button3 = this->findChild<QPushButton *>(QString::number(nimi.toInt()+boardSize));
        olemassa[3]=true;
        }

       //naapurit(button0,button1,button2,button3); // kutsuu seuraavan naapurit, tarkistaa onko ne piiritetty
    if (olemassa[0]){
    if (kaappaus(button0))      // tarkistetaan onko vapauksia, lisää varmistus että on olemassa
    {
        button0->setText("kuoli");
    }}
    if (olemassa[1]){
    if (kaappaus(button1))
    {
        button1->setText("kuoli");
    }}
    if (olemassa[2]){
    if (kaappaus(button2))
    {
        button2->setText("kuoli");
    }}
    if (olemassa[3]){
    if (kaappaus(button3))
    {
        button3->setText("kuoli");
    }}
}

void MainWindow::naapurit(QPushButton * left, QPushButton * right, QPushButton * up, QPushButton * down)
{
    int vapaudet=3;
    if (left->objectName()=="20"&&right->objectName()=="22"&&up->objectName()=="12"&&down->objectName()=="30")
        qDebug()<<"ruutu 21, eli 3,3";

        if(left->text()==vastustaja){
            QString nimi=left->objectName();
            if (nimi.toInt()%boardSize!=1) {                // jos kivi ei ole vasemmassa reunassa
                button0 = this->findChild<QPushButton *>(QString::number(nimi.toInt()-1));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
                if (button0->text()==pelaaja){
                    qDebug()<< "vieressä oleva kivi on vastustaja";
                    vapaudet--;
                }
            }
            else vapaudet--;            // kivi on reunassa, 1 vapaus vähemmän
            if (nimi.toInt()%boardSize!=0){
                button1 = this->findChild<QPushButton *>(QString::number(nimi.toInt()+1));
                if (button1->text()==pelaaja) {
                    qDebug()<< "oikealla oleva kivi on vastustaja";
                    vapaudet--;
                }
            }
            else vapaudet--;
            if (nimi.toInt()>boardSize){
                button2 = this->findChild<QPushButton *>(QString::number(nimi.toInt()-boardSize));
                if (button2->text()==pelaaja) {
                    qDebug()<< "yllä oleva kivi on vastustaja";
                    vapaudet--;
                }
            }
            else vapaudet--;
            if (nimi.toInt()<(boardSize*boardSize)-boardSize){
                button3 = this->findChild<QPushButton *>(QString::number(nimi.toInt()+boardSize));
                if (button3->text()==pelaaja) {
                    qDebug()<< "alla oleva kivi on vastustaja";
                    vapaudet--;
                }
            }
            else vapaudet--;
            }
        qDebug()<<vapaudet;
        if (vapaudet==0)
        {
            left->setText("kuoli");
        }


}

bool MainWindow::kaappaus(QPushButton * uhri)
{
        int vapaudet=3;
            if(uhri->text()==vastustaja){
                QString nimi=uhri->objectName();
                if (nimi.toInt()%boardSize!=1) {                // jos kivi ei ole vasemmassa reunassa
                    QPushButton *buttonA = this->findChild<QPushButton *>(QString::number(nimi.toInt()-1));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
                    if (buttonA->text()==pelaaja){
                        vapaudet--;
                    }
                }
                else vapaudet--;            // kivi on reunassa, 1 vapaus vähemmän
                if (nimi.toInt()%boardSize!=0){
                    QPushButton *buttonB = this->findChild<QPushButton *>(QString::number(nimi.toInt()+1));
                    if (buttonB->text()==pelaaja) {
                        vapaudet--;
                    }
                }
                else vapaudet--;
                if (nimi.toInt()>boardSize){
                    QPushButton *buttonC = this->findChild<QPushButton *>(QString::number(nimi.toInt()-boardSize));
                    if (buttonC->text()==pelaaja) {
                        vapaudet--;
                    }
                }
                else vapaudet--;
                if (nimi.toInt()<(boardSize*boardSize)-boardSize){
                    QPushButton *buttonD = this->findChild<QPushButton *>(QString::number(nimi.toInt()+boardSize));
                    if (buttonD->text()==pelaaja) {
                        vapaudet--;
                    }
                }
                else vapaudet--;
                }
            if (vapaudet==0)
            {
                return 1;
            }
    else return 0;
}

void MainWindow::on_btnSmallBoard_clicked()
{
    boardSize=9;
    if (!lautaLuotu) createLayout();
}

void MainWindow::on_btnMediumBoard_clicked()
{
    boardSize=13;
    if (!lautaLuotu) createLayout();
}

void MainWindow::on_btnNormalBoard_clicked()
{
    boardSize=19;
    if (!lautaLuotu) createLayout();
}
