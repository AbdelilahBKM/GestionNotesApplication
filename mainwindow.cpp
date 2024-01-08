#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "qmessagebox.h"
#include <QSqlRecord>
#include <QVariantMap>
#include <QItemDelegate>
#include <QLineEdit>
#include <QDoubleValidator>

class CustomDelegate : public QItemDelegate {
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (index.column() >= 2 && index.column() <= 4) { // Columns 3, 4, and 5 (index starts from 0)
            QLineEdit *editor = new QLineEdit(parent);
            QDoubleValidator *validator = new QDoubleValidator(0, 20, 2, editor);
            editor->setValidator(validator);
            return editor;
        }
        return QItemDelegate::createEditor(parent, option, index);
    }
};



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox_donneesEtud->hide();
    ui->Affiche_resultat->hide();
    ui->groupBox_tableNote->hide();
    ui->tableDonneeEtud->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableResultat->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->sauvegarderDonneeEtud->hide();
    ui->annulerModifEtud->hide();
    CustomDelegate *delegate = new CustomDelegate();
    ui->tableNoteEtudiants->setItemDelegate(delegate);
}
MainWindow::~MainWindow()
{
    delete ui;
}

// ************************************** Page etudiant: *************************************************************//
void MainWindow::on_pushButton_clicked()
{
    QString nom = ui->lineEdit->text();
    QString prenom  = ui->lineEdit_2->text();
    QString filiere = ui->comboBox->currentText();
    QDate date_naissance = ui->dateEdit->date();
    if(nom.isEmpty() &&  prenom.isEmpty()){
        QMessageBox::warning(this, "attention", "veuillez remplir tous les champ");
    }else {
        etudiant.setNomEtudiant(nom);
        etudiant.setPrenomEtudiant(prenom);
        etudiant.setDateNaissanceEtudiant(date_naissance.toString("yyyy/MM/dd"));
        etudiant.setSpecialite(filiere);
        if(etudiant.verifierEtudiantExist(dbManager)){
            ui->groupBox_donneesEtud->show();
            ui->Affiche_resultat->show();
            ui->tableDonneeEtud->setItem(0, 0,  new QTableWidgetItem(QString::number(etudiant.getNumeroEtudiant())));
            ui->tableDonneeEtud->setItem(1, 0,  new QTableWidgetItem(etudiant.getNomEtudiant()));
            ui->tableDonneeEtud->setItem(2, 0,  new QTableWidgetItem(etudiant.getPrenomEtudiant()));
            ui->tableDonneeEtud->setItem(3, 0,  new QTableWidgetItem(etudiant.getDateNaissanceEtudiant()));
            ui->tableDonneeEtud->setItem(4, 0,  new QTableWidgetItem(etudiant.getSpecialite()));
        } else {
               if(etudiant.ajouterEtudiant(dbManager)){
                        int numero = etudiant.getNumeroEtudiant(dbManager);
                        ui->groupBox_donneesEtud->show();
                        ui->Affiche_resultat->show();
                        ui->tableDonneeEtud->setItem(0, 0,  new QTableWidgetItem(QString::number(numero)));
                        ui->tableDonneeEtud->setItem(1, 0,  new QTableWidgetItem(etudiant.getNomEtudiant()));
                        ui->tableDonneeEtud->setItem(2, 0,  new QTableWidgetItem(etudiant.getPrenomEtudiant()));
                        ui->tableDonneeEtud->setItem(3, 0,  new QTableWidgetItem(etudiant.getDateNaissanceEtudiant()));
                        ui->tableDonneeEtud->setItem(4, 0,  new QTableWidgetItem(etudiant.getSpecialite()));
               }else {
                   QMessageBox::warning(this, "Erreur", "Une erreur s'est produite. Veuillez réessayer");
                   return;
                }
        }
        QVariantList courses =  Cours::getCoursesByFiliere(dbManager ,etudiant.getSpecialite());
        int indexTable = 4;
        int resultTable = 0;
        for (const QVariant& cours : courses) {
            QVariantMap coursData = cours.toMap();
            int numeroCours = coursData["numeroCours"].toInt();
            QString titulaire = coursData["titulaire"].toString();
            QString filiere = coursData["filiere"].toString();
            double noteCours = dbManager.obtenirNotePourEtudiantCours(etudiant.getNumeroEtudiant(), numeroCours);
            ui->tableDonneeEtud->setItem(++indexTable, 0, new QTableWidgetItem(titulaire));
            ui->tableResultat->setItem(0, resultTable, new QTableWidgetItem(titulaire));
            if(noteCours != -1.0){
                ui->tableResultat->setItem(1, resultTable,  new QTableWidgetItem(QString::number(noteCours)));
            }
            resultTable++;
            qDebug() << "Cours numéro:" << numeroCours << ", Titulaire:" << titulaire << ", Filière:" << filiere;
        }
    }
}


