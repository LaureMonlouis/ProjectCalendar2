#ifndef FENAJOUTEREVE_H
#define FENAJOUTEREVE_H

#include <QWidget>

namespace Ui {
    class fenAjouterEve;
}

class fenAjouterEve : public QWidget
{
    Q_OBJECT

  public:
    explicit fenAjouterEve(QWidget *parent = 0);
    ~fenAjouterEve();
  private:
    Ui::fenAjouterEve *ui;
};

#endif // FENAJOUTEREVE_H
