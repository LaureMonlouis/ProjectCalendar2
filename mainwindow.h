#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "gestionprojet.h"
#include <QCoreApplication>
#include <QMainWindow>
#include <QTableView>
#include "treemodel.h"
#include <QApplication>
#include <QFile>
#include <QTreeView>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
  private:
    Ui::MainWindow *ui;
};

QTableView Qv (QWidget* parent = 0);
/*

QFile file(":/default.txt");
file.open(QIODevice::ReadOnly);
TreeModel model(file.readAll());
file.close();

QTreeView view;
view.setModel(&model);*/

QStringList entetes;
entete << "titre" << "message" << etc;

ton_QTableView->setHorizontalHeaderLabels(entetes);

#endif // MAINWINDOW_H
