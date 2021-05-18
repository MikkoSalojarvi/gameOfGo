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
    for(int i=1; i <= boardSize; i++){      // ei tarvi kun keksi miten saa tekstin pois...
        for(int j=1; j <= boardSize; j++){
            ui->ristikko->addWidget(new QPushButton(" ", this),i,j,Qt::Alignment(Qt::AlignLeft));
            QLayoutItem* item = ui->ristikko->itemAtPosition(i, j);
            QPushButton* btn = qobject_cast<QPushButton *>(item->widget());
            btn->setObjectName(QString::number(nimet));
            QPalette pal2= btn->palette();
            QColor lasi;
            //lasi.setAlpha(0);
            lasi.setAlphaF(0);
            pal2.setColor(QPalette::ButtonText,lasi);
            btn->setPalette(pal2);
            btn->setFixedSize(btnSize);
            btn->setStyleSheet("QPushButton{background:transparent;}");
            connect(btn, &QPushButton::clicked, this, &lauta::kivenAsetus);
            paikat.append(btn);
            ui->ristikko->setSpacing(0);
            nimet++;
            }
        }
}

void lauta::kivenAsetus()
{
    QObject * sender = QObject::sender();
    pelattu=sender->objectName();
    int row, col, col_span, row_span;
    ui->ristikko->getItemPosition(pelattu.toInt()-1, &row, &col, &col_span, &row_span);
    qDebug() << "Clicked Item is at:" << row << col << "button number" << sender;
    QPushButton* ptr = qobject_cast<QPushButton *>(sender);
    if (naapurit(ptr))          // hae naapurit findchild, nimet +1,-1,+9,-9
    {
        if (ptr->text()!="musta"&&ptr->text()!="valkea")
        {
            ptr->setText(pelaaja);
            ptr->setStyleSheet("QPushButton{color:rgb(155, 0, 55);}");
            vaihdaPelaaja(ptr);
        }
    }
}
            // tee kaava joka laskee naapurien numerot,tai tarkistaa onko ne olemassa tehokkaamin ettei tarvi jokaiselle naapurille olla omaa funktiota, vaan voi käyttää jotain for funktiota esim
bool lauta::naapurit(QPushButton * kivi)   // hae naapurit findchild, nimet +1,-1,+9,-9
{
    //QString nimi=kivi->objectName();
    int omatVapaudet=4;
    naapurit2(kivi);
    int viereisetKivet2[4];
    for (int j=0;j<4;j++)
            viereisetKivet2[j]=viereisetKivet[j];
    for (int j=0;j<4;j++)
    {
        if (viereisetKivet2[j]!=0)
        {
            QPushButton *button = this->findChild<QPushButton *>(QString::number(viereisetKivet2[j]));
            if (kaappaus(button))      // tarkistetaan onko vapauksia
            {
                button->setStyleSheet("QPushButton{background:transparent;}");
                button->setText("");
            }
            if (button->text()==vastustaja) omatVapaudet--; // jos naapuria ei kaapattu ja se on vastustajan
        }
        else omatVapaudet--;
    }
    /*
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
    */
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
    qDebug()<<omatVapaudet<< " omat vapaudet";
    if (omatVapaudet==0){ // jos omia vapauksia ei ole siirto on laiton ja mitään ei tapahdu
        return 0;
    }
    return 1;
}

void lauta::naapurit2(QPushButton *kivi)
{
    QPushButton *button;
    QString nimi=kivi->objectName();
    if (nimi.toInt()%boardSize!=1)
    {              // jos kivi ei ole vasemmassa reunassa
        button = this->findChild<QPushButton *>(QString::number(nimi.toInt()-1));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
        viereisetKivet[0]=button->objectName().toInt();
    }
    else viereisetKivet[0]=0;
    if (nimi.toInt()%boardSize!=0)
    {              // jos kivi ei ole oikeassa reunassa
        button = this->findChild<QPushButton *>(QString::number(nimi.toInt()+1));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
        viereisetKivet[1]=button->objectName().toInt();
    }
    else viereisetKivet[1]=0;
    if (nimi.toInt()>boardSize)
    {              // jos kivi ei ole ylä reunassa
        button = this->findChild<QPushButton *>(QString::number(nimi.toInt()-boardSize));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
        viereisetKivet[2]=button->objectName().toInt();
    }
    else viereisetKivet[2]=0;
    if (nimi.toInt()<boardSize*(boardSize-1))
    {              // jos kivi ei ole ala reunassa
        button = this->findChild<QPushButton *>(QString::number(nimi.toInt()+boardSize));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
        viereisetKivet[3]=button->objectName().toInt();
    }
    else viereisetKivet[3]=0;
}

