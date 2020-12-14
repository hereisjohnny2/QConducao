#ifndef CFFLUXOCALOR_H
#define CFFLUXOCALOR_H

#include <iostream>

/// Objeto função para o calculo do fluxo de calor por condução unidimensional em uma placa plana para materiais isotrópicos.
class CFluxoCalor
{
private:
    double q{1.0};                         ///> Fluxo de Calor Unidimensional (W/m²)

public:
    /// Construtor Padrão.
    CFluxoCalor() = default;
    /// Construtor de Copia
    CFluxoCalor(const CFluxoCalor &obj) : q(obj.q) {}
    /// Destrutor
    ~CFluxoCalor() = default;

    /// Calculo do fluxo de calor com a condutividade termica calculada a partir da curva de interpolação.
    double inline CalculaFluxoCalor(double &dx, double &dt, double &A, double &k) { return q = k * A * dt / dx; }

    /// Sobrecarga do operador (). Torna a classe um objeto função.
    double inline operator()(double dx, double dt, double A, double k) { return CalculaFluxoCalor(dx, dt, A, k); }
};

#endif // !CFFLUXOCALOR_H
