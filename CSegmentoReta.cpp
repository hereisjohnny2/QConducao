#include "CSegmentoReta.hpp"
#include <random>

void CSegmentoReta::Entrada(std::ostream &os, std::istream &in)
{
    os << "Entre com o valor de a, b, xmin e xmax: ";
    in >> a >> b >> xmin >> xmax;
    os << CSegmentoReta::Equacao() << "\n";
}

void CSegmentoReta::Entrada(std::string nomeArquivo)
{
    std::ifstream fin(nomeArquivo);
    if (!fin)
    {
        std::cerr << "Não foi possível abrir o arquivo!\n";
        exit(1);
    }
    fin >> a >> b >> xmin >> xmax;
    fin.close();
}

void CSegmentoReta::Saida(std::ostream &os) const
{
    os << "[" << xmin << " -> " << xmax << "] "
       << "y = " << std::setw(10) << a << " + " << std::setw(10) << b << "*x";
}

void CSegmentoReta::Saida(std::string nomeArquivo)
{
    std::ofstream fout(nomeArquivo);
    if (!fout)
    {
        std::cerr << "Não foi possível abrir o arquivo!\n";
        exit(1);
    }
    fout << a << "\t" << b << "\t" << xmin << "\t" << xmax;
    fout.close();
}

std::ostream &operator<<(std::ostream &os, const CSegmentoReta &reta)
{
    os << reta.a << "\t" << reta.b << "\t"
       << reta.xmin << "\t" << reta.xmax;
    return os;
}

std::istream &operator>>(std::istream &in, CSegmentoReta &reta)
{
    in >> reta.a >> reta.b >> reta.xmin >> reta.xmax;
    return in;
}

bool operator==(const CSegmentoReta &reta1, const CSegmentoReta &reta2)
{
    return (reta1.a == reta2.a and reta1.b == reta2.b and reta1.xmin == reta2.xmin and reta1.xmax == reta2.xmax);
}

bool operator!=(const CSegmentoReta &reta1, const CSegmentoReta &reta2)
{
    return !(reta1 == reta2);
}


void CSegmentoReta::Plot(QCustomPlot *grafico, int curva)
{
    QVector<double> x{xmin,xmax}, y{Fx(xmin), Fx(xmax)};
    grafico->graph(curva)->setData(x,y);
}
