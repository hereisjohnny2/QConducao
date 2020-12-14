#include <random>

#include "CInterpolacao.hpp"

CInterpolacao::CInterpolacao(const std::string &nomeArquivo)
{
    Entrada(nomeArquivo);
}

CInterpolacao::~CInterpolacao()
{
    for (auto reta : retas)
    {
        delete reta;
    }
}

void CInterpolacao::PreencheVetorRetas(std::vector<double> vx, std::vector<double> vy)
{
    if (!vx.empty() and !vy.empty())
    {
        for (auto reta : retas)
        {
            delete reta;
        }
        retas.clear();

        xmin = vx[0];
        xmax = vx.back();

        for (size_t i = 1; i < vx.size(); i++)
        {
            CSegmentoReta *reta = new CSegmentoReta(vx[i - 1], vy[i - 1], vx[i], vy[i]);
            retas.push_back(reta);
        }
    }

}

double CInterpolacao::Fx(double _x) const
{
    if (_x < xmin)
        return (*retas[0])(_x);
    else if (_x > xmax)
        return (*retas.back())(_x);

    for (auto reta : retas)
    {
        if (reta->TesteIntevalo(_x))
            return reta->Fx(_x);
    }
    return 0;
}

void CInterpolacao::Entrada(std::ostream &os, std::istream &in)
{
    double x, y;
    std::vector<double> vx, vy;

    do
    {
        os << "Digite os valores de X e Y do ponto " << vx.size() + 1 << ": ";
        in >> x >> y;
        in.get();
        if (in.good())
        {
            vx.push_back(x);
            vy.push_back(y);
        }
    } while (std::cin.good());
    in.clear();
    os << std::endl;

    PreencheVetorRetas(vx, vy);
}

void CInterpolacao::Entrada(const std::string &nomeArquivo)
{
    std::ifstream fin(nomeArquivo);

    double x, y;
    std::vector<double> vx, vy;

    if (fin.is_open())
    {
        while((fin >> x).good())
        {
            fin >> y;
            vx.push_back(x);
            vy.push_back(y);
        }
    }
    else
    {
        std::cerr << "Falha ao ler o arquivo";
    }

    fin.close();

    PreencheVetorRetas(vx, vy);
}

void CInterpolacao::Saida(std::ostream &os) const
{
    for (auto reta : retas)
    {
        reta->Saida(os);
        os << std::endl;
    }
}

void CInterpolacao::Saida(const std::string &nomeArquivo)
{
    std::ofstream fout(nomeArquivo);

    if (!fout)
    {
        std::cerr << "Não foi possível abrir o arquivo!\n";
        exit(1);
    }
    Saida(fout);

    fout.close();
}

std::ostream &operator<<(std::ostream &os, const CInterpolacao &inter)
{
    for (auto reta : inter.retas)
    {
        os << (*reta) << "\n";
    }

    return os;
}

std::istream &operator>>(std::istream &in, CInterpolacao &inter)
{
    for (auto reta : inter.retas)
    {
        in >> (*reta);
    }

    return in;
}

void CInterpolacao::Plot(QCustomPlot *grafico)
{
    if (!retas.empty())
    {
        std::random_device rand;
        std::uniform_int_distribution<int> dist(0,100);
        int numCurva;

        grafico->legend->setVisible(true);

        for (auto &reta : retas)
        {
            grafico->addGraph();

            numCurva = grafico->graphCount() - 1;
            reta->Plot(grafico, numCurva);

            grafico->graph(numCurva)->setName(QString::fromStdString(reta->Equacao()));

            QPen estiloCurva;
            estiloCurva.setColor(QColor(dist(rand),dist(rand),dist(rand),255));
            estiloCurva.setWidth(2);

            grafico->graph(numCurva)->setPen(estiloCurva);
        }
    }
}
