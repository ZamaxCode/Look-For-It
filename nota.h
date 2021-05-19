#ifndef NOTA_H
#define NOTA_H
#include <QString>

using namespace std;

class nota
{
public:
    nota();

    QString getTitulo() const;
    void setTitulo(const QString &value);

    QString getDescripcion() const;
    void setDescripcion(const QString &value);

    QString getLink() const;
    void setLink(const QString &value);

    float getCalificacion() const;
    void setCalificacion(float value);

    QString getCategoria() const;
    void setCategoria(const QString &value);

    int getContador() const;
    void setContador(int value);

private:
    QString titulo;
    QString descripcion;
    QString link;
    QString categoria;
    float calificacion;
    int contador;
};

#endif // NOTA_H
