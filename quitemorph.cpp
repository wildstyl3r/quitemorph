#include "quitemorph.h"
#include "./ui_quitemorph.h"

Quitemorph::Quitemorph(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Quitemorph)
{
    ui->setupUi(this);
    ui->plainTextEdit->setVisible(false);
}

Quitemorph::~Quitemorph()
{
    delete ui;
}


void Quitemorph::on_g_button_clicked()
{
    ui->g_path->setText(QFileDialog::getOpenFileName(this, tr("Open graph file"), "~", tr("Graph files ((*.cgf *.tgf *.dot)")));
}

void Quitemorph::on_h_button_clicked()
{
    ui->h_path->setText(QFileDialog::getOpenFileName(this, tr("Open graph file"), "~", tr("Graph files (*.cgf *.tgf *.dot)")));
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
    current_g = ui->g_path->text().trimmed();
    current_h = ui->h_path->text().trimmed();
    g = Graph(current_g.toStdString()), h = Graph(current_h.toStdString());
    ui->progress_g->setRange(0, g.V().size()*3);
    ui->progress_g->setValue(0);
    ui->progress_h->setRange(0, h.V().size()*3);
    ui->progress_h->setValue(0);
    if (ui->color_from_scratch->isChecked()){
        for(auto& c : g.colors()){
            c = 0;
        }
        for(auto& c : h.colors()){
            c = 0;
        }
    }

    test = new Isomorph(g, h, get_after_stable_mode(),
    [&](){
        ui->progress_g->setValue(ui->progress_g->value() + 1);
    },
    [&](){
        ui->progress_h->setValue(ui->progress_h->value() + 1);
    }
    );

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
                    g_qgrypho_cache[v]->setWindowTitle("G : " + QString::fromStdString(test->g().views()[v]->id(v)+
                                                                      " ("+std::to_string(test->g().views()[v]->color(v))+")"));
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
        g_drawer.setWindowTitle("G @ " + ui->g_path->text());
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
                        h_qgrypho_cache[v]->setWindowTitle("H : " + QString::fromStdString(test->h().views()[v]->id(v)+
                                                                  " ("+std::to_string(test->h().views()[v]->color(v))+")"));
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
        h_drawer.setWindowTitle("H @ " + ui->h_path->text());
    }


    ui->show_g->setEnabled(true);
    ui->show_h->setEnabled(true);
    ui->progress_g->setMaximum(1);
    ui->progress_h->setMaximum(1);
    ui->progress_g->setValue(1);
    ui->progress_h->setValue(1);

    //отчеты:

    QString p = "(";
    bool first = true;
    for (auto& e : test->perm()){
        if (first)
            first = false;
        else
            p += ", ";
        p += QString::number(e);
    }
    p += ")\n";

    QString tree_sets;
    for (int i = 0; i < 2; ++i){
        tree_sets += "Деревья " + QString(!i?"G":"H") + " по вершинам:\n";
        for (auto& c : (*test)[i].classes()){
            tree_sets += '(';
            bool first = true;
            for(vertex v : c.second){
                if(!first){
                    tree_sets += ", ";
                } else {
                    first = false;
                }
                tree_sets += QString::fromStdString((*test)[i].views()[0]->id(v));
            }
            tree_sets += "):";
            for(streeng_symbol s : c.first){
                tree_sets += " [" + QString::number(s) + ']';
            }
            tree_sets += '\n';
        }
        tree_sets += '\n';
    }

    ui->plainTextEdit->setPlainText((test->perm().size() == 0 ? "G и H не равны\n" : "H отображается на G: " + p) + tree_sets);
    ui->plainTextEdit->setVisible(true);
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

void Quitemorph::on_graphics_stateChanged(int arg1)
{
    if(arg1 == 0){
        ui->show_g->setVisible(false);
        ui->show_h->setVisible(false);
    } else {
        ui->show_g->setVisible(true);
        ui->show_h->setVisible(true);
    }
}

void Quitemorph::on_show_g_clicked()
{
    g_drawer.show();
}

void Quitemorph::on_show_h_clicked()
{
    h_drawer.show();
}

void Quitemorph::on_g_path_textChanged(const QString &arg1)
{
    ui->show_g->setEnabled(arg1.trimmed() == current_g && current_g != "");
    ui->progress_g->setValue(arg1.trimmed() == current_g && current_g != "");
}

void Quitemorph::on_h_path_textChanged(const QString &arg1)
{
    ui->show_h->setEnabled(arg1.trimmed() == current_h && current_h != "");
    ui->progress_h->setValue(arg1.trimmed() == current_h && current_h != "");
}