bool lauta::kaappaus(QPushButton * uhri)   // tutkii onko naapuri vihollisen kivi ja poistaa sen jos se on piiritetty
{
    int vapaudet=3;
    if(uhri->text()==vastustaja){
        QString nimi=uhri->objectName();
        if (!kasa.contains(nimi))   // lisätään tutkittava uhri kasaan jos se ei ole siellä
        {
            kasa.append(nimi);
            int viereisetKivet3[4];
            naapurit2(uhri);
            for (int j=0;j<4;j++) viereisetKivet3[j]=viereisetKivet[j];
            for (int j=0;j<4;j++)
            {
                if (viereisetKivet3[j]!=0) // j= nolla kun ei ole reunassa
                {
                    QPushButton *button = this->findChild<QPushButton *>(QString::number(viereisetKivet3[j]));
                    //if (nimi.toInt()%boardSize!=1) {              // jos kivi ei ole vasemmassa reunassa
                    //QPushButton *button = this->findChild<QPushButton *>(QString::number(nimi.toInt()-1));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
                    if (button->text()==pelaaja)
                    {
                        vapaudet--;
                    }
                    else if (button->text()==vastustaja) // jos kivi on vastustajan, se lisätään syömäuhan alla olevaan ryhmään
                    {                                    // tätä pitäisi jotenkin saada iteroitua, tapahtunee kasakaappauksessa
                        if(!kasa.contains(button->objectName()))
                        {
                            kasa.append(button->objectName());
                        }
                    }
                }
                else vapaudet--; // kivi on reunassa, 1 vapaus vähemmän
            }


        /*
        if (nimi.toInt()%boardSize!=0){
            QPushButton *button = this->findChild<QPushButton *>(QString::number(nimi.toInt()+1));
            if (button->text()==pelaaja) {
                vapaudet--;
            }
            else if (button->text()==vastustaja) // lisää osa jos text()==vastustaja lisää tutkittavaan ryhmään ja tarkasta sen uhristatus
            {
                if(!kasa.contains(button->objectName()))
                {
                    kasa.append(button->objectName());
                }
            }
        }
        else vapaudet--;
        if (nimi.toInt()>boardSize){
            QPushButton *button = this->findChild<QPushButton *>(QString::number(nimi.toInt()-boardSize));
            if (button->text()==pelaaja) {
                vapaudet--;
            }
            else if (button->text()==vastustaja) // lisää osa jos text()==vastustaja lisää tutkittavaan ryhmään ja tarkasta sen uhristatus
            {
                if(!kasa.contains(button->objectName()))
                {
                    kasa.append(button->objectName());
                }
            }
        }
        else vapaudet--;
        if (nimi.toInt()<(boardSize*boardSize)-boardSize){
            QPushButton *button = this->findChild<QPushButton *>(QString::number(nimi.toInt()+boardSize));
            if (button->text()==pelaaja) {
                vapaudet--;
            }
            else if (button->text()==vastustaja) // lisää osa jos text()==vastustaja lisää tutkittavaan ryhmään ja tarkasta sen uhristatus
            {
                if(!kasa.contains(button->objectName()))
                {
                    kasa.append(button->objectName());
                }
            }
        }
        else vapaudet--;

        */

        }
    }
    if (vapaudet==0)
    {
        return 1;
    }
    else if (kasa.capacity()>1)
    {
        bool tulos= kasaKaappaus();
        return tulos;
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
    kasa.clear();
}

bool lauta::kasaKaappaus()
{
    int alku = kasa.count();
    // int row, col, col_span, row_span;
    foreach(QString nimi,kasa)          // luettelee kasassa eli syömäuhan alla olevat kivet
    {                                   // näille pitäisi tutkia onko näill edes yksi vapaus
        // ui->ristikko->getItemPosition(nimi.toInt(), &row, &col, &col_span, &row_span);
        QPushButton *uhriKivet = this->findChild<QPushButton *>(nimi);
        int viereisetKivet4[4];
        naapurit2(uhriKivet);
        for (int j=0;j<4;j++) viereisetKivet4[j]=viereisetKivet[j];
        for (int j=0;j<4;j++)
        {
            if (viereisetKivet4[j]!=0) // jos kivi ei ole reunassa
            {
                QPushButton *button = this->findChild<QPushButton *>(QString::number(viereisetKivet4[j]));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
                qDebug ()<<button->objectName()<< " nimi "<<button->text()<<" teksti";
                if (button->text()==vastustaja && !kasa.contains(button->objectName()))
                    kasa.append(button->objectName());
                else if (button->text()!=vastustaja && button->text()!=pelaaja && button->objectName()!=pelattu)  // tarkistetaan onko vapauksia
                {       // jos vasemmalla on tyhjä paikka, eli nappi jonka teksti ei ole valkea tai musta

                    kasa.clear(); // ryhmällä on vapaus joten se ei kuole ja tutkinnan voi lopettaa
                    qDebug()<< "kasalla on vapaus";
                    return 0;    // voiko tämän funktion lopettaa tässä muuten kuin returnilla ja vaihtamalla tyypin muuksi?
                }       // jos kasaa ei tyhjennetä mitään kiveää tutkittaessa, sillä ei ole vapauksia
            }
        }
    }
    if (kasa.count()!=0)// kasa tuli syödyksi, vaihda jokainen tyhjäksi ruuduksi
    {
        if (alku< kasa.count()) // kasaan lisättiin kiviä tällä iteraatiolla
        {
             return kasaKaappaus();
        }
        else{
            foreach(QString nimi,kasa)
            {
                QPushButton *uhriKivet = this->findChild<QPushButton *>(nimi);
                qDebug()<< "poistetaan kivi "<< uhriKivet->objectName();
                uhriKivet->setStyleSheet("QPushButton{background:transparent;}");
                uhriKivet->setText("");
            }
        }
        return 1;
    }
    return 0;
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
