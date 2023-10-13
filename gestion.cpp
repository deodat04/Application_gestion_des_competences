#include "gestion.h"
#include "database.h"
#include <qsqlquery.h>
#include <qsqlrecord.h>
#include <QSqlError>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QtWidgets>
#include <QLayout>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QString>


gestion::gestion()
{

}

void Database::ouvrirgenerer()
{
    //cette methode permet à l'utilsateur de choisir entre l'utilisation de la base de donnee intégrée et l'importation du fichier.cpp
    QDialog *dialog = new QDialog;

            QLabel *label = new QLabel("Ceci est une image");
            label->setPixmap(QPixmap("r.png"));

            QPushButton *bouton1 = new QPushButton("Utiliser la base de donnée intégrée");
            QPushButton *bouton2 = new QPushButton("Charger un fichier.cpt");
            QVBoxLayout *layout = new QVBoxLayout;
            bouton1->move(50,50);
            bouton2->move(60,60);
            layout->addWidget(bouton1);
            layout->addWidget(bouton2);

            QVBoxLayout *mainLayout = new QVBoxLayout;
            mainLayout->addWidget(label);
            mainLayout->addLayout(layout);

            dialog->setLayout(mainLayout);
            dialog->show();

            connect(bouton1, &QPushButton::clicked, this, &Database::boutongenerer1);
            connect(bouton2, &QPushButton::clicked, this, &Database::boutongenerer2);
}


void Database::ouvrirapropos()
{

    //methode concernant les infos sur l'application
    QMessageBox::information(this, "A Propos", "Bienvenu dans CPT Manager!\n Ce logiciel est mis en place pour permettre de gérer les compétences et profils des professionnels de la base de donnée intégrée. Vous pourrez également utlisée les données d'un fichier.cpt que vous importerai.\n Découvrez notre logiciel plus amplement en cliquant sur START ! .\n\n \t\tRéalisé par Déodat ADANDEDJAN\t\t \n \t\tCopyright , Tous droits réservés\t\t");
}

void Database::boutongenerer1()
{
    //interface de gestion de la base de donnée

        QGroupBox *mainGroupBox = new QGroupBox;
        QVBoxLayout *mainLayout = new QVBoxLayout;

        //competence
        QGroupBox *groupBox1 = new QGroupBox;
        QVBoxLayout *groupBox1Layout = new QVBoxLayout;
        QLabel *image1 = new QLabel;
        image1->setPixmap(QPixmap("competence.png"));
        QPushButton *buttoncompetence = new QPushButton("Gérer");
        groupBox1Layout->addWidget(image1);
        groupBox1Layout->addWidget(buttoncompetence);
        groupBox1->setLayout(groupBox1Layout);
        groupBox1->setTitle("Compétence");

        //profil
        QGroupBox *groupBox2 = new QGroupBox;
        QVBoxLayout *groupBox2Layout = new QVBoxLayout;
        QLabel *image2 = new QLabel;
        image2->setPixmap(QPixmap("profil.jpg"));
        QPushButton *buttonprofil = new QPushButton("Gérer");
        groupBox2Layout->addWidget(image2);
        groupBox2Layout->addWidget(buttonprofil);
        groupBox2->setLayout(groupBox2Layout);
        groupBox2->setTitle("Profil");

        //professionnel
        QGroupBox *groupBox3 = new QGroupBox;
        QVBoxLayout *groupBox3Layout = new QVBoxLayout;
        QLabel *image3 = new QLabel;
        image3->setPixmap(QPixmap("professionnel.jpg"));
        QPushButton *buttonprofessionnel = new QPushButton("Gérer");
        groupBox3Layout->addWidget(image3);
        groupBox3Layout->addWidget(buttonprofessionnel);
        groupBox3->setLayout(groupBox3Layout);
        groupBox3->setTitle("Professionnel");

        //reinitialisation
        QGroupBox *groupBox4 = new QGroupBox;
        QVBoxLayout *groupBox4Layout = new QVBoxLayout;
        QLabel *image4 = new QLabel;
        image4->setPixmap(QPixmap("reinitialiser.jpg"));
        QPushButton *buttonreset = new QPushButton("Réinitialiser");
        groupBox4Layout->addWidget(image4);
        groupBox4Layout->addWidget(buttonreset);
        groupBox4->setLayout(groupBox4Layout);
        groupBox4->setTitle("Réinitialiser la base de donnée");

        QHBoxLayout *groupBoxLayout1 = new QHBoxLayout;
        groupBoxLayout1->addWidget(groupBox1);
        groupBoxLayout1->addWidget(groupBox2);
        QHBoxLayout *groupBoxLayout2 = new QHBoxLayout;
        groupBoxLayout2->addWidget(groupBox3);
        groupBoxLayout2->addWidget(groupBox4);

        mainLayout->addLayout(groupBoxLayout1);
        mainLayout->addLayout(groupBoxLayout2);
        //mainLayout->addWidget(groupBox3);
        mainGroupBox->setLayout(mainLayout);

        //bouton pour quitter l'app
        QPushButton *buttonquitter = new QPushButton("Quitter");
        QObject::connect(buttonquitter, SIGNAL(clicked()), qApp, SLOT(quit()));
        mainLayout->addWidget(buttonquitter);

        //creation des signaux
        //pour le 1er connect, quand on clique sur le bouton mis au niveau de competence, cela ouvre, le slot ouvrir_gestioncompetence()
        connect(buttoncompetence, &QPushButton::clicked, this, &Database::ouvrir_gestioncompetence);
        connect(buttonprofessionnel, &QPushButton::clicked, this, &Database::ouvrir_gestionprofessionnel);
        connect(buttonprofil, &QPushButton::clicked, this, &Database::ouvrir_gestionprofil);
       // connect(buttonreset, &QPushButton::clicked, this, &Database::ouvrir_resetdatabase);


        mainGroupBox->show();
}

