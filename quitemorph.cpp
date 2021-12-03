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
    ui->output_path->setText(QFileDialog::getSaveFileName(this, tr("Select export file"), "", tr("Quitemorph spanning web (*.qsw *.csw);;Quitemorph testing report (*.mtr)")));
}

void Quitemorph::on_start_button_clicked()
{
    if (ui->g_path->text() == "" || ui->h_path->text() == "") {
        QMessageBox::critical(this, tr("Ошибка"), tr("Укажите файлы для анализа"));
    }
}
