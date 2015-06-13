#include "fenajoutereve.h"
#include "ui_fenajoutereve.h"

fenAjouterEve::fenAjouterEve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenAjouterEve)
{
    ui->setupUi(this);   
}

fenAjouterEve::~fenAjouterEve()
{
    delete ui;
}