void Database::boutongenerer2(QString filepath)
{
/*
    // ce slot permet à l'utilisateur d'importer uniquement un fichier.cpt
    QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"), "/path/to/default/directory", tr("Text files (*.cpt)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                qDebug() << line;
            }
            file.close();
        }
    }
*/
    reinitiliser();
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line = in.readLine();

    while (!line.isNull()) {
        if (line.startsWith("PFL:")) {
            // Récupération des informations de profil
            QString profileName = in.readLine().trimmed();
            int numberSkills = in.readLine().toInt();

            // Insertion du profil dans la base de données
            QSqlQuery query;
            query.prepare("INSERT INTO profiles (name_profile, number_skills_profile) VALUES (:name, :number)");
            query.bindValue(":name", profileName);
            query.bindValue(":number", numberSkills);
            query.exec();

            // Récupération de l'identifiant du profil inséré
            int profileID = query.lastInsertId().toInt();

            // Récupération des compétences associées au profil
            for (int i = 0; i < numberSkills; i++) {
                QString skillName = in.readLine().trimmed();

                // Vérification si la compétence existe déjà dans la base de données
                query.prepare("SELECT ID_skill FROM skills WHERE skill=:name");
                query.bindValue(":name", skillName);
                query.exec();

                int skillID;
                if (query.next()) {
                    // Si la compétence existe, on récupère son identifiant
                    skillID = query.value(0).toInt();
                } else {
                    // Sinon, on l'ajoute dans la base de données et on récupère son identifiant
                    query.prepare("INSERT INTO skills (skill) VALUES (:name)");
                    query.bindValue(":name", skillName);
                    query.exec();
                    skillID = query.lastInsertId().toInt();
                }

                // Ajout de la compétence associée au profil dans la table profiles_skills
                query.prepare("INSERT INTO profiles_skills (profilesID_profile, skillsID_skill) VALUES (:profileID, :skillID)");
                query.bindValue(":profileID", profileID);
                query.bindValue(":skillID", skillID);
                query.exec();
            }
        }

        else if (line.startsWith("PRO:")) {
            // Récupération des informations du professionnel
            QString firstName = in.readLine().trimmed();
            QString lastName = in.readLine().trimmed();
            int numberSkills = in.readLine().toInt();

            // Insertion du professionnel dans la base de données
            QSqlQuery query;
            query.prepare("INSERT INTO professionals (First_name, Last_name, number_of_skills) VALUES (:firstName, :lastName, :numberSkills)");
            query.bindValue(":firstName", firstName);
            query.bindValue(":lastName", lastName);
            query.bindValue(":numberSkills", numberSkills);
            query.exec();

            // Récupération de l'identifiant du professionnel inséré
            int professionalID = query.lastInsertId().toInt();

            // Récupération des compétences associées au professionnel
            for (int i = 0; i < numberSkills; i++) {
                QString skillName = in.readLine().trimmed();

                // Vérification si la compétence existe déjà dans la base de données
                query.prepare("SELECT ID_skill FROM skills WHERE skill=:name");
                query.bindValue(":name", skillName);
                query.exec();

                int skillID;
                if (query.next()) {
                    // Si la compétence existe, on récupère son identifiant
                    skillID = query.value(0).toInt();
                }
                else {
                    // La compétence n'existe pas, l'ajouter à la table des compétences et récupérer son ID
                    query.prepare("INSERT INTO skills (skill) VALUES (:skillName)");
                    query.bindValue(":skillName", skillName);
                    query.exec();
                    skillID = query.lastInsertId().toInt();
                }

                // Ajouter la relation entre le professionnel et la compétence
                query.prepare("INSERT INTO professionals_skills(professionalsID_professinal,skillsID_skill) VALUES (:id_professional, :id_skill)");
                query.bindValue(":id_professional", professionalID);
                query.bindValue(":id_skill", skillID);
                query.exec();
            }
        }
        else if (line.startsWith("CPT:")) {
            // Récupération des informations du professionnel
            int numberSkills = in.readLine().toInt();
            QString Skill = in.readLine().trimmed();


            // Insertion du professionnel dans la base de données
            QSqlQuery query;

            // Récupération des compétences associées au professionnel
            for (int i = 0; i < numberSkills; i++) {
                QString skillName = in.readLine().trimmed();

                // Vérification si la compétence existe déjà dans la base de données
                query.prepare("SELECT ID_skill FROM skills WHERE skill=:name");
                query.bindValue(":name", skillName);
                query.exec();

                if (query.next()) {
                    // Si la compétence existe, on récupère son identifiant

                }
                else {
                    // La compétence n'existe pas, l'ajouter à la table des compétences et récupérer son ID
                    query.prepare("INSERT INTO skills (skill) VALUES (:skillName)");
                    query.bindValue(":skillName", skillName);
                    query.exec();

                }
            }
        }

        else if (line.trimmed().isEmpty()) {
            // Action à réaliser lorsqu'une ligne vide est rencontrée
            //qDebug() << "Ligne vide trouvée";
        }

        // Lecture de la prochaine ligne
        line = in.readLine();
    }

    // Fermeture du fichier
    file.close();

}


