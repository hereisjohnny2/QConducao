#include "entradaunidades.h"
#include "ui_entradaunidades.h"

EntradaUnidades::EntradaUnidades(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntradaUnidades)
{
    ui->setupUi(this);
}

EntradaUnidades::~EntradaUnidades()
{
    delete ui;
}
