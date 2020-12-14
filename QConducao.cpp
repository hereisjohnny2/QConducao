#include "QConducao.h"
#include "ui_qconducao.h"

#include <QDebug>

QConducao::QConducao(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QConducao)
{
    ui->setupUi(this);
    SetCaminhoDados();

    connect(ui->BtnCalc, SIGNAL(clicked(bool)), this, SLOT(CalcularFluxoCalor()));

    int numLinhas = ui->TabelaDados->rowCount();
    connect(ui->BtnAddPonto, &QPushButton::clicked, [=](){ui->TabelaDados->insertRow(ui->TabelaDados->rowCount());});
    connect(ui->BtnRemovePonto, &QPushButton::clicked, [=](){ui->TabelaDados->removeRow(numLinhas - 1);});

    connect(ui->BtnLimpa, SIGNAL(clicked(bool)), this, SLOT(LimparCampos()));
    connect(ui->BtnCriaCurva, SIGNAL(clicked(bool)), this, SLOT(GerarCurva()));

    connect(ui->MenuNovoMaterial, SIGNAL(triggered(bool)), this, SLOT(CriarNovoMaterial()));
    connect(ui->MenuCarregar, SIGNAL(triggered(bool)), this, SLOT(SetCaminhoDados()));
    connect(ui->MenuSalvarTabela, SIGNAL(triggered(bool)), this, SLOT(SalvarTabela()));
    connect(ui->MenuSalvarEquacoes, SIGNAL(triggered(bool)), this, SLOT(SalvarEquacoes()));

    connect(ui->MenuAtivaLegenda, &QAction::toggled, [this](bool estado){ui->Plot->legend->setVisible(estado); ui->Plot->replot();});
    connect(ui->MenuAtivaLegendaGeral, &QAction::toggled, [this](bool estado){ui->PlotGeral->legend->setVisible(estado); ui->PlotGeral->replot();});
    connect(ui->MenuSalvarGrafico, SIGNAL(triggered(bool)), this, SLOT(SalvarGrafico()));
    connect(ui->MenuSair, &QAction::triggered, [this](){QApplication::quit();});
}

QConducao::~QConducao()
{
    delete ui;
}

void QConducao::AtualizaGrafico(QString material)
{
    ui->Plot->legend->setVisible(false);
    ui->Plot->legend->clearItems();
    ui->Plot->clearGraphs();

    ui->Plot->xAxis->setLabel("Temperatura (°C)");
    ui->Plot->yAxis->setLabel("Condutividade (W/m².C°)");

    (*dadosConducao)[material.toStdString()]->Plot(ui->Plot);

    ui->Plot->xAxis->rescale(true);
    ui->Plot->yAxis->rescale(true);
    ui->Plot->replot();
}

void QConducao::AtualizaGraficoGeral()
{
    ui->PlotGeral->legend->setVisible(false);
    ui->PlotGeral->legend->clearItems();
    ui->PlotGeral->clearGraphs();

    ui->PlotGeral->xAxis->setLabel("Temperatura (°C)");
    ui->PlotGeral->yAxis->setLabel("Condutividade (W/m².C°");

    dadosConducao->Plot(ui->PlotGeral);
    ui->PlotGeral->xAxis->rescale(true);
    ui->PlotGeral->yAxis->rescale(true);
    ui->PlotGeral->replot();
}

void QConducao::CalcularFluxoCalor()
{
    if (ui->BoxMaterial->count() <= 0 or ui->TabelaDados->rowCount() == 0)
    {
        QMessageBox::warning(this, "Warning", "Não exitem materiais para realizar os calculos");
        return;
    }

    double temp = ui->InputTemp->text().toDouble();
    QString inputMaterial = ui->BoxMaterial->currentText();

    double dx = ui->InputDx->text().toDouble();
    double dt = ui->InputDt->text().toDouble();
    double A = ui->InputArea->text().toDouble();

    double k = (*dadosConducao)[inputMaterial.toStdString()]->Fx(temp + dt / 2.0);

    double fluxoCalor = fluxo(dx, dt, A, k);

    ui->InputCondut->setText(QString::number(k));
    ui->InputFlux->setText(QString::number(fluxoCalor));
}