//gestion competence
void Database::ouvrir_gestioncompetence()
{
    QGroupBox *groupboxPrincipal = new QGroupBox("Gestion des compétences");
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Créer une nouvelle compétence
    QGroupBox *groupbox1 = new QGroupBox("Créer une nouvelle compétence");
    QLineEdit *lineEditNom = new QLineEdit();
    QPushButton *boutoncreercpt = new QPushButton("Valider");
    QFormLayout *layoutGroupbox1 = new  QFormLayout;
    layoutGroupbox1->addRow("Nom de la compétence:", lineEditNom);
    layoutGroupbox1->addRow(boutoncreercpt);
    groupbox1->setLayout(layoutGroupbox1);
    QObject::connect(boutoncreercpt, &QPushButton::clicked, [=]() {
            QString Nom = lineEditNom->text();
            addskills(Nom);

        });


    // Mettre à jour une compétence
    QGroupBox *groupbox2 = new QGroupBox("Mettre à jour le nom d'une compétence");
    QComboBox *comboAncienneCompetence = new QComboBox();
    QSqlQuery query;
    query.exec("SELECT skill FROM skills");
    while (query.next()) {
        comboAncienneCompetence->addItem(query.value(0).toString());
    }
    comboAncienneCompetence->setCurrentText("Faites votre choix");
    QLineEdit *newcompetence = new QLineEdit();
    QPushButton *boutonupdatecpt = new QPushButton("Valider");
    QFormLayout *layoutGroupbox2 = new QFormLayout;
    layoutGroupbox2->addRow("Ancienne compétence:", comboAncienneCompetence);
    layoutGroupbox2->addRow("Nouvelle compétence:", newcompetence);
    layoutGroupbox2->addRow(boutonupdatecpt);
    groupbox2->setLayout(layoutGroupbox2);


    // Supprimer une compétence
    QGroupBox *groupbox3 = new QGroupBox("Supprimer une compétence");
    QSqlQuery querysuprr;
    querysuprr.exec("SELECT skill FROM skills");
    QComboBox *comboSupprimerCompetence = new QComboBox();
    while (querysuprr.next()) {
      comboSupprimerCompetence->addItem(querysuprr.value(0).toString());
    }
    QPushButton *boutonSupprimer = new QPushButton("Supprimer");
    QFormLayout *layoutGroupbox3 = new QFormLayout;
    layoutGroupbox3->addRow("Compétence à supprimer:", comboSupprimerCompetence);
    layoutGroupbox3->addRow(boutonSupprimer);
    groupbox3->setLayout(layoutGroupbox3);

    QObject::connect(boutonSupprimer, &QPushButton::clicked, [=]() {
            QString Nom =comboSupprimerCompetence->currentText();
            deleteskills(Nom);

        });

    //ajouter ou retirer une competence a un professionnel

    QGroupBox *groupbox4 = new QGroupBox("Ajouter/Retirer une competence à un professionnel");
    QSqlQuery querry;
    querry.exec("SELECT First_name FROM professionals");
    QComboBox *combonomProfessionnel = new QComboBox();
    while (querry.next()) {
        combonomProfessionnel->addItem(querry.value(0).toString());
    }
    QSqlQuery qry;
    qry.exec("SELECT Last_name FROM professionals");
    QComboBox *combonom = new QComboBox();
    while (qry.next()) {
        combonom->addItem(qry.value(0).toString());
    }
     QLineEdit *competence = new QLineEdit;
    /*
    QComboBox *combonomCompetence = new QComboBox();
    QSqlQuery querycpt;
    querycpt.exec("SELECT skill FROM skills");
    while (querycpt.next()) {
      combonomCompetence->addItem(querycpt.value(0).toString());
    }
*/
    //combonomCompetence->setCurrentText("Faites votre choix");
    QPushButton *boutonAjouter = new QPushButton("Ajouter");
    QPushButton *boutonSupprimer2 = new QPushButton("Supprimer");
    QFormLayout *layoutGroupbox4 = new QFormLayout;
    layoutGroupbox4->addRow("Nom Professionnel:", combonomProfessionnel );
    layoutGroupbox4->addRow("Prénom Professionnel:", combonom );
    layoutGroupbox4->addRow("competence:", competence);
    layoutGroupbox4->addRow(boutonAjouter);
    layoutGroupbox4->addRow(boutonSupprimer2);
    groupbox4->setLayout(layoutGroupbox4);

    QObject::connect(boutonAjouter, &QPushButton::clicked, [=]() {
        QString firstname = combonomProfessionnel->currentText();
        QString lastname = combonom->currentText();
        QString skill = competence->text();
        addprofessionalskills(firstname,lastname,skill); });


    QObject::connect(boutonSupprimer2, &QPushButton::clicked, [=]() {
        QString firstname = combonomProfessionnel->currentText();
        QString lastname = combonom->currentText();
        QString skill = competence->text();
        deleteProfessionalSkill(firstname,lastname,skill); });


    //ajouter ou retirer une competence a un profil

    QGroupBox *groupbox5 = new QGroupBox("Ajouter/Retirer une competence à un profil");
    QComboBox *combonomProfil = new QComboBox();
    QSqlQuery querypfl;
    querypfl.exec("SELECT profile_name FROM profiles");
    while (querypfl.next()) {
      combonomProfil->addItem(querypfl.value(0).toString());
    }
    combonomProfil->setCurrentText("Faites votre choix");
     QLineEdit *competenceProfil = new QLineEdit;
    /*
    QComboBox *combonomCompetence2 = new QComboBox();
    QSqlQuery querycpt2;
    querycpt2.exec("SELECT skill FROM skills");
    while (querycpt2.next()) {
      combonomCompetence2->addItem(querycpt2.value(0).toString());
    }
    combonomCompetence2->setCurrentText("Faites votre choix");
    */
    QPushButton *boutonAjouter2 = new QPushButton("Ajouter");
    QPushButton *boutonSupprimer3 = new QPushButton("Supprimer");
    QFormLayout *layoutGroupbox5 = new QFormLayout;
    layoutGroupbox5->addRow("Nom Profil:", combonomProfil );
    layoutGroupbox5->addRow("Nom Compétence:", competenceProfil);
    layoutGroupbox5->addRow(boutonAjouter2);
    layoutGroupbox5->addRow(boutonSupprimer3);
    groupbox5->setLayout(layoutGroupbox5);

    QObject::connect(boutonAjouter2, &QPushButton::clicked, [=]() {
        QString profilename = combonomProfil->currentText();
        QString skill =competenceProfil->text();
        addprofileskills(profilename,skill); });

    QObject::connect(boutonSupprimer3, &QPushButton::clicked, [=]() {
        QString profilename = combonomProfil->currentText();
        QString skill =competenceProfil->text();
        deleteProfIlSkill(profilename,skill); });


    QPushButton *listercpt = new QPushButton("Lister les compétences");

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(groupbox1);
    groupBoxLayout->addWidget(groupbox2);
    groupBoxLayout->addWidget(groupbox3);
    groupBoxLayout->addWidget(groupbox4);
    groupBoxLayout->addWidget(groupbox5);
    groupBoxLayout->addWidget(listercpt);


    mainLayout->addLayout(groupBoxLayout);
    groupboxPrincipal->setLayout(mainLayout);
    groupboxPrincipal->show();


   connect(listercpt,&QPushButton::clicked, this, &Database::ouvrir_listercpt);
}
void Database::ouvrir_listercpt()
{
    // Exécuter la requête SQL pour récupérer toutes les compétences
    QSqlQuery query;
    query.prepare("SELECT DISTINCT skill FROM skills ORDER BY skill ASC");
    query.exec();

    QDialog* dialog = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Ajout d'un label pour afficher le nom du professionnel
    QString title = QString("Affichage de toute les competences de la base de données");
    QLabel* titleLabel = new QLabel(title, dialog);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    layout->addWidget(titleLabel);

    // Ajout d'un widget QTextEdit pour afficher les compétences
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);

    // Ajout des compétences à la QTextEdit
    QStringList skillsList;
    while (query.next()) {
        QString skill = query.value(0).toString();
        skillsList << skill;
    }
    textEdit->setPlainText(skillsList.join("\n"));

    // Affichage de la fenêtre de dialogue
    dialog->exec();

    // Suppression de la fenêtre de dialogue pour éviter les fuites de mémoire
    delete dialog;
}
void Database::addskills(QString const& name)
{
    QSqlQuery request;
    request.prepare("INSERT INTO skills(skill) VALUES(:skill)");
    request.bindValue(":skill",name);
    bool success = request.exec();
    if(success) {
        QMessageBox::information(nullptr, "Competence ajoutée", "La competence a été ajouté avec succès.");
    } else {
        QMessageBox::critical(nullptr, "Erreur d'ajout", "Impossible d'ajouter la competence.");
    }
}
int gestion::getIdskill(QString name_skill)
{
    QSqlQuery query;
    query.prepare("SELECT ID_skill FROM skills WHERE skill= :name_skill");
    query.bindValue(":name_skill",name_skill);
    query.exec();

    // Vérification s'il y a un résultat
    if (query.next()) {
        int id = query.value(0).toInt();
        //qDebug() << "ID de la compétence: " << id;
        return id;
    } else {
        // Gérer le cas où la requête ne renvoie aucun résultat
       // qDebug() << "Aucun résultat trouvé pour la compétence : " << name_skill;
        return -1; // Ou retourner une valeur indiquant qu'aucun résultat n'a été trouvé
    }
}

