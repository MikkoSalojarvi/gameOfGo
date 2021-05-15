#include "lauta.h"
#include "ui_lauta.h"

lauta::lauta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lauta)
{
    ui->setupUi(this);

}

lauta::~lauta()
{
    qDebug()<< "closing";
    delete ui;
}

void lauta::createLayout(int koko)
{
    boardSize=koko;
    QSize btnSize;
    if (koko ==19)
    {
        btnSize=QSize(60,60);
        this->resize(1100,1400);
    }
    else if(koko==13)
    {
        btnSize=QSize(75,75);
        this->resize(1000,1000);
    }
    else if(koko==9)
    {
        btnSize=QSize(100,100);
        this->resize(900,900);
    }
    QPixmap tausta(":/pictures/board.jpg");
    tausta =tausta.scaled(this->size(),Qt::IgnoreAspectRatio);
    QPalette paletti;
    paletti.setBrush(QPalette::Window, tausta);
    this->setPalette(paletti);
    int nimet=1;                // voisi olla taulu kivet, jossa [nimi, tila(tyhjä/musta/valkea)]?
    for(int i=1; i <= boardSize; i++){
        for(int j=1; j <= boardSize; j++){
//            QString teksti=QString::number(i);
//            teksti.append(" "+QString::number(j));
            ui->ristikko->addWidget(new QPushButton(" ", this),i,j,Qt::Alignment(Qt::AlignLeft));
            QLayoutItem* item = ui->ristikko->itemAtPosition(i, j);
            QPushButton* btn = qobject_cast<QPushButton *>(item->widget());
            //btn->QPushButton::setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            btn->setObjectName(QString::number(nimet));
            btn->setFixedSize(btnSize);
            btn->setStyleSheet("QPushButton{background:transparent;}");
            connect(btn, &QPushButton::clicked, this, &lauta::kivenAsetus);
            //ui->ristikko->setColumnMinimumWidth(j,2);
            paikat.append(btn);
            ui->ristikko->setSpacing(0);
            nimet++;
            }
        //ui->ristikko->setRowMinimumHeight(i,10);
        }
}

void lauta::kivenAsetus()
{
    QObject * sender = QObject::sender();
    QString paikka = sender->objectName();
    foreach(QPushButton * ptr, paikat)
    {
        if(ptr ->objectName() == paikka)
        {
            pelattu=ptr->objectName();
            if (naapurit(ptr))// hae naapurit findchild, nimet +1,-1,+9,-9
            {
                if (ptr->text()!="musta"&&ptr->text()!="valkea")
                {
//                    QString nimi = ptr->objectName();
//                    nimi.append(pelaaja);
//                    ptr->setObjectName(nimi);
                    ptr->setText(pelaaja);
                    ptr->setStyleSheet("QPushButton{color:rgb(155, 0, 55);}");
                    vaihdaPelaaja(ptr);
                }
            }
        }
    }
}

