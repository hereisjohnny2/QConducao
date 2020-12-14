#ifndef CRETA_H
#define CRETA_H

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "qcustomplot.h"

/// Classe CReta: Utilizada para representa uma função na forma f(x) = a + bx, onde a e b são atributos.
class CReta
{
protected:
    double b{0.0}; ///< Representa o coeficiente angular b de uma reta.
    double a{0.0}; ///< Representa o coeficiente linear a de uma reta.

public:
    /// Construtor default.
    CReta() = default;
    /// Construtor sobrecarregado para receber os coeficientes a e b da reta.
    CReta(double _a, double _b) : b(_b), a(_a) {}
    /// Construtor sobrecarregado para receber dois pares ordenados ((x1,y1) e (x2,y2)) e calcular os valores de a e b.
    CReta(double x1, double y1, double x2, double y2) : b{(y2 - y1) / (x2 - x1)}, a{y1 - b * x1} {}
    /// Construtor de cópia.
    CReta(const CReta &reta) = default;
    /// Construtor move.
    CReta(CReta &&) = default;
    /// Destrutor.
    ~CReta() = default;

    /// Get e Set para coeficiente A.
    void inline A(double _a) { a = _a; }
    double inline A() const { return a; }
    /// Get e Set para coeficiente B.
    void inline B(double _b) { b = _b; }
    double inline B() const { return b; }

    /// Calula o valor de f(x), tal que f(x) = a + b*x.
    double inline Fx(double _x) const { return a + b * _x; }
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
    friend std::ostream &operator<<(std::ostream &os, const CReta &reta);
    /// Sobrecarga operador extração >> para receber os coeficientes a e b.
    friend std::istream &operator>>(std::istream &in, CReta &reta);

    /// Sobrecarga de operador de igualdade, verifica de os coeficientes são iguais.
    friend bool operator==(const CReta &reta1, const CReta &reta2);
    /// Sobrecarga de operador de diferença
    friend bool operator!=(const CReta &reta1, const CReta &reta2);

    /// Retorna string com equação completa f(x) = a + b*x.
    std::string Equacao() const;

    /// Plota um gráfico a partir de um objeto QCP. é necessário também informar qual a curva que está sendo plotada.
    void Plot(QCustomPlot *grafico, int curva);
};

#endif // !CRETA_H
