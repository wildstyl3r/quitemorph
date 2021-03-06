#ifndef QUITEMORPH_H
#define QUITEMORPH_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QGrypho.hpp>
#include <isomorph/isomorph.hpp>
#include <isomorph/view.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class Quitemorph; }
QT_END_NAMESPACE

class Quitemorph : public QWidget
{
    Q_OBJECT

public:
    Quitemorph(QWidget *parent = nullptr);
    ~Quitemorph();

private slots:
    void on_g_button_clicked();

    void on_h_button_clicked();

    void on_output_button_clicked();

    void on_start_button_clicked();

    void on_graphics_stateChanged(int arg1);

    void on_show_g_clicked();

    void on_show_h_clicked();

    void on_g_path_textChanged(const QString &arg1);

    void on_h_path_textChanged(const QString &arg1);

private:
    Ui::Quitemorph *ui;
    Graph g, h;
    AfterStable get_after_stable_mode();
    QVector<QGrypho*> g_qgrypho_cache;
    QVector<QGrypho*> h_qgrypho_cache;
    QString current_g;
    QString current_h;
    QGrypho g_drawer;
    QGrypho h_drawer;
    Isomorph* test = nullptr;
    bool cache_valid;
    void highlight(Graph* w, int v, bool graph_H);
    void openTree(Graph* w, int v, bool left, bool graph_H);
};
#endif // QUITEMORPH_H
