#include "nota.h"

nota::nota()
{

}

QString nota::getTitulo() const
{
    return titulo;
}

void nota::setTitulo(const QString &value)
{
    titulo = value;
}

QString nota::getDescripcion() const
{
    return descripcion;
}

void nota::setDescripcion(const QString &value)
{
    descripcion = value;
}

QString nota::getLink() const
{
    return link;
}

void nota::setLink(const QString &value)
{
    link = value;
}

float nota::getCalificacion() const
{
    return calificacion;
}

void nota::setCalificacion(float value)
{
    calificacion = value;
}

QString nota::getCategoria() const
{
    return categoria;
}

void nota::setCategoria(const QString &value)
{
    categoria = value;
}

int nota::getContador() const
{
    return contador;
}

void nota::setContador(int value)
{
    contador = value;
}
