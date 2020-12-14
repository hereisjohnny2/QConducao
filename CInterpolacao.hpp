#ifndef CINTERPOLACAO_H
#define CINTERPOLACAO_H

#include "CSegmentoReta.hpp"

/// Classe CInterpolacao, referente a aplicação do algoritimo de interpolacao linear sobre objetos da classe CSegmentoReta
class CInterpolacao
{
private:
    std::vector<CSegmentoReta *> retas; ///< Vetor de ponteiro para objetos de segmentos de retas.
    double xmin;                        ///< Limite inferior da interpolação (limite inferior do primeiro segmento de reta).
    double xmax;                        ///< Limite superior da interpolação (limite superior do último segmento de reta).

public:
    /// Construtor default.
    CInterpolacao() = default;
    /// Construtor de cópia.
    CInterpolacao(const CInterpolacao &interpolacao) = default;
    /// Construtor sobrecarregado, recebe o nome do arquivo no qual se encontram os pares ordenados a serem interpolados
    CInterpolacao(const std::string &nomeArquivo);
    /// Construtor sobrecarregado, recebe um vetor com objetos de segmentos de reta.
    CInterpolacao(std::vector<CSegmentoReta *> _retas) : retas(_retas), xmin(_retas[0]->Xmin()), xmax(_retas.back()->Xmax()) {}
    /// Destrutor default.
    ~CInterpolacao();

    /// Get e set do vetor de objetos de segmentos de retas.
    void inline Retas(std::vector<CSegmentoReta *> _retas) { retas = _retas; }
    std::vector<CSegmentoReta *> inline Retas() const { return retas; }

    /// Get e set do xmin.
    void inline Xmin(double _xmin) { xmin = _xmin; }
    double inline Xmin() const { return xmin; }

    /// Get e set do xmax.
    void inline Xmax(double _xmax) { xmax = _xmax; }
    double inline Xmax() const { return xmax; }

    /// Verifica se o valor de x se encontra entre o intervalo xmin e xmax.
    bool inline TesteIntevalo(double _x) { return _x >= xmin and _x <= xmax; }

    /// Recebe os vetores de x e de y para plotar com o método PlotVector da biblioteca CGnuplot.
    void PreencheVetorRetas(std::vector<double> vx, std::vector<double> vy);

    /// Calcula o valor de f(x), tal que f(x) = a + b*x, com x no intervalo [xmin, xmax].
    double Fx(double _x) const;
    /// Sobrecarga do operador () para calcular o valor de f(x)
    double inline operator()(double _x) const { return Fx(_x); }

    /// Entrada de dados pelo teclado pedindo os valor de a e b.
    void Entrada(std::ostream &os = std::cout, std::istream &in = std::cin);
    /// Entrada de dados por meio de um arquivo de texto pedindo o nome do arquivo.
    void Entrada(const std::string &nomeArquivo);

    /// Saida de dados no terminal.
    void Saida(std::ostream &os = std::cout) const;
    /// Saida de dados para o disco.
    void Saida(const std::string &nomeArquivo);

    /// Sobrecarga operador de inserção << para saida "y = a + b*x".
    friend std::ostream &operator<<(std::ostream &os, const CInterpolacao &inter);
    /// Sobrecarga operador extração >> para receber os coeficientes a e b.
    friend std::istream &operator>>(std::istream &in, CInterpolacao &inter);

    friend bool operator<(const CInterpolacao &reta1, const CInterpolacao &reta2) {return reta1.Xmin() < reta2.Xmin();}
    friend bool operator>(const CInterpolacao &reta1, const CInterpolacao &reta2) {return reta1.Xmax() > reta2.Xmax();}

    /// Plota um grafico em um objeto QCP.
    void Plot(QCustomPlot *grafico);
};

#endif // !CINTERPOLACAO_H
