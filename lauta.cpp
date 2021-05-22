#include "lauta.h"
#include "ui_lauta.h"

lauta::lauta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lauta)
{
    ui->setupUi(this);
    this->setFocusPolicy(Qt::FocusPolicy (Qt::TabFocus ));

}

lauta::~lauta()
{
    // qDebug()<< "closing";
    delete ui;
}

void lauta::createLayout(int koko)
{
    boardSize=koko;
    QSize btnSize;
    if (koko ==19)              //
    {
        tausta.load(":/pictures/goBoard19.png");
        btnSize=QSize(60,60);   // 19*60=1140  QRect(610,99 1340x1373)
        this->resize(1340,1270); //(1340,1270) toimii senseisboardilla,  pitäisi saada vielä alareuna tyhjäksi
    }
    else if(koko==13)
    {
        tausta.load(":/pictures/goBoard13.png");
        btnSize=QSize(85,85);   // 13*75=975
        this->resize(1200,1200);    // QRect(643,198 1275x1175)
    }
    else if(koko==9)
    {
        tausta.load(":/pictures/goBoard9.png");
        btnSize=QSize(100,100); // 9*100=900    QRect(730,235 1100x1100)
        this->resize(990,970);    // 1100 oikea leveys
    }

    tausta =tausta.scaled(this->size(),Qt::IgnoreAspectRatio);
    QPalette paletti;
    paletti.setBrush(QPalette::Window, tausta);
    this->setPalette(paletti);
    int nimet=1;                // voisi olla taulu kivet, jossa [nimi, tila(tyhjä/musta/valkea)]?
    for(int i=1; i <= boardSize; i++)
    {      // ei tarvi kun keksi miten saa tekstin pois...
        for(int j=1; j <= boardSize; j++)
        {
            ui->ristikko->addWidget(new QPushButton("", this),i,j,Qt::Alignment(Qt::AlignLeft));
            QLayoutItem* item = ui->ristikko->itemAtPosition(i, j);
            QPushButton* btn = qobject_cast<QPushButton *>(item->widget());
            btn->setObjectName(QString::number(nimet));
            QPalette pal2= btn->palette();
            QColor lasi;
            lasi.setAlphaF(0);
            pal2.setColor(QPalette::ButtonText,lasi);
            btn->setPalette(pal2);
            btn->setFixedSize(btnSize);
            btn->setStyleSheet("QPushButton{background:transparent;}");
            connect(btn, &QPushButton::clicked, this, &lauta::kivenAsetus);
            paikat.append(btn);
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
    // qDebug() << "Clicked Item is at:" << row << col << "button number" << sender;
    QPushButton* ptr = qobject_cast<QPushButton *>(sender);
    if (naapurit(ptr))          // hae naapurit findchild, nimet +1,-1,+9,-9
    {
        omaKasa.clear();
        if (ptr->text()!="musta"&&ptr->text()!="valkea")
        {
            ptr->setText(pelaaja);
            ptr->setStyleSheet("QPushButton{color:rgb(155, 0, 55);}");
            vaihdaPelaaja(ptr);
        }
    }
}

bool lauta::naapurit(QPushButton * kivi)   // hae naapurit findchild, nimet +1,-1,+9,-9
{
    omaKasa.append(kivi->objectName());
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
            // qDebug()<<button->text()<< button->objectName();
            if (kaappaus(button))       // tarkistetaan onko vapauksia
            {           /* ongelma nurkassa tapahtuvien kaappauksien kanssa, mahdollisesti tekemistä kasaKaappaus tyhjennyksen kanssa
                        Kun on kaksi ryhmää kaapattavana, ei toiminut. 1-8 kivi ja 2-9,3-9 kivet kun kaappaa kerralla , 1-8 ei kaappaannu
                        */
                button->setStyleSheet("QPushButton{background:transparent;}");
                button->setText("");
            }
            if (button->text()==vastustaja) omatVapaudet--; // jos naapuria ei kaapattu ja se on vastustajan
            else if (button->text()==pelaaja&& !omaKasa.contains(button->objectName())) // jos naapurikivi on oma ja sitä ei vielä ole laskettu omaan ryhmään, lisätään siihen
            {
                omaKasa.append(button->objectName());
            }
            // jos kivi on oma, toista tutkimus sille ja iteroi kunnes löytyy vapaus tai kaikki naapurit on tutkittu
        }
        else
        {
            omatVapaudet--;
        }

    }

    if (omatVapaudet==0){   // jos omia vapauksia ei ole siirto on laiton ja mitään ei tapahdu
                            // tarvitaan vastaava kuin ryhmäkaappauksissa isomman oman ryhmän vapauksien tutkintaan
        return 0;
    }
    else if(omaKasa.count()>1)
    {
    // qDebug()<< "kutsutaan omatKivet";
    return omatKivet();
    }
    return 1;
}

