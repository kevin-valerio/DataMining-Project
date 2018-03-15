#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include "MainWindow.h"
#include <QPushButton>

int main(int argc, char* argv[]){

     QApplication app(argc, argv);
     MainWindow window;
     window.show();
     return app.exec();
}