bool lauta::naapurit(QPushButton * kivi)   // hae naapurit findchild, nimet +1,-1,+9,-9
{
    QString nimi=kivi->objectName();
    int omatVapaudet=4;
    if (nimi.toInt()%boardSize!=1)
    {              // jos kivi ei ole vasemmassa reunassa
        button0 = this->findChild<QPushButton *>(QString::number(nimi.toInt()-1));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
        if (kaappaus(button0))      // tarkistetaan onko vapauksia
        {
            button0->setStyleSheet("QPushButton{background:transparent;}");
            button0->setText("");
        }
        if (button0->text()==vastustaja) omatVapaudet--; // jos tätä naapuria ei kaapattu ja se on vastustajan
    }
    else omatVapaudet--;
    if (nimi.toInt()%boardSize!=0)
    {
        button1 = this->findChild<QPushButton *>(QString::number(nimi.toInt()+1));

        if (kaappaus(button1))
        {
            button1->setStyleSheet("QPushButton{background:transparent;}");
            button1->setText("");
        }if (button1->text()==vastustaja) omatVapaudet--;
    }
    else omatVapaudet--;
    if (nimi.toInt()>boardSize){
        button2 = this->findChild<QPushButton *>(QString::number(nimi.toInt()-boardSize));

        if (kaappaus(button2))
        {
            button2->setStyleSheet("QPushButton{background:transparent;}");
            button2->setText("");
        }if (button2->text()==vastustaja) omatVapaudet--; // jos tätä naapuria ei kaapattu ja se on vastustajan

        }
    else omatVapaudet--;
    if (nimi.toInt()<(boardSize*boardSize)-boardSize){
        button3 = this->findChild<QPushButton *>(QString::number(nimi.toInt()+boardSize));

        if (kaappaus(button3))
        {
            button3->setStyleSheet("QPushButton{background:transparent;}");
            button3->setText("");
        }if (button3->text()==vastustaja) omatVapaudet--; // jos tätä naapuria ei kaapattu ja se on vastustajan

        }
    else omatVapaudet--;
/*  // vanha tapa varmistaa että on olemassa ja syödä tarvittaessa
    if (olemassa[0]){
    if (kaappaus(button0))      // tarkistetaan onko vapauksia
    {
        button0->setStyleSheet("QPushButton{background:transparent;}");
        button0->setText("");
    }
    if (button0->text()==vastustaja) omatVapaudet--; // jos tätä naapuria ei kaapattu ja se on vastustajan
    }
    if (olemassa[1]){
    if (kaappaus(button1))
    {
        button1->setStyleSheet("QPushButton{background:transparent;}");
        button1->setText("");
    }if (button1->text()==vastustaja) omatVapaudet--; // jos tätä naapuria ei kaapattu ja se on vastustajan
    }
    if (olemassa[2]){
    if (kaappaus(button2))
    {
        button2->setStyleSheet("QPushButton{background:transparent;}");
        button2->setText("");
    }if (button2->text()==vastustaja) omatVapaudet--; // jos tätä naapuria ei kaapattu ja se on vastustajan
    }
    if (olemassa[3]){
    if (kaappaus(button3))
    {
        button3->setStyleSheet("QPushButton{background:transparent;}");
        button3->setText("");
    }if (button3->text()==vastustaja) omatVapaudet--; // jos tätä naapuria ei kaapattu ja se on vastustajan
    }
    */
    if (omatVapaudet==0){ // jos omia vapauksia ei ole siirto on laiton ja mitään ei tapahdu
        return 0;
    }
    return 1;
}

bool lauta::kaappaus(QPushButton * uhri)   // tutkii onko naapuri vihollisen kivi ja poistaa sen jos se on piiritetty
{
    int vapaudet=3;
    if(uhri->text()==vastustaja){
        QString nimi=uhri->objectName();
        if (nimi.toInt()%boardSize!=1) {                // jos kivi ei ole vasemmassa reunassa
            QPushButton *buttonA = this->findChild<QPushButton *>(QString::number(nimi.toInt()-1));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
            if (buttonA->text()==pelaaja){
                vapaudet--;
            }
            // lisää osa jos text()==vastustaja lisää tutkittavaan ryhmään ja tarkasta sen uhristatus
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

void lauta::vaihdaPelaaja(QPushButton* pointer)
{
    if (pelaaja == "musta")
    {
        pointer->setStyleSheet("border-image: url(:/pictures/musta1Vara1.png) 0 0 0 0 stretch stretch;");
        pelaaja= "valkea";
        vastustaja="musta";
    }
    else {
        pointer->setStyleSheet("border-image: url(:/pictures/valkea1.png) 0 0 0 0 stretch stretch;");
        pelaaja= "musta";
        vastustaja="valkea";
    }
}

void lauta::on_btnQuit_clicked()
{
    this->close();
}

void lauta::on_btnPass_clicked()
{
    if (pelattu!="pass"){
        if (pelaaja == "musta")
        {
            pelaaja= "valkea";
            vastustaja="musta";
        }
        else
        {
            pelaaja= "musta";
            vastustaja="valkea";
        }
    }

    pelattu="pass";
    qDebug()<<pelattu;
}
