#include "database.h"
#include "ui_database.h"

#include "database.h"
#include "ui_database.h"
#include <QDebug>
#include <QSqlQuery>
#include <QPushButton>


Database::Database()
 : comboSupprimerCompetence(nullptr)
{
  //parametre de connexion a la base de donnee
  bdd = QSqlDatabase::addDatabase("QSQLITE");
  bdd.setHostName("127.0.0.1");
  bdd.setDatabaseName("ProjetQt.db");
  bdd.setUserName("root");
  bdd.setPassword("");

//verifier si la base de donnée est ouverte
  if(bdd.open())
      qDebug() <<"Base de donnee ouverte";
  else
      qDebug() << "Erreur d'ouverture";


  //interface graphique qui s'affiche au lancement du programme
  QVBoxLayout *layoutprincipal = new QVBoxLayout;


  QGroupBox *groupbox1 = new QGroupBox("BIENVENUE DANS LE PROJET QT DU GROUPE 1", this);
  groupbox1 -> setFont(QFont("Eras Bold ITC"));
  QFormLayout *layoutt = new QFormLayout;

  QLabel *label = new QLabel();
  label->setPixmap(QPixmap("back.jpg"));
  label->setScaledContents(true);

  QLabel *textLabel = new QLabel();
  textLabel->setText("GESTION DES COMPETENCES");
  textLabel->setAlignment(Qt::AlignCenter);
  textLabel->setStyleSheet("QLabel { background-color : transparent; color : black; font-size: 50px; font-family: Times New Roman }");
  //qDebug() << "Error loading image";

  layoutt->addWidget(label);
  layoutt->addWidget(textLabel);
  groupbox1->setLayout(layoutt);
  groupbox1->move(1,1);

  //creation du bouton START pour commencer l'utilisation de notre app
    QPushButton *m_bouton = new QPushButton("START",this);
    m_bouton -> setFont(QFont("Eras Bold ITC"));
    m_bouton -> setCursor(Qt::PointingHandCursor);
    m_bouton->move(120,300);

    //creation du bouton pour avoir les informations sur l'app
    QPushButton *m_boutonn = new QPushButton("A Propos",this);
    m_boutonn -> setFont(QFont("Eras Bold ITC"));
    m_boutonn -> setCursor(Qt::PointingHandCursor);

  setLayout(layoutprincipal);
  layoutprincipal->addWidget(groupbox1);
  layoutprincipal->addWidget(m_bouton);
  layoutprincipal->addWidget(m_boutonn);

  connect(m_bouton, &QPushButton::clicked, this, &Database::ouvrirgenerer);
  connect(m_boutonn, &QPushButton::clicked, this, &Database::ouvrirapropos);
}
