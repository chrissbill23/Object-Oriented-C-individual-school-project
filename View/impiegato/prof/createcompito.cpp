#include "createcompito.h"
#include "../../../Controller/controllerprof.h"
#include "createxmlcompito.h"
#include "loadpdfcompito.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

QGroupBox *CreateCompito::FirstPage()
{
    QGroupBox* temp = new QGroupBox(this);
    QVBoxLayout* lay = new QVBoxLayout(temp);

    QComboBox* scelte = new QComboBox(temp);
    scelte->addItem("Seleziona un formato", -1);
    list<string> l = ctrl->tipiFileCompito();

    int i = 0;
    for(list<string>::const_iterator it = l.begin(); it != l.end(); ++it ){
        scelte->addItem(QString::fromStdString(*it), i);
        ++i;
    }
    scelte->setFixedSize(500, 50);
    scelte->setFont(QFont("Times", 12));
    lay->addWidget(scelte,0, Qt::AlignHCenter|Qt::AlignTop);

    QPushButton* b = new QPushButton("Invia",temp);
    b->setFixedSize(100,30);
    b->setFont(QFont("Times", 9));
    b->setStyleSheet("QPushButton{"
                         "background-color: #336699; "
                         "border-radius: 5px 5px 5px 5px; "
                         "color: white;}"
                         "QPushButton:pressed {"
                        " background-color:#003300;}");
    b->setCursor(QCursor(Qt::PointingHandCursor));
    connect(b,SIGNAL(clicked(bool)),this, SLOT(gotoForm()));
    lay->addWidget(b,1, Qt::AlignHCenter|Qt::AlignTop);

    temp->setLayout(lay);
    return temp;

}

void CreateCompito::gotoForm()
{
    QWidget* temp = sc->widget();
    QComboBox* combo = temp->findChild<QComboBox*>();
    if(combo && combo->itemData(combo->currentIndex()).toInt() != -1){
        if(combo->itemData(combo->currentIndex()).toInt() == 0){
            delete temp;
            sc->setWidget(new createXMLCompito(ctrl, this));
            sc->setWidgetResizable(true);
            return;
        }
        if(combo->itemData(combo->currentIndex()).toInt() == 1){
            delete temp;
            sc->setWidget(new LoadPDFcompito(ctrl,this));
            sc->setWidgetResizable(true);
            return;
        }

    }
    else{
        QMessageBox::information(0, tr("Nessuna selezione è stata fatta"),
                                 tr("E' richiesta la selezione di un formato."));
    }
}

void CreateCompito::salvaXml()
{
    createXMLCompito* p = dynamic_cast<createXMLCompito*>(sc->widget());
    if(p && p->salva()){
        delete p;
        createXMLCompito * temp = new createXMLCompito(ctrl, this);
        QLabel* error = new QLabel("Salvato con successo!", temp);
        error->setStyleSheet("color: green;");
        error->setFont(QFont("Times", 13));
        temp->setMessage(error);
        sc->setWidget(temp);
        sc->setWidgetResizable(true);
    }
}

void CreateCompito::salvaPdf()
{
    LoadPDFcompito* p = dynamic_cast<LoadPDFcompito*>(sc->widget());
    if(p && p->salva()){
        delete p;
        LoadPDFcompito * temp = new LoadPDFcompito(ctrl, this);
        QLabel* error = new QLabel("Caricato con successo!", temp);
        error->setStyleSheet("color: green;");
        error->setFont(QFont("Times", 13));
        temp->setMessage(error);
        sc->setWidget(temp);
        sc->setWidgetResizable(true);
    }
    else  QMessageBox::information(0, tr("Errore"),
                                   tr("Errore nel salvataggio: verifica di aver compilato\n"
                                      " correttamente tutti i campi, evitando i caratter < & \""));
}


CreateCompito::CreateCompito(ControllerProf *c, QWidget *parent):QDialog(parent), ctrl(c), sc(new QScrollArea(this))
{
    setFixedSize(800,650);
    setStyleSheet("background-color: none;");
    QVBoxLayout* lay = new QVBoxLayout(this);
    sc->setWidget(FirstPage());
    sc->setWidgetResizable(true);
    lay->addWidget(sc);
    setLayout(lay);

    setWindowTitle("Nuovo compito");
}