void MainWindow::on_modifierDonnerEtud_clicked()
{
    // Disable editing for the entire table initially
    ui->tableDonneeEtud->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->sauvegarderDonneeEtud->show();
    ui->annulerModifEtud->show();

    // Define the target column
    int targetColumn = 0;  // Column index

    // Enable editing for specific rows (2, 3, and 4) and disable for others in that column
    for (int row = 0; row < ui->tableDonneeEtud->rowCount(); ++row) {
        QTableWidgetItem *item = ui->tableDonneeEtud->item(row, targetColumn);
        if (!item) {
               item = new QTableWidgetItem();
               ui->tableDonneeEtud->setItem(row, targetColumn, item);
        }

        if (row == 1 || row == 2 || row == 3) {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        } else {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }

    // Set edit triggers to allow specific actions
    ui->tableDonneeEtud->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
}

void MainWindow::on_sauvegarderDonneeEtud_clicked()
{
    QString newNom = ui->tableDonneeEtud->item(1,0)->text();
    QString newPrenom = ui->tableDonneeEtud->item(2, 0)->text();
    QString newDateNaissance = ui->tableDonneeEtud->item(3, 0)->text();
    if(newNom.isEmpty() || newPrenom.isEmpty() || newDateNaissance.isEmpty()){
        QMessageBox::warning(this, "attention", "veuillez remplir tous les champ!");
    }else{
        QDate date = QDate::fromString(newDateNaissance, "yyyy/MM/dd");
        if(!date.isValid()){
                QMessageBox::warning(this, "attention", "veuillez saisire une date valide!");
        }else{
                // modifier etudiant;
            if(!etudiant.modifierEtudiant(dbManager, newNom, newPrenom, newDateNaissance)){
                    QMessageBox::warning(this, "Erreur", "Une erreur s'est produite. Veuillez réessayer");
                }
                ui->sauvegarderDonneeEtud->hide();
                ui->annulerModifEtud->hide();
                ui->tableDonneeEtud->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }
    }
}


void MainWindow::on_annulerModifEtud_clicked()
{
        ui->tableDonneeEtud->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableDonneeEtud->setItem(0, 0,  new QTableWidgetItem("1"));
        ui->tableDonneeEtud->setItem(1, 0,  new QTableWidgetItem(etudiant.getNomEtudiant()));
        ui->tableDonneeEtud->setItem(2, 0,  new QTableWidgetItem(etudiant.getPrenomEtudiant()));
        ui->tableDonneeEtud->setItem(3, 0,  new QTableWidgetItem(etudiant.getDateNaissanceEtudiant()));
        ui->tableDonneeEtud->setItem(4, 0,  new QTableWidgetItem(etudiant.getSpecialite()));
        ui->sauvegarderDonneeEtud->hide();
        ui->annulerModifEtud->hide();
}


// ************************************** Page Enseignant: *************************************************************//

// numero des cours: d'appres la specialite de l'enseignant:


void MainWindow::on_Authentifier_Enseignant_clicked()
{
    QString nom = ui->nomEnseignant->text();
    QString prenom = ui->prenomEnseignant->text();
    int numero = ui->numeroEnseignant->text().toInt();
    if(nom.isEmpty() || prenom.isEmpty() || !numero){
        QMessageBox::warning(this, "attention", "veuillez remplir tous les champ");
    }else {
        if(!enseignant.authentifierEnseignant(dbManager, nom, prenom, numero)){
            QMessageBox::warning(this, "Warning", "accès refusé!");
        } else {
            ui->groupBox_authentifierEnseignant->hide();
            ui->groupBox_tableNote->show();
            int headerIndex = 1;
            int coursIndex = 0;
            QVariantList courses =  Cours::getCoursesByFiliere(dbManager ,enseignant.getSpecialiteEnseignant());
            for (const QVariant& cours : courses) {
                QVariantMap coursData = cours.toMap();
                int numeroCours = coursData["numeroCours"].toInt();
                QString titulaire = coursData["titulaire"].toString();
                QString filiere = coursData["filiere"].toString();
                ui->tableNoteEtudiants->setHorizontalHeaderItem(++headerIndex, new QTableWidgetItem(titulaire));
                lesCours[coursIndex].setNumeroCours(numeroCours);
                lesCours[coursIndex].setTitulaire(titulaire);
                lesCours[coursIndex].setFiliere(filiere);

                qDebug() << "Cours numéro:" << lesCours[coursIndex].getNumeroCours() << ", Titulaire:" << lesCours[coursIndex].getTitulaire() << ", Filière:" <<
                    lesCours[coursIndex].getFiliere();
                coursIndex++;
            }
            QVariantList etudiantsSameFiliere = dbManager.GetEtudiantsSameFiliere(enseignant.getSpecialiteEnseignant());
            int rowCount = etudiantsSameFiliere.size();
            ui->tableNoteEtudiants->setRowCount(rowCount); // Set the correct number of rows

            for (int i = 0; i < rowCount; ++i) {
                QVariantMap etudiantData = etudiantsSameFiliere[i].toMap();
                QString numeroEtudiant = etudiantData["numero"].toString();
                QString nomEtudiant = etudiantData["nomEtudiant"].toString();
                QTableWidgetItem *item = new QTableWidgetItem(numeroEtudiant);
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                QTableWidgetItem *item2 = new QTableWidgetItem(nomEtudiant);
                item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);

                ui->tableNoteEtudiants->setItem(i, 0, item);
                ui->tableNoteEtudiants->setItem(i, 1, item2);
            }

        }
    }
}


