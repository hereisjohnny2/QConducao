#ifndef CCONDUTIVIDADE_H
#define CCONDUTIVIDADE_H

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

#include "CInterpolacao.hpp"

class CCondutividade
{
private:
    std::map<std::string, std::shared_ptr<CInterpolacao>> data; ///> Conjunto de Dados entre os Materiais e as Curvas de Interpolação

public:
    /// Construtor Padrão
    CCondutividade() = default;
    /// Construtor sobrecarregado para a inclusão de dados a partir de um arquivo do disco.
    CCondutividade(const std::string &_nomeCaminho);
    /// Construtor de Cópia
    CCondutividade(CCondutividade &obj) : data(obj.data) {}
    /// Destrutor Padrão
    ~CCondutividade() = default;

    /// Sobrecarga do operador de atribuição
    CCondutividade &operator =(const CCondutividade& obj) = default;

    /// Getter e Setter do mapa com os dados
    std::map<std::string, std::shared_ptr<CInterpolacao>> inline Data() const { return data; }
    void inline Data(std::map<std::string, std::shared_ptr<CInterpolacao>> _data) { data = _data; }

    /// Adiciona uma material solicitando o nome e os dados de temperatura e condutividade
    void AdicionaMaterial(std::ostream &os = std::cout, std::istream &in = std::cin);

    /// Adiciona um novo material ao map solicitando apenas o nome. Um objeto interpolacao vazio é atribuido a ele para depois ser preenchido com dos dados de temperatura e condutividade.
    void AdicionaMaterial(const std::string &nomeMaterial, const std::string &nomeCaminho);

    /// Limpa o mapa e le novamente o diretório com os arquivos dos materiais
    void AtualizaListaMaterial(const std::string &nomeCaminho, const std::string &extensao = ".dat");

    /// Sobrecarrega o operador de acesso [] para retornar a csua curva correspondente.
    std::shared_ptr<CInterpolacao> inline operator[](const std::string &nomeMaterial) { return data[nomeMaterial]; }

    void Plot(QCustomPlot *grafico);
};

#endif // !CCONDUTIVIDADE_H
