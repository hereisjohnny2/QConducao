#include <vector>
#include <filesystem>
#include <algorithm>
#include <random>

#include "CCondutividade.hpp"

namespace fs = std::filesystem;

using namespace std;

CCondutividade::CCondutividade(const std::string &_nomeCaminho)
{
    AtualizaListaMaterial(_nomeCaminho);
}

void CCondutividade::AdicionaMaterial(std::ostream &os, std::istream &in)
{
    os << "Digite o nome do novo material: ";
    string novoMaterial;
    in >> novoMaterial;

    std::ofstream file("../data/" + novoMaterial + ".dat");

    double x, y;
    if (file.is_open())
    {
        do
        {
            os << "Digite os valores de X e Y do ponto : ";
            in >> x >> y;
            in.get();
            if (in.good())
            {
                file << x << "\t" << y << "\n";
            }
        } while (std::cin.good());

        in.clear();
        os << std::endl;
    }
    file.close();

    AtualizaListaMaterial("data");
}

void CCondutividade::AdicionaMaterial(const std::string &nomeMaterial, const std::string &nomeCaminho)
{
    std::ofstream file(nomeCaminho + "/" + nomeMaterial + ".dat");
    file.close();
}

void CCondutividade::AtualizaListaMaterial(const std::string &nomeCaminho, const std::string &extensao)
{
    data.clear();

    for (const auto &material : fs::directory_iterator(nomeCaminho))
        if (material.path().extension() == extensao)
            data[material.path().stem()] = make_shared<CInterpolacao>(string(material.path()));
}

void CCondutividade::Plot(QCustomPlot *grafico)
{
    std::random_device rand;
    std::uniform_int_distribution<int> dist(0,100);

    QPen estiloCurva;

    for (auto material = data.begin(); material != data.end(); ++material)
    {
        if (!material->second->Retas().empty())
        {
            QVector<double> vx, vy;

            estiloCurva.setColor(QColor(dist(rand),dist(rand),dist(rand),255));
            estiloCurva.setWidth(2);

            grafico->addGraph();
            for (auto &reta : material->second->Retas())
            {
                vx.push_back(reta->Xmin());
                vy.push_back(reta->Fx(reta->Xmin()));
            }
            vx.push_back(material->second->Retas().back()->Xmax());
            vy.push_back(material->second->Retas().back()->Fx(material->second->Retas().back()->Xmax()));

            grafico->graph(grafico->graphCount() - 1)->setData(vx, vy);
            grafico->graph(grafico->graphCount() - 1)->setName(QString::fromStdString(material->first));

            grafico->graph(grafico->graphCount() - 1)->setPen(estiloCurva);
        }
    }
    grafico->legend->setVisible(true);
}
