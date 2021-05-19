#include "notaswidget.h"
#include "ui_notaswidget.h"

NotasWidget::NotasWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotasWidget)
{
    ui->setupUi(this);
}

NotasWidget::~NotasWidget()
{
    delete ui;
}

QString NotasWidget::getLink() const
{
    return link;
}

void NotasWidget::setLink(const QString &value)
{
    link = value;
}

void NotasWidget::agregarNota(const QString &titulo, const float &calif)
{
    ui->califEditLB->setNum(calif);
    ui->tituloEditLB->setText(titulo);
}

void NotasWidget::on_verPB_clicked()
{
    emit notaSeleccionada(link);
}
