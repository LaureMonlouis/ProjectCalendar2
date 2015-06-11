#include "fentachesprecedentes.h"
#include "ui_fentachesprecedentes.h"

fenTachesPrecedentes::fenTachesPrecedentes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenTachesPrecedentes)
{
    ui->setupUi(this);
}

fenTachesPrecedentes::~fenTachesPrecedentes()
{
    delete ui;
}
