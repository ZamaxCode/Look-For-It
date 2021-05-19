#ifndef NOTASWIDGET_H
#define NOTASWIDGET_H

#include <QWidget>

namespace Ui {
class NotasWidget;
}

class NotasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NotasWidget(QWidget *parent = nullptr);
    ~NotasWidget();

    QString getLink() const;
    void setLink(const QString &value);

    void agregarNota(const QString&, const float&);

signals:
    void notaSeleccionada(QString link);

private slots:
    void on_verPB_clicked();

private:
    Ui::NotasWidget *ui;
    QString link;
};

#endif // NOTASWIDGET_H
