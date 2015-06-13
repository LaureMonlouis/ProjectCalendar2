#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->activateWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//QTableView Qv (QWidget* parent = 0);
/*

QFile file(":/default.txt");
file.open(QIODevice::ReadOnly);
TreeModel model(file.readAll());
file.close();

QTreeView view;
view.setModel(&model);

QStringList entetes;
entete << "titre" << "message" << etc;

ton_QTableView->setHorizontalHeaderLabels(entetes);*/
