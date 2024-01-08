#ifndef EVALUATION_H
#define EVALUATION_H
#include <QMainWindow>
#include <QVariant>
#include <QVariantList>
#include <QDebug>
#include "basedonnes.h"
class Evaluation {
private:
    int numeroEvaluation;
    int etudiantAssocie;
    int coursAssoicie;
    double noteObtenu;
    QString dateEvaluation;
public:
    Evaluation(int numero, int etudiant, int cours, double note, const QString& date)
        : numeroEvaluation(numero), etudiantAssocie(etudiant), coursAssoicie(cours), noteObtenu(note), dateEvaluation(date)
    {}
};

#endif // EVALUATION_H