void QConducao::PreencherTabela(QString nomeArquivo)
{
    QFile arquivo("data/" + nomeArquivo + ".dat");
    if (!arquivo.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Ops, algo deu errado ao abrir o arquivo" + arquivo.errorString());
        return;
    }

    QTextStream in(&arquivo);
    QStringList data = in.readAll().split(QRegExp("\\n"));
    data.pop_back();

    ui->TabelaDados->setRowCount(data.length());

    QStringList elem;
    QTableWidgetItem *pitem;
    for (int i = 0; i < data.length(); i++)
    {
        elem = data.at(i).split(QRegExp("\\s+"));
        for (int j = 0; j < ui->TabelaDados->columnCount(); j++)
        {
            pitem = new QTableWidgetItem();
            pitem->setText(elem.at(j));
            ui->TabelaDados->setItem(i, j, pitem);
        }
    }

    arquivo.close();
}

void QConducao::LimparCampos()
{
    ui->InputCondut->clear();
    ui->InputDt->setText("1");
    ui->InputDx->setText("1");
    ui->InputArea->setText("1");
    ui->InputFlux->clear();
    ui->InputTemp->clear();
}

void QConducao::SetCaminhoDados()
{
    caminhoDados = QFileDialog::getExistingDirectory(this, "Diretório com Arquivos");

    if(!caminhoDados.isEmpty())
        CarregarDados(caminhoDados);
}

void QConducao::CarregarDados(QString _caminhoDados)
{
    caminhoDados = _caminhoDados;

    disconnect(ui->BoxMaterial, SIGNAL(currentTextChanged(QString)), this, SLOT(AtualizaGrafico(QString)));
    disconnect(ui->BoxMaterial, SIGNAL(currentTextChanged(QString)), this, SLOT(PreencherTabela(QString)));

    ui->BoxMaterial->clear();

    dadosConducao = std::make_unique<CCondutividade>(caminhoDados.toStdString());
    for (auto &material : dadosConducao->Data())
        ui->BoxMaterial->addItem(QString::fromStdString(material.first));

    connect(ui->BoxMaterial, SIGNAL(currentTextChanged(QString)), this, SLOT(AtualizaGrafico(QString)));
    connect(ui->BoxMaterial, SIGNAL(currentTextChanged(QString)), this, SLOT(PreencherTabela(QString)));

    PreencherTabela(ui->BoxMaterial->currentText());
    AtualizaGrafico(ui->BoxMaterial->itemText(0));
    AtualizaGraficoGeral();
}

void QConducao::SalvarTabela(QString nomeArquivo)
{
    if (nomeArquivo.isEmpty())
        nomeArquivo = QFileDialog::getSaveFileName(this, "Salvar Tabela de Dados", caminhoDados);

    QFile arquivo(nomeArquivo);

    if (!arquivo.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Não foi possível salvar o arquivo.");
        return;
    }

    QTextStream out(&arquivo);
    for (int i = 0; i < ui->TabelaDados->rowCount(); i++)
        out << ui->TabelaDados->item(i,0)->text() << "\t" << ui->TabelaDados->item(i, 1)->text() << "\n";

    arquivo.close();
}

void QConducao::SalvarEquacoes()
{
    QString nomeArquivo = QFileDialog::getSaveFileName(this, "Salvar Equações de Dados", caminhoDados);
    (*dadosConducao)[ui->BoxMaterial->currentText().toStdString()]->Saida(nomeArquivo.toStdString());
}

void QConducao::SalvarGrafico()
{
    QString nomeArquivo = QFileDialog::getSaveFileName(this, "Salvar Gráfico");
    QFile arquivo(nomeArquivo);

    if (!arquivo.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Não foi possível salvar o arquivo.");
        return;
    }

    ui->Plot->savePng(nomeArquivo);

    arquivo.close();
}

void QConducao::GerarCurva()
{
    if (ui->TabelaDados->rowCount() < 2)
    {
        QMessageBox::warning(this, "Warning", "Não existem dados suficientes para a realização dos cálculos!");
        return;
    }

    QString material = ui->BoxMaterial->currentText();
    if (material.isEmpty())
        SalvarTabela();
    else
        SalvarTabela(caminhoDados + "/" + material + ".dat");

    CarregarDados(caminhoDados);
    AtualizaGrafico(material);
}

void QConducao::CriarNovoMaterial()
{
    bool ok;
    QString nomeMaterial = QInputDialog::getText(this, tr("Novo Material"), tr("Nome: "), QLineEdit::Normal, QDir::home().dirName(), &ok);

    if (ok and !nomeMaterial.isEmpty())
    {
       dadosConducao->AdicionaMaterial(nomeMaterial.toStdString(), caminhoDados.toStdString());
       CarregarDados(caminhoDados);
       ui->BoxMaterial->setCurrentText(nomeMaterial);
       AtualizaGrafico(nomeMaterial);
    }
}

