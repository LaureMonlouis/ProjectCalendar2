#ifndef FENTACHESPRECEDENTES_H
#define FENTACHESPRECEDENTES_H

#include <QWidget>

namespace Ui {
class fenTachesPrecedentes;
}

class fenTachesPrecedentes : public QWidget
{
    Q_OBJECT

public:
    explicit fenTachesPrecedentes(QWidget *parent = 0);
    ~fenTachesPrecedentes();

private:
    Ui::fenTachesPrecedentes *ui;
};

#endif // FENTACHESPRECEDENTES_H
