#ifndef ETUDIANT_H
#define ETUDIANT_H
#include <QMainWindow>
#include <QVariant>
#include <QVariantList>
#include <QDebug>
#include "basedonnes.h"


class Etudiant {
private:
    int numeroEtudiant;
    QString  nomEtudiant;
    QString prenomEtudiant;
    QString  dateNaissance;
    QString filiere;
public:
    Etudiant(){
        this->numeroEtudiant = 0;
    }
    Etudiant(int numero, QString nom, QString prenom, QString date, QString specialite)
        :  numeroEtudiant(numero), nomEtudiant(nom), prenomEtudiant(prenom), dateNaissance(date), filiere(specialite)
    {}
    bool ajouterEtudiant(GestionBaseDonnees& dbManager) {
        QString query = QString("INSERT INTO Etudiant (nomEtudiant, prenomEtudiant, dateNaissance, filiere) "
                                "VALUES ('%1', '%2', '%3', '%4')")
                            .arg(nomEtudiant).arg(prenomEtudiant).arg(dateNaissance).arg(filiere);
        return dbManager.executeQuery(query);
    }
    bool modifierEtudiant(GestionBaseDonnees& dbManager,
                          const QString& nouveauNom,
                          const QString& nouveauPrenom,
                          const QString& nouvelleDateNaissance
                          )
    {
        nomEtudiant = nouveauNom;
        prenomEtudiant = nouveauPrenom;
        dateNaissance = nouvelleDateNaissance;

        QString query = QString("UPDATE Etudiant "
                                "SET nomEtudiant = '%1', "
                                "prenomEtudiant = '%2', "
                                "dateNaissance = '%3' "
                                "WHERE numero = '%4'")
                            .arg(nouveauNom)
                            .arg(nouveauPrenom)
                            .arg(nouvelleDateNaissance)
                            .arg(numeroEtudiant);
        return dbManager.executeQuery(query);
    }
    bool verifierEtudiantExist(GestionBaseDonnees& dbManager) {
        QString query = QString("SELECT numero FROM Etudiant "
                                "WHERE nomEtudiant = '%1' AND prenomEtudiant = '%2'  AND dateNaissance = '%3' AND filiere = '%4' ")
                            .arg(nomEtudiant)
                            .arg(prenomEtudiant)
                            .arg(dateNaissance)
                            .arg(filiere);
        QVariantList result =  dbManager.executerRequeteSelection(query);
        if(!result.isEmpty()){
            numeroEtudiant = result.first().toInt();
            return true;
        }
        return false;
    }
    int getNumeroEtudiant(GestionBaseDonnees& dbManager) {
        QString query = QString("SELECT numero FROM Etudiant "
                                "WHERE nomEtudiant = '%1' AND prenomEtudiant = '%2'")
                            .arg(nomEtudiant)
                            .arg(prenomEtudiant);

        QVariantList result = dbManager.executerRequeteSelection(query);

        if (!result.isEmpty()) {
            numeroEtudiant = result.first().toInt();
            return numeroEtudiant;
        }

        return -1; // Retourner une valeur par défaut si la récupération échoue
    }
    int getNumeroEtudiant(){
        return numeroEtudiant;
    }
    void setNumeroEtudiant(int newNumero){
       numeroEtudiant  = newNumero;
    }
    QString  getNomEtudiant(){
        return nomEtudiant;
    }
    void setNomEtudiant(QString newNom){
        nomEtudiant = newNom;
    }
    QString  getPrenomEtudiant(){
        return prenomEtudiant;
    }
    void setPrenomEtudiant(QString newPrenom){
        prenomEtudiant = newPrenom;
    }

    QString  getDateNaissanceEtudiant(){
        return dateNaissance;
    }
    void setDateNaissanceEtudiant(QString newDate){
        dateNaissance = newDate;
    }
    QString getSpecialite(){
        return filiere;
    }
    void  setSpecialite(QString newFiliere){
       filiere = newFiliere;
    }
};
#endif // ETUDIANT_H

