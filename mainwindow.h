#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "basedonnes.h"
#include "Etudiant.h"
#include "Cours.h"
#include "Enseignant.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    GestionBaseDonnees dbManager;
    Etudiant etudiant;
    Enseignant enseignant;
    Cours lesCours[3];

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_modifierDonnerEtud_clicked();

    void on_annulerModifEtud_clicked();

    void on_sauvegarderDonneeEtud_clicked();

    void on_Authentifier_Enseignant_clicked();

    void on_ActualiserTableEtuds_clicked();

    void on_envoyer_resultat_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
