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

    QLabel *qPrenom;
    QLabel *qNom;
    QLabel *qTitre;

    QLabel *qLblToux;
    QLabel *qLblDouleur;
    QLabel *qLblFievre;

    QLabel *qLblAttribut;

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
