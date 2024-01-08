#ifndef ENSEIGNANT_H
#define ENSEIGNANT_H
#include <QMainWindow>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QDebug>
#include "basedonnes.h"
class Enseignant{
private:
    int numeroEnseignant;
    QString nomEnseignant;
    QString prenomEnseignant;
    QString specialite;
public:
    Enseignant(){
        numeroEnseignant = 0;
    }
    Enseignant(int numero, const QString& nom_e, const QString& prenom_e, QString specialite)
        : numeroEnseignant(numero), nomEnseignant(nom_e), prenomEnseignant(prenom_e), specialite(specialite)
    {}
    bool authentifierEnseignant(GestionBaseDonnees& dbManager, const QString& nom, const QString& prenom, int numero) {
        QString query = QString("SELECT specialite FROM Enseignant "
                                "WHERE nomEnseignant = '%1' AND prenomEnseignant = '%2'  AND numeroEnseignant = %3")
                            .arg(nom).arg(prenom).arg(numero);

        QVariantList result =  dbManager.executerRequeteSelection(query);
        if(result.isEmpty()){
            return false;
        }
       specialite = dbManager.GetSpecialite(numero);
        numeroEnseignant = numero;
        nomEnseignant = nom;
        prenomEnseignant = prenom;
        qDebug() << numeroEnseignant << nomEnseignant << prenomEnseignant << specialite;
        return true;
    }
    int getNumeroEnseignant(){
        return numeroEnseignant;
    }
    void setNumeroEnseignant(int numero){
        numeroEnseignant = numero;
    }
    QString getNomEnseignant(){
        return nomEnseignant;
    }
    void setNomEnseignant(const QString& nom){
        nomEnseignant = nom;
    }
    QString getPrenomEnseignant(){
        return prenomEnseignant;
    }
    void setPrenomEnseignant(const QString& prenom){
        prenomEnseignant = prenom;
    }
    QString getSpecialiteEnseignant(){
        return specialite;
    }
    void setSpecialiteEnseignant(const QString& spec){
        specialite = spec;
    }
};

#endif // ENSEIGNANT_H
