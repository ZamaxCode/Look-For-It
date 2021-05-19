#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QMessageBox>
#include <ctype.h>
#include <regex>
#include <string>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "user.h"
#include "nota.h"
#include "notaswidget.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_correoLE_textChanged(const QString &arg1);

    void on_passwordLE_textChanged(const QString &arg1);

    void on_usuarioLE_textChanged(const QString &arg1);

    void on_correoNuevoLE_textChanged(const QString &arg1);

    void on_passwordNuevoLE_textChanged(const QString &arg1);

    void on_inicarSesionPB_clicked();

    void on_registrarsePB_clicked();

    void on_barraBuscarLE_textChanged(const QString &arg1);

    void on_buscarPB_clicked();

    void on_confirmarCheck_stateChanged(int arg1);

    void on_enviarPB_clicked();

    void on_regresarPB_clicked();

    void on_cerrarSesionPB_clicked();

    void on_artePB_clicked();

    void on_regresarCatPB_clicked();

    void on_economiaPB_clicked();

    void on_historiaPB_clicked();

    void on_culturaFisicaPB_clicked();

    void on_medicinaPB_clicked();

    void on_matematicasPB_clicked();

    void on_geografiaPB_clicked();

    void on_cienciaPB_clicked();

    void on_seleCategoriaCB_currentTextChanged(const QString &arg1);

    void dirigirNota(const QString &link);

private:
    Ui::MainWindow *ui;
    vector<user>vectUsers;
    vector<nota>vectNotas;
    vector<nota>vectNotasCat;
    QFile archivoBD;
    QJsonArray usersBD, notasBD;
    nota notaActual;
    QString usuarioActual;

    //Cargar Base de Datos
    void cargarArchivoBD();
    void cargarBD();
    void guardarBD();

    //validaciones
    bool validarCorreo(const string&);

    //Inicio Sesion
    void inicioSesion();
    void habilitarInicioPB();

    //Registro
    void registro();
    void habilitarRegistroPB();

    //Busqueda Directa
    void busquedaDirecta(const QString&);
    void habilitarBuscarPB();

    //Busqueda Categoria
    void filtrarVectorNotas(const QString&);
    void mostrarCategorias();

    //Calificaciones
    float calculoCalificacion(const int&, const float&, const float&) const;
    void realizarResenia();
    void almacenarResenia(const QString&);
    bool habilitarResenia(const QString&);
    void habilitarEnviarPB();
};
#endif // MAINWINDOW_H
