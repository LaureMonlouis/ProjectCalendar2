#include "fentachescompo.h"
#include "ui_fentachescompo.h"

fenTachesCompo::fenTachesCompo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenTachesCompo)
{
    ui->setupUi(this);
}

fenTachesCompo::~fenTachesCompo()
{
    delete ui;
}