bool lauta::omatKivet()
{
    int lasku = omaKasa.count();
    foreach (QString nimi,omaKasa)
    {
        QPushButton *button = this->findChild<QPushButton *>(nimi);
        // qDebug()<< button->objectName()<<" omaKasassa oleva kivi";
        naapurit2(button);
        int viereisetKivet3[4];
        for (int j=0;j<4;j++) viereisetKivet3[j]=viereisetKivet[j];
        for (int j=0;j<4;j++)
        {

            if (viereisetKivet3[j]!=0)
            {
                QPushButton *button1 = this->findChild<QPushButton *>(QString::number(viereisetKivet3[j]));
                // qDebug()<< button1->objectName()<<" tutkittava nappi";
                if (button1->text()==pelaaja && !omaKasa.contains(button1->objectName()))
                {
                    omaKasa.append(button1->objectName());
                    // qDebug()<< button1->objectName()<<" lisätty omaKasaan";
                }
                else if (button1->text()==""&&button1->objectName()!=pelattu){ // muuten jos vieressä olevassa paikassa ei ole vastustajan
                    // qDebug()<<    button1->objectName()<< " löytyi vapaus tästä";
                    return 1;           // paikka on tyhjä -> ryhmällä on vapaus ja palautetaan 1
                }
            }
        }
    }
    if (omaKasa.count()>lasku)
    {
        // qDebug()<< "kasa kasvanut-> uusi lasku";
        return omatKivet();
    }
    else
    {
        // qDebug()<< "kasa käytyläpi, vapauksia ei löytynyt";
        omaKasa.clear();
        return 0;
    }
}

void lauta::naapurit2(QPushButton *kivi) // tallentaa viereisten kivien nimet, huomioi reunat
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
    {
        button = this->findChild<QPushButton *>(QString::number(nimi.toInt()+1));
        viereisetKivet[1]=button->objectName().toInt();
    }
    else viereisetKivet[1]=0;

    if (nimi.toInt()>boardSize)
    {
        button = this->findChild<QPushButton *>(QString::number(nimi.toInt()-boardSize));
        viereisetKivet[2]=button->objectName().toInt();
    }
    else viereisetKivet[2]=0;

    if (nimi.toInt()<=boardSize*(boardSize-1))
    {
        button = this->findChild<QPushButton *>(QString::number(nimi.toInt()+boardSize));
        viereisetKivet[3]=button->objectName().toInt();
    }
    else viereisetKivet[3]=0;
}

bool lauta::kaappaus(QPushButton * uhri)   // tutkii onko naapuri vihollisen kivi ja poistaa sen jos se on piiritetty
{
    int vapaudet=4;
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
                if (viereisetKivet3[j]!=0) // viereisetKivet3[j]= nolla kun ei ole reunassa
                {
                    QPushButton *button = this->findChild<QPushButton *>(QString::number(viereisetKivet3[j]));
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
        pointer->setStyleSheet("border-image: url(:/pictures/mustaOma2.png) 0 0 0 0 stretch stretch;");
        pelaaja= "valkea";
        vastustaja="musta";
    }
    else {
        pointer->setStyleSheet("border-image: url(:/pictures/valkeaOma2.png) 0 0 0 0 stretch stretch;");
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
    {                                   // näille pitäisi tutkia onko näill edes yksi vapaus :: Done!
        QPushButton *uhriKivet = this->findChild<QPushButton *>(nimi);
        int viereisetKivet4[4];
        naapurit2(uhriKivet);
        for (int j=0;j<4;j++) viereisetKivet4[j]=viereisetKivet[j];
        for (int j=0;j<4;j++)
        {
            if (viereisetKivet4[j]!=0) // jos kivi ei ole reunassa
            {
                QPushButton *button = this->findChild<QPushButton *>(QString::number(viereisetKivet4[j]));  //tehdään viittaus/haetaan nimi vasemmalla olevaan kiveen
                // qDebug ()<<button->objectName()<< " nimi "<<button->text()<<" teksti";
                if (button->text()==vastustaja && !kasa.contains(button->objectName()))
                    kasa.append(button->objectName());
                else if (button->text()!=vastustaja && button->text()!=pelaaja && button->objectName()!=pelattu)  // tarkistetaan onko vapauksia
                {   // jos vasemmalla on tyhjä paikka, eli nappi jonka teksti ei ole valkea tai musta
                    kasa.clear(); // ryhmällä on vapaus joten se ei kuole ja tutkinnan voi lopettaa
                    // qDebug()<< "kasalla on vapaus";
                    return 0;   // voiko tämän funktion lopettaa tässä muuten kuin returnilla ja vaihtamalla tyypin muuksi?
                }               // jos kasaa ei tyhjennetä mitään kiveää tutkittaessa, sillä ei ole vapauksia
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
              // qDebug()<< "poistetaan kivi "<< uhriKivet->objectName();
                uhriKivet->setStyleSheet("QPushButton{background:transparent;}");
                uhriKivet->setText("");
            }
            if (pelaaja=="musta")
            {
                ui->blackCaps->setText(QString::number(ui->blackCaps->text().toInt()+kasa.count()));
            }
            else
            {
                ui->whiteCaps->setText(QString::number(ui->whiteCaps->text().toInt()+kasa.count()));
            }
        }
        kasa.clear();
        return 1;
    }
    return 0;
}

void lauta::keyPressEvent(QKeyEvent *e)
{
    // qDebug()<<"press event";
    if (e->key() == Qt::Key_Alt||e->key()==Qt::Key_P)
    this->passTurn();
}

void lauta::on_btnQuit_clicked()
{
    this->close();
}

void lauta::on_btnPass_clicked()
{
    this->passTurn();
}

void lauta::passTurn()
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
    qDebug()<< this->geometry();
}

void lauta::paintEvent(QPaintEvent *)
{

}
