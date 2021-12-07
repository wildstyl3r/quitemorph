#ifndef QUITEMORPH_H
#define QUITEMORPH_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
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

private:
    Ui::Quitemorph *ui;
    AfterStable get_after_stable_mode();
    QVector<QGrypho*> g_qgrypho_cache;
    QVector<QGrypho*> h_qgrypho_cache;
    QGrypho g_drawer;
    QGrypho h_drawer;
    Isomorph* test = nullptr;
    bool cache_valid;
};
#endif // QUITEMORPH_H
