#include "MainWindow.h"
#include <algorithm>
#include <map>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QString>
#include <vector>
#include "charger_csv.h"

using namespace std;
MainWindow::MainWindow() : QWidget()
{

    setFixedSize(800,600);
    read_csv(m_mat, m_vet, "data.csv");

    qTitre = new QLabel("CLASSIFICATION DES PATIENTS - PREDICTION", this);
    qTitre->setFont(QFont("Arial", 15, QFont::Bold, true));
    qTitre->move(200, 20);

    qNom = new QLabel("Nom", this);
    qNom->setFont(QFont("Arial", 12, NULL, true));
    qNom->move(500, 100);

    qPrenom = new QLabel("Prenom", this);
    qPrenom->setFont(QFont("Arial", 12, NULL, true));
    qPrenom->move(500, 130);

    qLinePrenom = new QLineEdit(this);;
    qLinePrenom->move(570,125);
    qLinePrenom->resize(165, qLinePrenom->height());

    qLineNom = new QLineEdit(this);;
    qLineNom->move(570,90);
    qLineNom->resize(165, qLineNom->height());


    ///////////// POUR LA FIEVRE
    QStringList tab;
    qComboFievre = new QComboBox(this);
    qComboFievre->setGeometry(180,210,100,30);
    qComboFievre->addItem("NULL");
    for(unsigned i(1); i < m_mat.size() ; ++i){
        tab.append(QString::fromStdString(m_mat[i][0]));
    }
    tab.removeDuplicates();
    qComboFievre->addItems(tab);
    tab.clear();

    ///////////// POUR LA DOULEUR

    qComboDouleur = new QComboBox(this);
    qComboDouleur->setGeometry(305,210,100,30);
    qComboDouleur->addItem("NULL");
    for(unsigned i(1); i < m_mat.size() ; ++i){
        tab.append(QString::fromStdString(m_mat[i][1]));
    }
    tab.removeDuplicates();
    qComboDouleur->addItems(tab);
    tab.clear();


    ///////////// POUR LA TOUX

    qComboToux = new QComboBox(this);
    qComboToux->setGeometry(420,210,100,30);
    qComboToux->addItem("NULL");
    for(unsigned i(1); i < m_mat.size() ; ++i){
        tab.append(QString::fromStdString(m_mat[i][2]));
    }
    tab.removeDuplicates();
    qComboToux->addItems(tab);
    tab.clear();

    qLblToux = new QLabel("Toux", this);
    qLblToux->setFont(QFont("Arial", 12, NULL, true));
    qLblToux->move(420, 185);

    qLblFievre = new QLabel("Fievre", this);
    qLblFievre->setFont(QFont("Arial", 12, NULL, true));
    qLblFievre->move(180, 185);



    qLblDouleur = new QLabel("Douleur", this);
    qLblDouleur->setFont(QFont("Arial", 12, NULL, true));
    qLblDouleur->move(305, 185);

    qLblAttribut = new QLabel("Les valeurs des attributs", this);
    qLblAttribut->setFont(QFont("Arial", 12, NULL, true));
    qLblAttribut->move(1, 220);

    qPredire = new QPushButton("Predire", this);
    qPredire->setGeometry(50,280,80,40);

    qTable = new QTableWidget(9, 4, this);

    QStringList horinzontalLbl, verticalLbl;

    for(unsigned i(0); i < m_vet.size(); ++i)
        horinzontalLbl.append(QString::fromStdString(m_vet[i]));

    for(unsigned i(0); i < m_mat.size(); ++i)
        verticalLbl.append("Patient " + QString::number(i));

    for(unsigned i(0); i < m_vet.size() ; ++i){
        for(unsigned a(0); a < m_mat.size() ; ++a)
            qTable->setItem(a,i, new QTableWidgetItem(QString::fromStdString(m_mat[a][i])));
    }


    qTable->setHorizontalHeaderLabels(horinzontalLbl);
    qTable->setVerticalHeaderLabels(verticalLbl);

    qTable->move(1,350);
    qTable->resize(800, 250);

    qResult = new QLabel("Aucune maladie", this);
    qResult->setFont(QFont("Arial", 12, NULL, true));
    qResult->setGeometry(190,280,420,40);

    vector<string> c = getMaladies();


    connect(qPredire, SIGNAL(clicked()), this, SLOT(setPrediction()));
}


vector<string> MainWindow::getMaladies(){
    vector<string> maladies;
    for(unsigned i(1); i < m_mat.size() ; ++i)
        maladies.push_back(m_mat[i][3]);

    sort(maladies.begin(), maladies.end());
    maladies.erase(unique(maladies.begin(), maladies.end()), maladies.end());

    return maladies;
}



float MainWindow::getFrequence(string maladie){
    float frequence;
    for(unsigned i(0); i < m_mat.size(); ++i)
        if(m_mat[i][3] == maladie)
            ++frequence;

    return (frequence / m_mat.size());
}

float MainWindow::getFrequence(string maladie, string symptome, int col){
    float frequence;
    for(unsigned i(0); i < m_mat.size(); ++i)
        if(m_mat[i][col] == symptome && m_mat[i][3] == maladie)
            ++frequence;
    return (frequence / m_mat.size());
}

float MainWindow::getConfiance(string maladie, string symptome, int colonne){
    return getFrequence(maladie, symptome, colonne) / getFrequence(maladie);
}

string MainWindow::getFinalMaladie() {


    string txtFievre =  qComboFievre->currentText().toStdString();
    string txtDouleur =  qComboDouleur->currentText().toStdString();
    string txtToux =  qComboToux->currentText().toStdString();

    map<string, float> maladiesEtScore;
    map<string, float>::iterator it;

    float score = 0;
    int nombreScoresEgaux = 0;
    string scoreFinal;
    int a = 0;

    for(unsigned i(0); i < getMaladies().size(); ++i){
        string maladieParcourue = getMaladies()[i];
        maladiesEtScore.insert(make_pair(maladieParcourue, 1));
    }

    if(txtToux == "NULL" && txtDouleur == "NULL" && txtFievre == "NULL"){

         return "Tout est NULL. Cela ne devrait pas etre le cas...";
    }

    else{

        if(txtFievre != "NULL"){
            map<string, float>::iterator it = maladiesEtScore.begin();
            for (;it != maladiesEtScore.end(); ++it)
                it->second *= getConfiance(it->first, txtFievre, 0);
        }

        if(txtDouleur != "NULL"){
            map<string, float>::iterator it = maladiesEtScore.begin();
            for (;it != maladiesEtScore.end(); ++it)
                it->second *= getConfiance(it->first, txtDouleur, 1);
        }

        if(txtToux != "NULL"){
            map<string, float>::iterator it = maladiesEtScore.begin();
            for (;it != maladiesEtScore.end(); ++it)
                it->second *= getConfiance(it->first, txtToux, 2);
        }

        for (it = maladiesEtScore.begin(); it != maladiesEtScore.end(); ++it ){
            if(a==0) score = it->second;
            if(it->second > score) score = it->second;
            ++a;
        }

    }

    for (it = maladiesEtScore.begin(); it != maladiesEtScore.end(); ++it){
        if (it->second == score){
            scoreFinal = it->first;
            ++nombreScoresEgaux;
        }
    }

    if((int) score == 0) return "Le score est de 0. Impossible de determiner la maladie";
    if(nombreScoresEgaux > 1) return "Trop de scores egaux !";
    return scoreFinal;

}

void MainWindow::setPrediction(){

    qResult->setText(QString::fromStdString(getFinalMaladie()));

}