void Database::deleteskills(QString name)
{
    QSqlQuery request;
    request.prepare("DELETE FROM skills WHERE ID_skill=:id");
    request.bindValue(":id",(getIdskill(name)));
    bool success = request.exec();
    if(success) {
        QMessageBox::information(nullptr, "Competence supprimée", "La competence a été supprimée avec succès.");
    } else {
        QMessageBox::critical(nullptr, "Erreur de suppression", "Impossible de supprimer la competence.");
    }
}
int Database::getIdskill(QString name_skill)
{
    QSqlQuery query;
    query.prepare("SELECT ID_skill FROM skills WHERE skill= :name_skill");
    query.bindValue(":name_skill",name_skill);
    query.exec();

    // Vérification s'il y a un résultat
    if (query.next()) {
        int id = query.value(0).toInt();
        //qDebug() << "ID de la compétence: " << id;
        return id;
    } else {
        // Gérer le cas où la requête ne renvoie aucun résultat
       // qDebug() << "Aucun résultat trouvé pour la compétence : " << name_skill;
        return -1; // Ou retourner une valeur indiquant qu'aucun résultat n'a été trouvé
    }
}

int Database::getIdprofessionel(QString firstName, QString lastName)
{
    QSqlQuery query;
    query.prepare("SELECT ID_professional FROM professionals WHERE First_name = :firstName AND Last_name = :lastName");
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.exec();
    if (query.next()) {
        int id = query.value(0).toInt();
        return id;
    } else {

        return -1; // Ou retourner une valeur indiquant qu'aucun résultat n'a été trouvé
    }
}


bool Database::addprofessionalskills(QString first_name, QString last_name,QString skill_name)
{

    int idprofessinnel=0;
    idprofessinnel=getIdprofessionel(first_name,last_name);
    int idcompetence=0;
    idcompetence=ajouterCompetence(skill_name);
    QSqlQuery request;
    request.prepare("INSERT INTO professionals_skills(professionalsID_professinal,skillsID_skill) VALUES(:professionalsID_professinal,:skillsID_skill)");
    request.bindValue(":professionalsID_professinal",idprofessinnel);
    request.bindValue(":skillsID_skill",idcompetence);
    bool success = request.exec();
    if(success) {qDebug() << "Competence du professionnel Ajoutée";
        actualiserCompetenceProfessionel(first_name,last_name);}
    else qDebug() << "Erreur lors de l'ajout du professionnel";
    return success;
}
void  Database::deleteProfessionalSkill(QString first_name,QString last_name,QString skill)
{
   ;
    // Ouvrir la base de données
    QSqlDatabase database = QSqlDatabase::database();

    // Préparer la requête SQL pour supprimer la compétence associée au professionnel
    QSqlQuery query(database);
    query.prepare("DELETE FROM professionals_skills WHERE professionalsID_professinal = :professionalId AND skillsID_skill = :skillId");
    query.bindValue(":professionalId",getIdprofessionel(first_name,last_name));
    query.bindValue(":skillId", getIdskill(skill));

    // Exécuter la requête SQL
    // Exécuter la requête SQL
      if (query.exec()) {
          QMessageBox::information(this, "Succès", "Compétence supprimée avec succès !");
      } else {
          QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression de la compétence :\n" + query.lastError().text());
      }
}
void Database::actualiserCompetenceProfessionel(QString firstName,QString lastName)
{

    int number_skills_professionel = numberskillprofessinel(firstName, lastName);
        int id = getIdprofessionel(firstName, lastName);
        QSqlQuery request;
        request.prepare("UPDATE professionals SET number_of_skills = :number_of_skills WHERE ID_professional = :id");
        request.bindValue(":number_of_skills", number_skills_professionel); // Correction: bindValue() prend le nom du paramètre, pas le nom de la variable
        request.bindValue(":id", id);
        request.exec();

}

int  Database::ajouterCompetence(QString competence)
{


    // Création de la requête d'insertion
    QSqlQuery query;
    query.prepare("INSERT INTO skills (skill) VALUES (:skill)");
    query.bindValue(":skill", competence);

    // Exécution de la requête
    if (!query.exec()) {
        qWarning() << "Erreur lors de l'ajout de la compétence : " << query.lastError().text();
        return -1;
    }

    // Récupération de l'ID_skill généré automatiquement
    int id_skill = query.lastInsertId().toInt();
    qDebug() << "L'ID_skill de la compétence ajoutée est : " << id_skill;

    return id_skill;
}
//Permet de récupérer le nombre de compétences associées à un profil

int  Database::numberskillprofessinel(QString firstName,QString lastName)
{
    int professionelId = getIdprofessionel(firstName,lastName);
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) AS num_skills FROM professionals_skills WHERE professionalsID_professinal = :professionelId") ;
    query.bindValue(":professionelId",professionelId);
    query.exec();
    query.next();
    int n = query.value(0).toInt();
    qDebug() << n;
    return n;
}
void  Database::deleteProfIlSkill(QString profile_name,QString skill)
{
   ;
    // Ouvrir la base de données
    QSqlDatabase database = QSqlDatabase::database();

    // Préparer la requête SQL pour supprimer la compétence associée au professionnel
    QSqlQuery query(database);
    query.prepare("DELETE FROM profiles_skills WHERE profilesID_profile = :profilId AND skillsID_skill = :skillId");
    query.bindValue(":profilId",getIdprofil(profile_name));
    query.bindValue(":skillId", getIdskill(skill));

    // Exécuter la requête SQL
    // Exécuter la requête SQL
      if (query.exec()) {
          QMessageBox::information(this, "Succès", "Compétence supprimée avec succès !");
      } else {
          QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression de la compétence :\n" + query.lastError().text());
      }
}
bool  Database::addprofileskills(QString profile_name,QString skill_name)
{
    int idprofil=0;
    idprofil=getIdprofil(profile_name);
    int idcompetence=0;
    idcompetence=ajouterCompetence(skill_name);
    QSqlQuery request;
    request.prepare("INSERT INTO profiles_skills(profilesID_profile,skillsID_skill) VALUES(:profilesID_profile,:skillsID_skill)");
    request.bindValue(":profilesID_profile",idprofil);
    request.bindValue(":skillsID_skill",idcompetence);
    bool success = request.exec();
    if(success)
    {qDebug() << "Competence du professionnel Ajoutée";
    actualiserCompetenceProfile(profile_name);}
    else qDebug() << "Erreur lors de l'ajout du professionnel";
    return success;
}
int Database::getIdprofil(QString name_profile)
{
    QSqlQuery query;
    query.prepare("SELECT ID_profile FROM profiles WHERE name_profile = :name_profile");
    query.bindValue(":name_profile", name_profile);
    query.exec();

    // Vérification s'il y a un résultat
    if (query.next()) {
        int id = query.value(0).toInt();
        return id;
    } else {

        return -1; // Ou retourner une valeur indiquant qu'aucun résultat n'a été trouvé
    }
}
void Database::actualiserCompetenceProfile(QString profile_name)
{

    int number_skills_profile = numberskillprofile(profile_name);
       int id = getIdprofil(profile_name);
       QSqlQuery request;
       request.prepare("UPDATE profiles SET number_skills_profile = :number_skills_profile WHERE ID_profile = :id");
       request.bindValue(":number_skills_profile", number_skills_profile);
       request.bindValue(":id", id);
       request.exec();
}
//Permet de récupérer le nombre de compétences associées à un profil
int Database::numberskillprofile(QString name_profile)
{
    int profileId = getIdprofil(name_profile);
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) AS num_skills FROM profiles_skills WHERE profilesID_profile = :profileId") ;
    query.bindValue(":profileId", profileId);
    query.exec();
    query.next();
    int n = query.value(0).toInt();
    qDebug() << n;
    return n;
}




