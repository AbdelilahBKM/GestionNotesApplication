#ifndef COURS_H
#define COURS_H
#include <QMainWindow>
#include <QVariant>
#include <QVariantList>
#include <QDebug>
#include "basedonnes.h"
class Cours {
private:
    int numeroCours;
    QString titulaire;
    QString  filiere;
public:
    Cours(){
        numeroCours = 0;
    }
    Cours(int code, const QString& titre, QString specialite)
        : numeroCours(code), titulaire(titre), filiere(specialite) {}

    // Getters
    int getNumeroCours() const {
        return numeroCours;
    }

    QString getTitulaire() const {
        return titulaire;
    }

    QString getFiliere() const {
        return filiere;
    }

    // Setters
    void setNumeroCours(int numero) {
        numeroCours = numero;
    }

    void setTitulaire(const QString& nomTitulaire) {
        titulaire = nomTitulaire;
    }

    void setFiliere(const QString& nomFiliere) {
        filiere = nomFiliere;
    }

    static QVariantList getCoursesByFiliere(GestionBaseDonnees& dbManager, const QString& filiere_c) {
        QString queryStr = QString("SELECT * FROM Cours WHERE filiere = '%1'").arg(filiere_c);
        QVariantList result = dbManager.SelectAllCours(queryStr);
        qDebug() << "Nombre de résultats pour la filière" << filiere_c << ":" << result.size();
        return result;
    }

};

#endif // COURS_H
