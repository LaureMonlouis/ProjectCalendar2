#ifndef FENTACHESCOMPO_H
#define FENTACHESCOMPO_H

#include <QWidget>

namespace Ui {
class fenTachesCompo;
}

class fenTachesCompo : public QWidget
{
    Q_OBJECT

public:
    explicit fenTachesCompo(QWidget *parent = 0);
    ~fenTachesCompo();

private:
    Ui::fenTachesCompo *ui;
};

#endif // FENTACHESCOMPO_H