//gestion professionnels
void Database::ouvrir_gestionprofessionnel()
{

    QGroupBox *groupboxPrincipal = new QGroupBox("Gestion des professionnels");
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Créer un nouveau professionnel
    QGroupBox *groupbox1 = new QGroupBox("Créer un nouveau professionnel");
    QLineEdit *lineeditnom = new QLineEdit();
    QLineEdit *prenom = new QLineEdit;
    QPushButton *boutoncreerpro = new QPushButton("Valider");
    QFormLayout *layoutGroupbox1 = new  QFormLayout;
    layoutGroupbox1->addRow("Nom du professionnel:", lineeditnom);
    layoutGroupbox1->addRow("Prénom du professionnel", prenom);
    layoutGroupbox1->addRow(boutoncreerpro);
    groupbox1->setLayout(layoutGroupbox1);

    QObject::connect(boutoncreerpro, &QPushButton::clicked, [=]() {
        QString Nom = lineeditnom->text();
        QString Prenom = prenom->text();
        addprofessional(Nom,Prenom); });



    // Afficher les infos d'un professionnel
    QGroupBox *groupbox2 = new QGroupBox("Afficher les infos d'un professionnel");
    QSqlQuery queryNom;
    queryNom.exec("SELECT First_name FROM professionals");
    QComboBox *comboNomProfessionnel = new QComboBox();
    while (queryNom.next()) {
        QString nom = queryNom.value(0).toString();
        comboNomProfessionnel->addItem(nom);
    }
    comboNomProfessionnel->setCurrentText("Faites votre choix");

    QSqlQuery queryPrenom;
    queryPrenom.exec("SELECT Last_name FROM professionals");

    QComboBox *comboPrenomProfessionnel = new QComboBox();
    while (queryPrenom.next()) {
        QString prenom = queryPrenom.value(0).toString();
        comboPrenomProfessionnel->addItem(prenom);
    }
    comboPrenomProfessionnel->setCurrentText("Faites votre choix");
    QPushButton *boutonaffichercptpro = new QPushButton("Voir les compétences du professionnel");
     QPushButton *presenter = new QPushButton("Présentation du professionnel");
    QFormLayout *layoutGroupbox2 = new QFormLayout;
    layoutGroupbox2->addRow("Nom Professionnel:", comboNomProfessionnel);
    layoutGroupbox2->addRow("Prenom Professionnel:", comboPrenomProfessionnel);
    layoutGroupbox2->addRow(boutonaffichercptpro);
    layoutGroupbox2->addRow(presenter);
    groupbox2->setLayout(layoutGroupbox2);

    // slot voir les compétences du professionnel
    QObject::connect(boutonaffichercptpro, &QPushButton::clicked, [=]() {
        QString nom = comboNomProfessionnel->currentText();
        QString prenom = comboPrenomProfessionnel->currentText();
        displayProfessionalSkills(nom, prenom);
    });

    QObject::connect(presenter, &QPushButton::clicked, [=]() {
        QString nom = comboNomProfessionnel->currentText();
        QString prenom = comboPrenomProfessionnel->currentText();
        displayProfessional(nom, prenom);
    });

    // Supprimer un professionnel
    QGroupBox *groupbox3 = new QGroupBox("Supprimer un professionnel");
    QLineEdit *nomProfessionel = new QLineEdit;
    QLineEdit *prenomProfessionel = new QLineEdit;
    /*
    QSqlQuery querysupp;
    querysupp.exec("SELECT first_name FROM professionals");
    QComboBox *comboSupprimerpro = new QComboBox();
    while (querysupp.next()) {
      comboSupprimerpro->addItem(querysupp.value(0).toString());
    }
    QComboBox *comboprepro = new QComboBox();
    QSqlQuery querysup;
    querysup.exec("SELECT last_name FROM professionals");
    while (querysup.next()) {
        comboprepro->addItem(querysup.value(0).toString());
    }
    comboprepro->setCurrentText("Faites votre choix");
    */
    QPushButton *boutonSupp = new QPushButton("Supprimer");
    QFormLayout *layoutGroupbox3 = new QFormLayout;
    layoutGroupbox3->addRow("Nom Professionnel :", nomProfessionel);
    layoutGroupbox3->addRow("Prénom Professionnel :", prenomProfessionel);
    layoutGroupbox3->addRow(boutonSupp);
    groupbox3->setLayout(layoutGroupbox3);

    QObject::connect(boutonSupp, &QPushButton::clicked, [=]() {
        QString Nom = nomProfessionel->text();
        QString Prenom = prenomProfessionel->text();
        deleteProfessional(Nom,Prenom); });



    //modifier les infos d'un professionnel

    QGroupBox *groupbox4 = new QGroupBox("Modifier les informations d'un professionnel");
    QSqlQuery querry;
    querry.exec("SELECT first_name FROM professionals ");
    QComboBox *combonamepro = new QComboBox();
    while (querry.next()) {
        combonamepro->addItem(querry.value(0).toString());
    }
    QSqlQuery myquery;
    myquery.exec("SELECT last_name FROM professionals ");
    QComboBox *combolastpro = new QComboBox();
    while (querry.next()) {
        combolastpro->addItem(myquery.value(0).toString());
    }
    QComboBox *combonamecpt = new QComboBox();
    QSqlQuery querycpt;
    querycpt.exec("SELECT skill FROM skills");
    while (querycpt.next()) {
      combonamecpt->addItem(querycpt.value(0).toString());
    }
    combonamecpt->setCurrentText("Faites votre choix");
    QPushButton *boutonajoutercpt = new QPushButton("Ajouter Compétence");
    QPushButton *boutonsupprimercpt = new QPushButton("Supprimer Compétence");
    QFormLayout *layoutGroupbox4 = new QFormLayout;
    layoutGroupbox4->addRow("Nom Professionnel:", combonamepro);
    layoutGroupbox4->addRow("Prenom Professionnel:",combolastpro);
    layoutGroupbox4->addRow("Nom Compétence:", combonamecpt);
    layoutGroupbox4->addRow(boutonajoutercpt);
    layoutGroupbox4->addRow(boutonsupprimercpt);
    groupbox4->setLayout(layoutGroupbox4);

    QPushButton *listerprofessionnels = new QPushButton("Lister les professionnels");

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(groupbox1);
    groupBoxLayout->addWidget(groupbox2);
    groupBoxLayout->addWidget(groupbox3);
    groupBoxLayout->addWidget(groupbox4);
    groupBoxLayout->addWidget(listerprofessionnels);


    mainLayout->addLayout(groupBoxLayout);
    groupboxPrincipal->setLayout(mainLayout);
    groupboxPrincipal->show();

    connect(listerprofessionnels, &QPushButton::clicked, this, &Database::ouvrir_listerprofessionnels);
    //connect(boutonafficherinfospro, &QPushButton::clicked, this, &Database::ouvrir_afficherinfospro);
}
void Database::displayProfessional(QString firstName, QString lastName)
{
    // Requête pour récupérer l'ID du professionnel
    int professionalId = getIdprofessionel(firstName, lastName);
    int totalSkills = numberskillprofessinel(firstName, lastName);
    QSqlQuery query;

    // Requête pour récupérer les compétences du professionnel
    query.prepare("SELECT skills.skill FROM professionals_skills "
                  "JOIN skills ON professionals_skills.skillsID_skill = skills.ID_skill "
                  "WHERE professionals_skills.professionalsID_professinal = :professional_id");
    query.bindValue(":professional_id", professionalId);
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        return;
    }

    // Création de la fenêtre de dialogue pour afficher les compétences
    QDialog* dialog = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Ajout d'un label pour afficher le nom du professionnel
    QString title = QString("Compétences de %1 %2 (%3)").arg(firstName, lastName).arg(totalSkills);
    QLabel* titleLabel = new QLabel(title, dialog);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    layout->addWidget(titleLabel);

    // Ajout d'un widget QTextEdit pour afficher les compétences
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);

    // Ajout des compétences à la QTextEdit
    QStringList skillsList;
    while (query.next()) {
        QString skill = query.value(0).toString();
        skillsList << skill;
    }
    textEdit->setPlainText(skillsList.join("\n"));

    // Affichage de la fenêtre de dialogue
    dialog->exec();

    // Suppression de la fenêtre de dialogue pour éviter les fuites de mémoire
    delete dialog;
}

