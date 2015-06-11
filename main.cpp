#include "gestionprojet.h"
#include "agenda.h"
#include "mainwindow.h"
#include "fenajoutereve.h"
#include <QCoreApplication>
//#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTime d1=QTime(3,0);
    QDate date1= QDate(1925,7,3);
    QDate date2= QDate (1925,11,3);
    TacheUnitaire* tU1 = new TacheUnitaire("112","Assembler la charpente",d1,date1,date2,false);
    TacheComposite* tc1 = new TacheComposite("lid","manger");
    Projet* p = new Projet("0","Construire une maison","C'est pas très dur");
    TacheManager::getInstance().addItem(p);
    TacheManager::getInstance().getTache("0").ajouterTacheUnitaire("1","Faire les courses chez castorama",d1,date1,date2,false);
    TacheManager::getInstance().getTache("0").ajouterTacheComposite("2","Assembler les morceaux");
    TacheManager::getInstance().getTache("1").ajouterTacheUnitaire("12","Assembler la charpente",d1,date1,date2,false);
    TacheManager::getInstance().getTache("1").ajouterTacheComposite("11","Assembler les briques");
    TacheManager::getInstance().getTache("11").ajouterTacheUnitaire("111","Assembler la charpente",d1,date1,date2,false);
    TacheManager::getInstance().getTache("11").ajouterTacheUnitaire("112","Assembler la charpente",d1,date1,date2,false);
    VisiteurSauvegarde* vS = new VisiteurSauvegarde ("Test1");
    //std::cout<<"Sauvegarde commence";
    vS->visiterProjet(p);
    //MainWindow w;
    //w.show();
    return a.exec();
}
