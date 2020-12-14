#ifndef ENTRADAUNIDADES_H
#define ENTRADAUNIDADES_H

#include <QDialog>

namespace Ui {
class EntradaUnidades;
}

class EntradaUnidades : public QDialog
{
    Q_OBJECT

public:
    explicit EntradaUnidades(QWidget *parent = 0);
    ~EntradaUnidades();

private:
    Ui::EntradaUnidades *ui;
};

#endif // ENTRADAUNIDADES_H