void Database::displayProfessionalSkills(QString firstName, QString lastName)
{
    // Requête pour récupérer l'ID du professionnel
    int professionalId = getIdprofessionel(firstName, lastName);
    QSqlQuery query;

    // Requête pour récupérer les compétences du professionnel
    query.prepare("SELECT skills.skill FROM professionals_skills "
                  "JOIN skills ON professionals_skills.skillsID_skill = skills.ID_skill "
                  "WHERE professionals_skills.professionalsID_professinal = :professional_id");
    query.bindValue(":professional_id", professionalId);
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        return;
    }

    // Création de la fenêtre de dialogue pour afficher les compétences
    QDialog* dialog = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Ajout d'un label pour afficher le nom du professionnel
    QString title = QString("Compétences de %1 %2").arg(firstName, lastName);
    QLabel* titleLabel = new QLabel(title, dialog);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    layout->addWidget(titleLabel);

    // Ajout d'un widget QTextEdit pour afficher les compétences
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);

    // Ajout des compétences à la QTextEdit
    QStringList skillsList;
    while (query.next()) {
        QString skill = query.value(0).toString();
        skillsList << skill;
    }
    textEdit->setPlainText(skillsList.join("\n"));

    // Affichage de la fenêtre de dialogue
    dialog->exec();

    // Suppression de la fenêtre de dialogue pour éviter les fuites de mémoire
    delete dialog;
}

void Database::ouvrir_listerprofessionnels()
{
    // Exécuter la requête SQL pour récupérer tous les professionnels
    QSqlQuery query;
    query.prepare("SELECT first_name, last_name FROM professionals ORDER BY first_name");

    query.exec();

    // Créer un QDialog pour afficher les professionnels
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle("Liste des professionnels");

    // Créer un QVBoxLayout pour organiser les widgets dans le QDialog
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Ajouter un QLabel pour le titre
    QLabel* titleLabel = new QLabel("Liste des professionnels", dialog);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    layout->addWidget(titleLabel);

    // Ajouter un QTextEdit pour afficher les professionnels
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);

    // Parcourir les résultats de la requête et ajouter chaque professionnel au QTextEdit
    QStringList professionalList;
    while (query.next()) {
        QString firstName = query.value(0).toString();
        QString lastName = query.value(1).toString();
        QString professional = firstName + " " + lastName;
        professionalList << professional;
    }
    textEdit->setPlainText(professionalList.join("\n"));

    // Afficher le QDialog
    dialog->exec();

    // Supprimer le QDialog pour éviter les fuites de mémoire
    delete dialog;
}

void Database::addprofessional(QString const First_name, QString const Last_name)
{
    QSqlQuery request;
    request.prepare("INSERT INTO professionals(First_name,Last_name) VALUES(:First_name,:Last_name)");
    request.bindValue(":First_name", First_name);
    request.bindValue(":Last_name", Last_name);
    bool success = request.exec();

    if(success) {
        QMessageBox::information(nullptr, "Professionnel ajouté", "Le professionnel a été ajouté avec succès.");
    } else {
        QMessageBox::critical(nullptr, "Erreur d'ajout", "Impossible d'ajouter le professionnel.");
    }
}
void Database::deleteProfessional(QString First_name, QString Last_name)
{

    QSqlQuery query;
    query.prepare("DELETE FROM professionals WHERE ID_professional= :id");
    query.bindValue(":id", getIdprofessionel(First_name,Last_name));
    bool success = query.exec();
    if(success) {
        QMessageBox::information(nullptr, "Professionnel supprimé", "Le professionnel a été supprimé avec succès.");
    } else {
        QMessageBox::critical(nullptr, "Erreur de suppression", "Impossible de supprimer le professionnel.");
    }
}


