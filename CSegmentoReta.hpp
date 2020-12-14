#ifndef CSEGMENTORETA_H
#define CSEGMENTORETA_H

#include "CReta.hpp"

/// Classe CSegmentoReta, expressa uma reta em um intervalo entre um xmin e xmax
class CSegmentoReta : public CReta
{
protected:
    double xmin{0.0}; ///< Início do Segmento de Reta
    double xmax{1.0}; ///< Final do Segmento de Reta

public:
    /// Construtor Default.
    CSegmentoReta() = default;
    /// Constutor de Cópia.
    CSegmentoReta(const CSegmentoReta &segReta) = default;
    /// Construtor sobrecarregado, recebe dois pares ordenados (x1,y1) e (x2,y2).
    CSegmentoReta(double x1, double y1, double x2, double y2) : CReta(x1, y1, x2, y2), xmin(x1), xmax(x2) {}
    /// Destrutor default.
    ~CSegmentoReta() = default;

    /// Operador de atribuição default.
    CSegmentoReta &operator=(const CSegmentoReta &reta) = default;

    /// Get e set para o limite inferior xmin.
    void inline Xmin(double _xmin) { xmin = _xmin; }
    double inline Xmin() const { return xmin; }

    /// Get e set para o limite superior xmax.
    void inline Xmax(double _xmax) { xmax = _xmax; }
    double inline Xmax() const { return xmax; }

    /// Verifica se o valor de x se encontra entre o intervalo xmin e xmax.
    bool inline TesteIntevalo(double _x) { return _x >= xmin and _x <= xmax; }

    /// Calula o valor de f(x), tal que f(x) = a + b*x.
    double inline Fx(double _x) const { return a + b * _x; }
    double inline operator()(double _x) const { return Fx(_x); }

    /// Entrada de dados pelo teclado pedindo os valor de a e b.
    void Entrada(std::ostream &os = std::cout, std::istream &in = std::cin);
    /// Entrada de dados por meio de um arquivo de texto pedindo o nome do arquivo.
    void Entrada(std::string nomeArquivo);

    /// Saida de dados no terminal.
    void Saida(std::ostream &os = std::cout) const;
    /// Saida de dados para o disco.
    void Saida(std::string nomeArquivo);

    /// Sobrecarga operador de inserção << para saida "y = a + b*x".
    friend std::ostream &operator<<(std::ostream &os, const CSegmentoReta &reta);
    /// Sobrecarga operador extração >> para receber os coeficientes a, b, xmin e xmax.
    friend std::istream &operator>>(std::istream &in, CSegmentoReta &reta);

    /// Sobrecarga de operador de igualdade, verifica de os coeficientes são iguais.
    friend bool operator==(const CSegmentoReta &reta1, const CSegmentoReta &reta2);
    /// Sobrecarga de operador de diferença
    friend bool operator!=(const CSegmentoReta &reta1, const CSegmentoReta &reta2);

    /// Plota um gráfico a partir de um objeto QCP. é necessário também informar qual a curva que está sendo plotada.
    void Plot(QCustomPlot *grafico, int curva);
};

#endif // !CSEGMENTORETA_H
