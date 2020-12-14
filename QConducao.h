#ifndef QCONDUCAO_H
#define QCONDUCAO_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QInputDialog>
#include <QDir>

#include "CCondutividade.hpp"
#include "CFluxoCalor.hpp"

namespace Ui {
class QConducao;
}

class QConducao : public QMainWindow
{
    Q_OBJECT

public:
    explicit QConducao(QWidget *parent = 0);
    ~QConducao();

private:
    Ui::QConducao *ui;
    QString caminhoDados = "";
    std::unique_ptr<CCondutividade> dadosConducao;
    CFluxoCalor fluxo;

private:
    /// Carrega as informações dos materiais contidas em um diretório, atualiza a lista da combobox de materiai, preenche a tabela e atualiza o grafico
    void CarregarDados(QString _caminhoDados);
    void AtualizaGraficoGeral();

private slots:
    /// Aciona d função replot do widget QCP
    void AtualizaGrafico(QString material);
    /// Realiza o calculo do fluxo de calor com base nas informaçõe sdadas pelo usuário.
    void CalcularFluxoCalor();
    /// Preenche a tabela de dados a partir de um arquivo
    void PreencherTabela(QString nomeArquivo);
    /// Limpa todos os campos de entrada de dados
    void LimparCampos();
    /// Define um novo local onde estão os dados dos materiais
    void SetCaminhoDados();
    /// Salva a apenas a tabela em um arquivo de texto em um diretório escolhido pelo usuário
    void SalvarTabela(QString nomeArquivo = "");
    /// Salva em um arquivo de texto as equaçãoes de cada uma das retas em um diretório escolhido pelo usuário
    void SalvarEquacoes();
    /// Salva o gráfico em formato png em um diretório escolhido pelo usuário
    void SalvarGrafico();
    /// Cria um novo arquivo com base nos dado inseridos na tabela, atualiza o diretório com os materiais e replota o grafico
    void GerarCurva();
    /// Adiciona um novo material ao diretorio de dados
    void CriarNovoMaterial();
};

#endif // QCONDUCAO_H