//gestion profile
void Database::ouvrir_gestionprofil()
{

    QGroupBox *groupboxPrincipal = new QGroupBox("Gestion des profils");
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Créer une nouveau profil
    QGroupBox *groupbox1 = new QGroupBox("Créer un nouveau profil");
    QLineEdit *lineEditnom = new QLineEdit();
    QPushButton *boutoncreerprofil = new QPushButton("Valider");
    QFormLayout *layoutGroupbox1 = new  QFormLayout;
    layoutGroupbox1->addRow("Nom du profil:", lineEditnom);
    layoutGroupbox1->addRow(boutoncreerprofil);
    groupbox1->setLayout(layoutGroupbox1);

    QObject::connect(boutoncreerprofil, &QPushButton::clicked, [=]() {
            QString Nom = lineEditnom->text();
            addprofile(Nom);

        });

    // Afficher les infos d'un profil
    QGroupBox *groupbox2 = new QGroupBox("Afficher les infos d'un profil");
    QComboBox *combonomprofil = new QComboBox();
    QSqlQuery query;
    query.exec("SELECT name_profile FROM profiles");
    while (query.next()) {
        combonomprofil->addItem(query.value(0).toString());
    }
    combonomprofil->setCurrentText("Faites votre choix");
    QPushButton *boutonaffichercptprofil = new QPushButton("Afficher compétences profil");
     QPushButton *boutonafficherprofil = new QPushButton("Afficher infos profil");
    QFormLayout *layoutGroupbox2 = new QFormLayout;
    layoutGroupbox2->addRow("Profil:", combonomprofil);
    layoutGroupbox2->addRow(boutonaffichercptprofil);
    layoutGroupbox2->addRow(boutonafficherprofil);
    groupbox2->setLayout(layoutGroupbox2);

    //slot voir les compétences du profil

  QObject::connect(boutonaffichercptprofil, &QPushButton::clicked, [=]() {
          QString Nom = combonomprofil->currentText();
          displayProfilSkills(Nom);

      });
  QObject::connect(boutonafficherprofil, &QPushButton::clicked, [=]() {
          QString Nom = combonomprofil->currentText();
          displayProfil(Nom);

      });


    // Supprimer un profil
    QGroupBox *groupbox3 = new QGroupBox("Supprimer un profil");
    QSqlQuery querysupp;
    querysupp.exec("SELECT profile_name FROM profiles");
    QComboBox *comboSupprimerProfil = new QComboBox();
    while (querysupp.next()) {
      comboSupprimerProfil->addItem(querysupp.value(0).toString());
    }
    QPushButton *boutonSupp = new QPushButton("Supprimer");
    QFormLayout *layoutGroupbox3 = new QFormLayout;
    layoutGroupbox3->addRow("Profil à supprimer:", comboSupprimerProfil);
    layoutGroupbox3->addRow(boutonSupp);
    groupbox3->setLayout(layoutGroupbox3);

    QObject::connect(boutonSupp, &QPushButton::clicked, [=]() {
        QString Nom = comboSupprimerProfil->currentText();
        deleteProfil(Nom); });

    //modifier les infos d'un profil

    QGroupBox *groupbox4 = new QGroupBox("Modifier les informations d'un profil");
    QSqlQuery querry;
    querry.exec("SELECT  profile_name FROM profiles");
    QComboBox *combonameprofil = new QComboBox();
    while (querry.next()) {
        combonameprofil->addItem(querry.value(0).toString());
    }
    QComboBox *combonamecpt = new QComboBox();
    QSqlQuery querycpt;
    querycpt.exec("SELECT skill FROM skills");
    while (querycpt.next()) {
      combonamecpt->addItem(querycpt.value(0).toString());
    }
    combonamecpt->setCurrentText("Faites votre choix");
    QPushButton *boutonajoutercpt = new QPushButton("Ajouter Compétence");
    QPushButton *boutonsupprimercpt = new QPushButton("Supprimer Compétence");
    QFormLayout *layoutGroupbox4 = new QFormLayout;
    layoutGroupbox4->addRow("Nom Profil:", combonameprofil);
    layoutGroupbox4->addRow("Nom Compétence:", combonamecpt);
    layoutGroupbox4->addRow(boutonajoutercpt);
    layoutGroupbox4->addRow(boutonsupprimercpt);
    groupbox4->setLayout(layoutGroupbox4);

    //rajouter une description a un profil

    QGroupBox *groupbox5 = new QGroupBox("Rajouter une description a un profil");
    QComboBox *combonompfl = new QComboBox();
    QSqlQuery querypfl;
    querypfl.exec("SELECT profile_name FROM profiles");
    while (querypfl.next()) {
      combonompfl->addItem(querypfl.value(0).toString());
    }
    combonompfl->setCurrentText("Faites votre choix");
    QTextEdit *textedit = new QTextEdit();
    QPushButton *boutonAjouterdescrip = new QPushButton("Valider");

    QFormLayout *layoutGroupbox5 = new QFormLayout;
    layoutGroupbox5->addRow("Nom Profil:", combonompfl);
    layoutGroupbox5->addRow("Description:", textedit);
    layoutGroupbox5->addRow(boutonAjouterdescrip);


    groupbox5->setLayout(layoutGroupbox5);

    QPushButton *listercptprofil = new QPushButton("Lister les profils");

    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
    groupBoxLayout->addWidget(groupbox1);
    groupBoxLayout->addWidget(groupbox2);
    groupBoxLayout->addWidget(groupbox3);
    groupBoxLayout->addWidget(groupbox4);
    groupBoxLayout->addWidget(groupbox5);
    groupBoxLayout->addWidget(listercptprofil);

    mainLayout->addLayout(groupBoxLayout);
    groupboxPrincipal->setLayout(mainLayout);
    groupboxPrincipal->show();

    connect(listercptprofil, &QPushButton::clicked, this, &Database::ouvrir_listcptprofil);
    //connect(boutonafficherinfosprofil, &QPushButton::clicked, this, &Database::ouvrir_afficherinfosprofil);
}
void Database::displayProfilSkills(QString name_profile)
{
    // Récupérer l'identifiant du profil professionnel
    int profileId =getIdprofil( name_profile);

    // Récupérer les compétences associées à ce profil professionnel dans la base de données
    QSqlQuery query;
    query.prepare("SELECT skills.skill FROM skills INNER JOIN profiles_skills ON skills.ID_skill = profiles_skills.skillsID_skill WHERE profiles_skills.profilesID_profile = :profileId");
    query.bindValue(":profileId", profileId);
    query.exec();

    // Créer une fenêtre pour afficher les compétences
    QDialog* dialog = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Ajouter un titre à la fenêtre
    QString title = QString("Compétences du profil %1").arg(name_profile);
    QLabel* titleLabel = new QLabel(title, dialog);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    layout->addWidget(titleLabel);

    // Ajouter un widget QTextEdit pour afficher les compétences
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);

    // Ajouter les compétences à afficher dans le widget QTextEdit
    QStringList skillsList;
    while (query.next()) {
        QString skillName = query.value(0).toString();
        skillsList << skillName;
    }
    textEdit->setPlainText(skillsList.join("\n"));

    // Afficher la fenêtre de dialogue
    dialog->exec();

    // Libérer la mémoire utilisée par la fenêtre de dialogue
    delete dialog;
}

void Database::displayProfil(QString name_profile)
{
    // Récupérer l'identifiant du profil professionnel
    int profileId = getIdprofil(name_profile);

    // Récupérer le nombre total de compétences associées à ce profil professionnel
    int totalSkills = numberskillprofile(name_profile);

    // Récupérer les compétences associées à ce profil professionnel dans la base de données
    QSqlQuery query;
    query.prepare("SELECT skills.skill FROM skills INNER JOIN profiles_skills ON skills.ID_skill = profiles_skills.skillsID_skill WHERE profiles_skills.profilesID_profile = :profileId");
    query.bindValue(":profileId", profileId);
    query.exec();

    // Créer une fenêtre pour afficher les compétences
    QDialog* dialog = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Ajouter un titre à la fenêtre avec le nombre total de compétences
    QString title = QString("Compétences du profil %1 (%2)").arg(name_profile).arg(totalSkills);
    QLabel* titleLabel = new QLabel(title, dialog);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    layout->addWidget(titleLabel);

    // Ajouter un widget QTextEdit pour afficher les compétences
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);

    // Ajouter les compétences à afficher dans le widget QTextEdit
    QStringList skillsList;
    while (query.next()) {
        QString skillName = query.value(0).toString();
        skillsList << skillName;
    }
    textEdit->setPlainText(skillsList.join("\n"));

    // Afficher la fenêtre de dialogue
    dialog->exec();

    // Libérer la mémoire utilisée par la fenêtre de dialogue
    delete dialog;
}
void Database::ouvrir_listcptprofil()
{
    // Exécuter la requête SQL pour récupérer tous les profils
    QSqlQuery query;
    query.prepare("SELECT DISTINCT profile_name FROM profiles ORDER BY profile_name ASC");
    query.exec();

    QDialog* dialog = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    // Ajout d'un label pour afficher le titre de la fenêtre
    QString title = QString("Affichage des profils de la base de données");
    QLabel* titleLabel = new QLabel(title, dialog);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    layout->addWidget(titleLabel);

    // Ajout d'un widget QTextEdit pour afficher les profils
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);

    // Ajout des profils à la QTextEdit
    QStringList profileList;
    while (query.next()) {
        QString profile = query.value(0).toString();
        profileList << profile;
    }
    textEdit->setPlainText(profileList.join("\n"));

    // Affichage de la fenêtre de dialogue
    dialog->exec();

    // Suppression de la fenêtre de dialogue pour éviter les fuites de mémoire
    delete dialog;
}