void MainWindow::on_ActualiserTableEtuds_clicked() {
    int rows = ui->tableNoteEtudiants->rowCount();

    for (int i = 0; i < rows; ++i) {
        int numeroEtudiant = ui->tableNoteEtudiants->item(i, 0)->text().toInt();
        int numeroCours1 = lesCours[0].getNumeroCours();
        int numeroCours2 = lesCours[1].getNumeroCours();
        int numeroCours3 = lesCours[2].getNumeroCours();

        double getnoteCours1 = dbManager.obtenirNotePourEtudiantCours(numeroEtudiant, numeroCours1);
        double getnoteCours2 = dbManager.obtenirNotePourEtudiantCours(numeroEtudiant, numeroCours2);
        double getnoteCours3 = dbManager.obtenirNotePourEtudiantCours(numeroEtudiant, numeroCours3);

        if (getnoteCours1 != -1.0 && getnoteCours2 != -1.0 && getnoteCours3 != -1.0) {
            QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(getnoteCours1));
            QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(getnoteCours2));
            QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(getnoteCours3));

            ui->tableNoteEtudiants->setItem(i, 2, item1);
            ui->tableNoteEtudiants->setItem(i, 3, item2);
            ui->tableNoteEtudiants->setItem(i, 4, item3);
        }
    }
}



void MainWindow::on_envoyer_resultat_clicked()
{
    int rows = ui->tableNoteEtudiants->rowCount();

    for (int i = 0; i < rows; ++i) {
        // Récupérer les informations nécessaires depuis votre tableau
        QString noteMatiere1 = ui->tableNoteEtudiants->item(i, 2)->text(); // Colonne 3 pour la première matière
        QString noteMatiere2 = ui->tableNoteEtudiants->item(i, 3)->text(); // Colonne 4 pour la deuxième matière
        QString noteMatiere3 = ui->tableNoteEtudiants->item(i, 4)->text(); // Colonne 5 pour la troisième matière
        if(noteMatiere1.isEmpty() && noteMatiere2.isEmpty() && noteMatiere3.isEmpty()){
             QMessageBox::warning(this, "attention", "veuillez saisir les notes pour tous les étudiants !");
             return;
        }
        // Récupérer les autres informations nécessaires, par exemple, le numéro d'étudiant et le numéro de cours
        int numeroEtudiant = ui->tableNoteEtudiants->item(i, 0)->text().toInt();
        int numeroCours1 = lesCours[0].getNumeroCours();
        int numeroCours2 = lesCours[1].getNumeroCours();
        int numeroCours3 = lesCours[2].getNumeroCours();

        qDebug() << "numeroCours1: " << numeroCours1;
        qDebug() << "numeroCours2: " << numeroCours2;
        qDebug() << "numeroCours3: " << numeroCours3;

        // Ajouter chaque évaluation à la base de données
        bool success = true;
        if (!dbManager.ajouterEvaluation(noteMatiere1.toDouble(), numeroCours1, numeroEtudiant)) {
            success = false;
        }
        if (!dbManager.ajouterEvaluation(noteMatiere2.toDouble(), numeroCours2, numeroEtudiant)) {
            success = false;
        }
        if (!dbManager.ajouterEvaluation(noteMatiere3.toDouble(), numeroCours3, numeroEtudiant)) {
            success = false;
        }

        if (success) {
            qDebug() << "Évaluations ajoutées avec succès pour l'étudiant " << numeroEtudiant;
        } else {
            qDebug() << "Erreur lors de l'ajout des évaluations pour l'étudiant " << numeroEtudiant;
        }
    }
    QMessageBox::information(this, "Succès", "Opération effectuée avec succès!");
}

