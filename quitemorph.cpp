#include "quitemorph.h"
#include "./ui_quitemorph.h"

Quitemorph::Quitemorph(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Quitemorph)
{
    ui->setupUi(this);
}

Quitemorph::~Quitemorph()
{
    delete ui;
}


void Quitemorph::on_g_button_clicked()
{
    ui->g_path->setText(QFileDialog::getOpenFileName(this, tr("Open graph file"), "~", tr("Graph files (*.tgf *.dot)")));
}

void Quitemorph::on_h_button_clicked()
{
    ui->h_path->setText(QFileDialog::getOpenFileName(this, tr("Open graph file"), "~", tr("Graph files (*.tgf *.dot)")));
}

void Quitemorph::on_output_button_clicked()
{
    ui->output_path->setText(
                QFileDialog::getSaveFileName(this,
                                             tr("Select export file"),
                                             "",
                                             tr("Canonical spanning web (*.qsw *.csw);;Quitemorph testing report (*.mtr)")));
}

void Quitemorph::on_start_button_clicked()
{
    if (ui->g_path->text() == "" || ui->h_path->text() == "") {
        QMessageBox::critical(this, tr("Ошибка"), tr("Укажите файлы для анализа"));
        return;
    }
    Graph g(ui->g_path->text().toStdString()), h(ui->h_path->text().toStdString());
    if (ui->color_from_scratch->isChecked()){
        for(auto& c : g.colors()){
            c = 0;
        }
        for(auto& c : g.colors()){
            c = 0;
        }
    }

    test = new Isomorph(g, h, get_after_stable_mode());

    if(ui->graphics->isChecked()){
        for(auto &qg : g_qgrypho_cache){
            delete qg;
        }
        g_qgrypho_cache.clear();
        for(int v = 0; v < g.V().size(); ++v){
            g_qgrypho_cache.push_back(new QGrypho);
        }

        g_drawer.drawGraph(&g);
        g_drawer.text = "итераций перекраски: " + QString::number(test->g().iterations()) + "\n" +
                "время: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(test->g().time()).count()) + "мс\n";

        g_drawer.setDblClick([&](Graph* g, int v, bool left){
            if (g->has(v)){
                if (left){
                    if(!g_qgrypho_cache[v]->isGraphSet()){
                        g_qgrypho_cache[v]->drawGraph(test->g().views()[v]->lexmst_tree());
                        g_qgrypho_cache[v]->highlight(QVector<vertex>(1,v));
                    }
                    g_qgrypho_cache[v]->show();
                }
            }
        });
        g_drawer.setSelect([&](Graph* g, int v){
            if (g->has(v)){
                QVector<edge> es;
                if(!g_drawer.highlighted(v)){
                    g_drawer.highlight(QVector<vertex>(1,v));
                    for(vertex i = 0; i < test->g().views()[v]->V().size(); ++i){
                        for(vertex j = 0; j < test->g().views()[v]->V().size(); ++j){
                            if (test->g().views()[v]->has({i,j})){
                                es.push_back(edge({i,j}));
                            }
                        }
                    }
                    g_drawer.highlight(es);
                } else {
                    for(edge& e : test->g().views()[v]->lexmst_edges()){
                        es.push_back(edge(e));
                    }
                    g_drawer.highlight(es);
                }
            } else {
                g_drawer.highlight(QVector<vertex>());
                g_drawer.highlight(QVector<edge>());
            }
        });
        g_drawer.show();
        ///////////////-------------------------------------------------
        for(auto &qg : h_qgrypho_cache){
            delete qg;
        }
        h_qgrypho_cache.clear();
        for(int v = 0; v < h.V().size(); ++v){
            h_qgrypho_cache.push_back(new QGrypho);
        }

        h_drawer.drawGraph(&h);
        h_drawer.text = "итераций перекраски: " + QString::number(test->h().iterations()) + "\n" +
                "время: " + QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(test->h().time()).count()) + "мс\n";

        h_drawer.setDblClick([&](Graph* w, int v, bool left){
            if (w->has(v)){
                if (left){
                    if(!h_qgrypho_cache[v]->isGraphSet()){
                        h_qgrypho_cache[v]->drawGraph(test->h().views()[v]->lexmst_tree());
                        h_qgrypho_cache[v]->highlight(QVector<vertex>(1,v));
                    }
                    h_qgrypho_cache[v]->show();
                }
            }
        });
        h_drawer.setSelect([&](Graph* w, int v){
            if (w->has(v)){
                QVector<edge> es;
                if(!h_drawer.highlighted(v)){
                    h_drawer.highlight(QVector<vertex>(1,v));
                    for(vertex i = 0; i < test->h().views()[v]->V().size(); ++i){
                        for(vertex j = 0; j < test->h().views()[v]->V().size(); ++j){
                            if (test->h().views()[v]->has({i,j})){
                                es.push_back(edge({i,j}));
                            }
                        }
                    }
                    h_drawer.highlight(es);
                } else {
                    for(edge& e : test->h().views()[v]->lexmst_edges()){
                        es.push_back(edge(e));
                    }
                    h_drawer.highlight(es);
                }
            } else {
                h_drawer.highlight(QVector<vertex>());
                h_drawer.highlight(QVector<edge>());
            }
        });
        h_drawer.show();
    }
    QString p = "(";
    bool first = true;
    for (auto& e : test->perm()){
        if (first)
            first = false;
        else
            p += ", ";
        p += QString::number(e);
    }
    p += ")";
    ui->plainTextEdit->setPlainText(test->perm().size() == 0 ? "G и H не равны" : "H отображается на G: " + p);
}

AfterStable Quitemorph::get_after_stable_mode()
{
    if(ui->after_stable_mode->currentIndex() == 0){
        return AfterStable::None;
    } else if (ui->after_stable_mode->currentIndex() == 1) {
        return AfterStable::Bruteforce;
    } else {
        return AfterStable::Destabilize;
    }
}