void Database::addprofile(QString const& name_profile)
{
    QSqlQuery request;
    request.prepare("INSERT INTO profiles(name_profile) VALUES(:name_profile)");
    request.bindValue(":name_profile",name_profile);
    bool success = request.exec();
    if(success) {
        QMessageBox::information(nullptr, "Profil ajouté", "Le profil a été ajouté avec succès.");
    } else {
        QMessageBox::critical(nullptr, "Erreur d'ajout", "Impossible d'ajouter le profil.");
    }
}

bool Database::deleteProfil(QString name_profile)
{
    QSqlQuery query;
    query.prepare("DELETE FROM profiles WHERE `name_profile` = :name_profile");
    query.bindValue(":name_profile", name_profile);
    bool success = query.exec();
    if (success) {
        qDebug() << "Profil supprimé";
    } else {
        qDebug() << "Erreur lors de la suppression du profil";
    }
    return success;
}

void Database::ouvrir_resetdatabase()
{
    QStringList tables = bdd.tables();
    bool resetDone = false; // Added boolean variable to keep track of reset status

    foreach (QString table, tables) {
        QSqlQuery query;
        query.exec("DELETE FROM " + table);
        if (query.lastError().isValid()) {
            // Traitement de l'erreur
        } else {
            resetDone = true; // If no error occurred, set resetDone to true
        }
    }

    // Display message alerting user if reset was done or not
    if (resetDone) {
        QMessageBox::information(nullptr, "Réinitialisation réussie", "La base de données a été réinitialisée.");
    } else {
        QMessageBox::warning(nullptr, "Réinitialisation échouée", "La base de données n'a pas été réinitialisée.");
    }
}
void Database::reinitiliser()
{
    QStringList tables = bdd.tables();
   // Added boolean variable to keep track of reset status

    foreach (QString table, tables) {
        QSqlQuery query;
        query.exec("DELETE FROM " + table);
    }

}

void Database::createCptFile(QString path) {
    QString filename = path + "/example.cpt";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // Write professionals
        QSqlQuery query_professionals("SELECT * FROM professionals");
        while (query_professionals.next()) {
            out << "PRO:" << endl;
            out << "\t"<<query_professionals.value("First_name").toString() << endl;
            out << "\t"<<query_professionals.value("Last_name").toString() << endl;

            int professional_id = query_professionals.value("ID_professional").toInt();
            QSqlQuery query_skills(QString("SELECT * FROM professionals_skills WHERE professionalsID_professinal = %1").arg(professional_id));
            int num_skills = 0;
            while (query_skills.next()) {
                num_skills++;
            }
            out << "\t"<<num_skills << endl;
            query_skills.first();
            while (query_skills.isValid()) {
                int skill_id = query_skills.value("skillsID_skill").toInt();
                QSqlQuery query_skill(QString("SELECT * FROM skills WHERE ID_skill = %1").arg(skill_id));
                query_skill.first();
                out << "\t"<<query_skill.value("skill").toString() << endl;
                query_skills.next();
            }
        }

        // Write profiles
        QSqlQuery query_profiles("SELECT * FROM profiles");
        while (query_profiles.next()) {
            out << "PRL:" << endl;
            out <<"\t"<<query_profiles.value("name_profile").toString() << endl;
            int profile_id = query_profiles.value("ID_profile").toInt();
            QSqlQuery query_skills(QString("SELECT * FROM profiles_skills WHERE profilesID_profile = %1").arg(profile_id));
            int num_skills = 0;
            while (query_skills.next()) {
                num_skills++;
            }
            out << "\t"<<num_skills << endl;
            query_skills.first();
            while (query_skills.isValid()) {
                int skill_id = query_skills.value("skillsID_skill").toInt();
                QSqlQuery query_skill(QString("SELECT * FROM skills WHERE ID_skill = %1").arg(skill_id));
                query_skill.first();
                out <<"\t"<< query_skill.value("skill").toString() << endl;
                query_skills.next();
            }
        }

        // Write unassigned skills
        QSqlQuery query_cpt("SELECT * FROM skills WHERE ID_skill NOT IN (SELECT skillsID_skill FROM professionals_skills UNION SELECT skillsID_skill FROM profiles_skills)");
        int num_skills = -1;
        while (query_cpt.next()) {
            num_skills++;
        }
        if (num_skills > 0) {
            out << "CPT:" << endl;
            out <<"\t"<< num_skills << endl;
            query_cpt.first();
            while (query_cpt.isValid()) {
                out << "\t"<<query_cpt.value("skill").toString() << endl;
                query_cpt.next();
            }
        }

        file.close();
        qDebug() << "File created successfully.";
    }
    else {
        qDebug() << "Failed to create file: " << file.errorString();
    }
}
void Database::choisirCheminExportation()
{
    // Créer un champ de saisie de texte pour le chemin d'exportation
      QLineEdit* exportPathLineEdit = new QLineEdit();

      // Créer un bouton de validation pour enregistrer le chemin d'exportation saisi
      QPushButton* saveExportPathButton = new QPushButton(tr("Enregistrer"));
      QObject::connect(saveExportPathButton, &QPushButton::clicked, [&]() {
          QString exportPath = exportPathLineEdit->text();
          Database::createCptFile(exportPath);
          // Vérifier que le chemin d'exportation saisi est valide

          // Faites quelque chose avec le chemin d'exportation ici...
      });

      // Créer une disposition pour la boîte de dialogue
      QFormLayout* layout = new QFormLayout();
      layout->addRow(tr("Chemin d'exportation :"), exportPathLineEdit);
      layout->addRow(saveExportPathButton);

      // Créer la boîte de dialogue
      QDialog* dialogWindow = new QDialog();
      dialogWindow->setWindowTitle(tr("Chemin d'exportation"));
      dialogWindow->setLayout(layout);

      // Afficher la boîte de dialogue
      dialogWindow->exec();
}
void Database::import()
{
    boutongenerer2("C:/Utilsateur/HP/Téléchargements/sample.cpt");
}

