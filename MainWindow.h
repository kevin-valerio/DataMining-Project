#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include "charger_csv.h"


class MainWindow : public QWidget {
    Q_OBJECT

public slots :
    void setPrediction();

public :
     MainWindow();

private :
    void clearTab(QStringList &tab);
    float getFrequence(string maladie);
    float getFrequence(string maladie, string symptome, int col);
    float getConfiance(string maladie, string symptome, int colonne);
    vector<string> getMaladies();
    string getFinalMaladie();



    QLabel *qPrenom;
    QLabel *qNom;
    QLabel *qTitre;

    QLabel *qLblToux;
    QLabel *qLblDouleur;
    QLabel *qLblFievre;

    QLabel *qLblAttribut;
    QLabel *qResult;
    QPushButton *qPredire;

    QLineEdit *qLinePrenom;
    QLineEdit *qLineNom;

    QComboBox *qComboFievre;
    QComboBox *qComboToux;
    QComboBox *qComboDouleur;

    CVString m_vet;
    CMatString m_mat;

    QTableWidget *qTable;

};
