#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "gestionprojet.h"
#include <QCoreApplication>

#include <QMainWindow>
#include <QTableView>

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

#endif // MAINWINDOW_H
