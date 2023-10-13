#ifndef DATABASE_H
#define DATABASE_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QtWidgets>
#include <QLayout>

class Database : public QWidget
{
    Q_OBJECT

public:
    Database();  
    QSqlDatabase bdd;
    void ouvrir_listerprofessionnels();
    void ouvrir_listcptprofil();
    void ouvrir_listercpt();
    int  getIdskill(QString name_skill);
    void deleteProfessionalSkill(QString first_name,QString last_name,QString skill);
    int getIdprofessionel(QString firstName, QString lastName);
    int ajouterCompetence(QString competence) ;
    void actualiserCompetenceProfessionel(QString firstName,QString lastName);
    int numberskillprofessinel(QString firstName,QString lastName);
    int  getIdprofil(QString name_profile);
    void actualiserCompetenceProfile(QString profile_name);
    int numberskillprofile(QString name_profile);
    bool deleteProfil(QString name_profile);
    void displayProfessionalSkills(QString firstName,QString lastName);
    void displayProfessional(QString firstName, QString lastName);
    void displayProfilSkills(QString profile_name);
    void displayProfil(QString name_profile);
    void reinitiliser();

public slots:
     void ouvrir_resetdatabase();
     void addskills(QString const& name);
     void deleteskills(QString name);
     bool addprofessionalskills(QString  first_name, QString  last_name,QString skill_name);
     bool addprofileskills(QString profile_name,QString skill_name);
     void deleteProfIlSkill(QString profile_name,QString skill);
     void addprofessional(QString const First_name,QString const Last_name);
     void deleteProfessional(QString first_name,QString last_name);
     void addprofile(QString const& name_profile);

private slots:
    void ouvrirgenerer();
    void ouvrirapropos();
    void boutongenerer1();
    void ouvrir_gestionprofessionnel();
    void ouvrir_gestioncompetence();
    void ouvrir_gestionprofil();
    void boutongenerer2(QString filepath);
    void import();
    void createCptFile(QString path);
    void choisirCheminExportation();

private:
    QPushButton *m_bouton;
    QPushButton *m_boutonn;
    QPushButton *buttonquitter;
    QTableWidget *m_tableau;
    QPushButton *buttoncompetence;
    QPushButton *buttonprofessionnel;
    QPushButton *buttonprofil;
    QComboBox *comboSupprimerCompetence;

};
#endif // DATABASE_H
