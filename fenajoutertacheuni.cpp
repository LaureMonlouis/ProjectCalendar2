#include "fenajoutertacheuni.h"
#include "ui_fenajoutertacheuni.h"

fenAjouterTacheUni::fenAjouterTacheUni(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenAjouterTacheUni)
{
    ui->setupUi(this);
}

fenAjouterTacheUni::~fenAjouterTacheUni()
{
    delete ui;
}
