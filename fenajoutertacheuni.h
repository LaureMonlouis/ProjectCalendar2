#ifndef FENAJOUTERTACHEUNI_H
#define FENAJOUTERTACHEUNI_H

#include <QWidget>

namespace Ui {
class fenAjouterTacheUni;
}

class fenAjouterTacheUni : public QWidget
{
    Q_OBJECT

public:
    explicit fenAjouterTacheUni(QWidget *parent = 0);
    ~fenAjouterTacheUni();

private:
    Ui::fenAjouterTacheUni *ui;
};

#endif // FENAJOUTERTACHEUNI_H
