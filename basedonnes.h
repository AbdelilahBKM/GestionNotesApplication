#ifndef BASEDONNES_H
#define BASEDONNES_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlRecord>

class GestionBaseDonnees{
private:
    QSqlDatabase m_db;

public:
    GestionBaseDonnees() {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("E:/sqlitedatabase/gestionDesNotes.db");
        if (!m_db.open()) {
                qDebug() << "Error opening database:" << m_db.lastError().text();
        } else {
                qDebug() << "Database connected!";
        }
    }

    ~GestionBaseDonnees() {
        m_db.close();
    }
    bool executeQuery(const QString& query) {
        QSqlQuery sqlQuery;
        if (!sqlQuery.exec(query)) {
            qDebug() << "Query error:" << sqlQuery.lastError().text();
            return false;
        }
        return true;
    }

    QVariantList executerRequeteSelection(const QString& queryStr) {
        QVariantList result; // Liste pour stocker les résultats

        QSqlQuery query;
        if (!query.exec(queryStr)) {
            qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
            return result; // Retourner une liste vide en cas d'erreur
        }

        while (query.next()) {
            QVariant numeroVariant = query.value(0);
            result.append(numeroVariant);
        }

        return result;
    }

    QVariantList SelectAllCours(const QString& queryStr) {
        QVariantList result;

        QSqlQuery query;
        if (!query.exec(queryStr)) {
            qDebug() << "Error executing query:" << query.lastError().text();
            return result;
        }

        QSqlRecord record = query.record();

        while (query.next()) {
            QVariantMap coursData;

            // Explicitly fetch required columns by name
            coursData["numeroCours"] = query.value(record.indexOf("numeroCours"));
            coursData["titulaire"] = query.value(record.indexOf("titulaire"));
            coursData["filiere"] = query.value(record.indexOf("filiere"));

            qDebug() << "Retrieved data:" << coursData; // Output retrieved data

            result.append(coursData);
        }

        return result;
    }
    QString GetSpecialite(int numero) {
        QString result;
        QString query = QString("SELECT specialite FROM Enseignant "
                                "WHERE numeroEnseignant = %1")
                            .arg(numero);

        QSqlQuery sqlQuery;
        if (sqlQuery.exec(query) && sqlQuery.next()) {
            result = sqlQuery.value(0).toString();
        } else {
            qDebug() << "Error retrieving specialite:" << sqlQuery.lastError().text();
        }

        return result;
    }
    QVariantList GetEtudiantsSameFiliere(const QString& specialite) {
        QVariantList result;

        QString query = QString("SELECT * FROM Etudiant WHERE filiere = '%1'").arg(specialite);

        QSqlQuery sqlQuery;
        if (sqlQuery.exec(query)) {
            QSqlRecord record = sqlQuery.record();
            while (sqlQuery.next()) {
                QVariantMap etudiantData;
                for (int i = 0; i < record.count(); ++i) {
                    etudiantData[record.fieldName(i)] = sqlQuery.value(i);
                }
                result.append(etudiantData);
            }

            qDebug() << "Total number of Etudiants fetched:" << result.size();
            for (const QVariant& etudiant : result) {
                qDebug() << "Etudiant Data:" << etudiant.toMap();
            }
        } else {
            qDebug() << "Error fetching Etudiants with same filiere:" << sqlQuery.lastError().text();
        }

        return result;
    }
    bool ajouterEvaluation(double noteObtenue, int coursAssocie, int etudiantAssocie) {
        QSqlQuery query;

        query.prepare("INSERT INTO evaluation (noteObtenue, coursAssocier, etudiantAssocier) "
                      "VALUES (:note, :cours, :etudiant)");
        query.bindValue(":note", noteObtenue);
        query.bindValue(":cours", coursAssocie);
        query.bindValue(":etudiant", etudiantAssocie);

        if (query.exec()) {
            qDebug() << "Evaluation ajoutée avec succès!";
            return true;
        } else {
            qDebug() << "Erreur lors de l'ajout de l'évaluation:" << query.lastError().text();
            return false;
        }
    }
    double obtenirNotePourEtudiantCours(int numeroEtudiant, int numeroCours) {
        QSqlQuery query;
        query.prepare("SELECT noteObtenue FROM evaluation WHERE etudiantAssocier = :etudiant AND coursAssocier = :cours");
        query.bindValue(":etudiant", numeroEtudiant);
        query.bindValue(":cours", numeroCours);

        if (query.exec() && query.next()) {
            return query.value(0).toDouble();
        } else {
            qDebug() << "Aucune note trouvée pour cet étudiant et ce cours :" << query.lastError().text();
            return -1.0; // Retourne une valeur par défaut ou un code d'erreur spécifique
        }
    }




};

#endif // BASEDONNES_H
