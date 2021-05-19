#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cargarArchivoBD();
}

MainWindow::~MainWindow()
{
    guardarBD();
    delete ui;
}

void MainWindow::habilitarInicioPB()
{
    if(ui->correoLE->text().length()>0 && ui->passwordLE->text().length()>0)
        ui->inicarSesionPB->setEnabled(true);
    else
        ui->inicarSesionPB->setEnabled(false);
}

void MainWindow::habilitarRegistroPB()
{
    if(ui->correoNuevoLE->text().length()>0 && ui->usuarioLE->text().length()>0 && ui->passwordNuevoLE->text().length()>0)
        ui->registrarsePB->setEnabled(true);
    else
        ui->registrarsePB->setEnabled(false);
}

bool MainWindow::validarCorreo(const string& correo)
{
    bool correo_correcto;
    regex correoValido("([a-zA-Z][\\w\\d]+(([-\\.][\\w\\d]+)+)?@[a-zA-Z][a-zA-Z]+.(([a-zA-Z]{3}(.[a-zA-Z]{2})?)|[a-zA-Z]{2}))");
    if(regex_match(correo,correoValido))
    {
        correo_correcto=true;
        int len=correo.length();
        for(int i=0;i<len;i++)
        {
            if((((correo)[i]=='.')||((correo)[i]=='-')||((correo)[i]=='_'))&&(((correo)[i+1]=='.')||((correo)[i+1]=='-')||((correo)[i+1]=='_')))
            {
                correo_correcto=false;
            }
        }
    }
    else
    {
        correo_correcto=false;
    }
    return correo_correcto;
}

void MainWindow::inicioSesion()
{
    QMessageBox mensaje;
    size_t i;

    if(validarCorreo(ui->correoLE->text().toStdString())!=true)
    {
        mensaje.setText("Correo Invalido");
        mensaje.setIcon(QMessageBox::Warning);
        mensaje.exec();
    }
    else
    {
        for(i=0;i<vectUsers.size();++i)
        {
            if(ui->correoLE->text()==vectUsers.at(i).getEmail() && ui->passwordLE->text()==vectUsers.at(i).getPassword())
            {
                mensaje.setText("Bienvenid@ " + vectUsers.at(i).getName());
                usuarioActual=vectUsers.at(i).getName();
                ui->categoriasSW->setCurrentIndex(1);
                mensaje.exec();
                break;
            }
        }
        if(i==vectUsers.size())
        {
            mensaje.setText("Correo/Contraseña Invalidos");
            mensaje.setIcon(QMessageBox::Warning);
            mensaje.exec();
        }
    }
}

void MainWindow::registro()
{
    QMessageBox mensaje;
    QJsonObject jsonObj;
    QJsonArray arregloVacio;
    size_t i;
    user u;
    if(validarCorreo(ui->correoNuevoLE->text().toStdString())!=true)
    {
        mensaje.setText("Correo Invalido");
        mensaje.setIcon(QMessageBox::Warning);
        mensaje.exec();
    }
    else
    {
        for(i=0;i<vectUsers.size();++i)
        {
            if(ui->correoNuevoLE->text()==vectUsers.at(i).getEmail() || ui->usuarioLE->text()==vectUsers.at(i).getName())
            {
                mensaje.setText("Correo/Nombre de usuario ya existente");
                mensaje.setIcon(QMessageBox::Warning);
                mensaje.exec();
                break;
            }
        }
        if(i==vectUsers.size())
        {
            u.setName(ui->usuarioLE->text());
            u.setEmail(ui->correoNuevoLE->text());
            u.setPassword(ui->passwordNuevoLE->text());
            vectUsers.push_back(u);

            jsonObj["name"] = u.getName();
            jsonObj["email"] = u.getEmail();
            jsonObj["password"] = u.getPassword();
            jsonObj["resenias"] = arregloVacio;
            usersBD.append(jsonObj);

            mensaje.setText("Registro Exitoso!");
            mensaje.exec();
        }
    }
}

void MainWindow::habilitarBuscarPB()
{
    if(ui->barraBuscarLE->text().length()>0)
        ui->buscarPB->setEnabled(true);
    else
        ui->buscarPB->setEnabled(false);

}

void MainWindow::filtrarVectorNotas(const QString &categoria)
{
    vectNotasCat.clear();
    if(categoria=="Todo")
        vectNotasCat=vectNotas;
    else
        for(size_t i(0); i<vectNotas.size(); ++i)
            if(vectNotas.at(i).getCategoria()==categoria)
                vectNotasCat.push_back(vectNotas.at(i));
}

void MainWindow::mostrarCategorias()
{
    NotasWidget* nw;
    for(unsigned i(0); i<vectNotasCat.size(); ++i)
    {
        nw = new NotasWidget();
        nw->setLink(vectNotasCat.at(i).getLink());
        nw->agregarNota(vectNotasCat.at(i).getTitulo(),vectNotasCat.at(i).getCalificacion());
        nw->setLink(vectNotasCat.at(i).getLink());
        connect(nw, SIGNAL(notaSeleccionada(QString)), this, SLOT(dirigirNota(QString)));
        ui->notasFiltroGL->addWidget(nw, i/1, i%1,Qt::AlignCenter);
    }
}

float MainWindow::calculoCalificacion(const int &cont, const float &promT, const float &promU) const
{
    float califTotal;
    califTotal=((promT*cont)+promU)/(cont+1);
    return califTotal;
}

void MainWindow::realizarResenia()
{
    QMessageBox mensaje;
    float prom=(ui->prestigioSB->value()+ui->renombreSB->value()+ui->calidadSB->value()+ui->antiguedadSB->value())/4;
    if(notaActual.getContador()==0)
        notaActual.setCalificacion(prom);
    else
        notaActual.setCalificacion(calculoCalificacion(notaActual.getContador(), notaActual.getCalificacion(), prom));

    notaActual.setContador(notaActual.getContador()+1);

    for(size_t i(0); i<vectNotas.size(); ++i)
    {
        if(notaActual.getLink()==vectNotas.at(i).getLink())
        {
            QJsonObject obj;
            vectNotas.at(i)=notaActual;
            obj["calificacion"]=notaActual.getCalificacion();
            obj["categoria"]=notaActual.getCategoria();
            obj["contador"]=notaActual.getContador();
            obj["descripcion"]=notaActual.getDescripcion();
            obj["link"]=notaActual.getLink();
            obj["titulo"]=notaActual.getTitulo();
            notasBD.replace(i, obj);
            break;
        }
    }

    almacenarResenia(notaActual.getLink());

    QString calif;
    calif.setNum(notaActual.getCalificacion());
    ui->porcentajeLB->setText(calif+'%');

    mensaje.setText("Reseña realizada con exito");
    mensaje.exec();
}

void MainWindow::almacenarResenia(const QString &link)
{
    QJsonObject objUsuario, objReseniaNueva;
    QJsonArray resenias;
    for(int i(0); i<usersBD.size(); ++i)
    {
        objUsuario=usersBD.at(i).toObject();
        if(objUsuario["name"]==usuarioActual)
        {
            resenias=objUsuario["resenias"].toArray();
            objReseniaNueva["link"]=link;
            resenias.append(objReseniaNueva);
            objUsuario["resenias"]=resenias;
            usersBD.replace(i, objUsuario);
            break;
        }
    }
}

bool MainWindow::habilitarResenia(const QString &link)
{
    QJsonObject objUsuario, objResenia;
    QJsonArray resenias;
    bool reseniaHecha=false;
    for(int i(0); i<usersBD.size(); ++i)
    {
        objUsuario=usersBD.at(i).toObject();
        if(objUsuario["name"]==usuarioActual)
        {
            resenias=objUsuario["resenias"].toArray();
            for(int j(0); resenias.size(); ++j)
            {
                objResenia=resenias.at(j).toObject();
                if(objResenia["link"]==link)
                {
                    reseniaHecha=true;
                    break;
                }
            }
            break;
        }
    }
    return reseniaHecha;
}

void MainWindow::habilitarEnviarPB()
{
    if(ui->confirmarCheck->checkState())
        ui->enviarPB->setEnabled(true);
    else
        ui->enviarPB->setEnabled(false);
}

void MainWindow::busquedaDirecta(const QString &link)
{
    QMessageBox mensaje;
    size_t i;
    QString calif;

    for(i=0; i<vectNotas.size(); ++i)
    {
        if(link==vectNotas.at(i).getLink())
        {
            ui->tituloLB->setText(vectNotas.at(i).getTitulo());
            ui->descripcionLB->setText(vectNotas.at(i).getDescripcion());
            calif.setNum(vectNotas.at(i).getCalificacion());
            ui->porcentajeLB->setText(calif+'%');
            notaActual=vectNotas.at(i);
            if(habilitarResenia(vectNotas.at(i).getLink())){
                ui->evaluacionGB->setEnabled(false);
            }
            ui->categoriasSW->setCurrentIndex(3);
            break;
        }
    }
    if(i==vectNotas.size())
    {
        mensaje.setText("No se encontro registro\nLo sentimos");
        mensaje.setIcon(QMessageBox::Warning);
        mensaje.exec();
    }
}

void MainWindow::cargarArchivoBD()
{
    QString name;
    name = "Z:/Personal/Escritorio/Preyecto Ing. Software/LOOK_FOR_IT/lookForItBD.json";
    archivoBD.setFileName(name);
    cargarBD();
}

void MainWindow::cargarBD()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray data;

    archivoBD.open(QIODevice::ReadOnly);
    data = archivoBD.readAll();
    archivoBD.close();

    jsonDoc = QJsonDocument::fromJson(data);
    jsonObj = jsonDoc.object();
    usersBD = jsonObj["users"].toArray();

    for(int i(0); i < usersBD.size(); i++)
    {
        user u;
        QJsonObject obj = usersBD[i].toObject();
        u.setName(obj["name"].toString());
        u.setEmail(obj["email"].toString());
        u.setPassword(obj["password"].toString());
        vectUsers.push_back(u);
    }

    notasBD = jsonObj["info"].toArray();
    for(int i(0); i < notasBD.size(); i++)
    {
        nota n;
        QJsonObject obj = notasBD[i].toObject();
        n.setLink(obj["link"].toString());
        n.setTitulo(obj["titulo"].toString());
        n.setDescripcion(obj["descripcion"].toString());
        n.setCategoria(obj["categoria"].toString());
        n.setCalificacion(obj["calificacion"].toDouble());
        n.setContador(obj["contador"].toInt());
        vectNotas.push_back(n);
    }

}

void MainWindow::guardarBD()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;

    jsonObj["users"] = usersBD;
    jsonObj["info"] = notasBD;
    jsonDoc = QJsonDocument(jsonObj);

    archivoBD.open(QIODevice::WriteOnly);
    archivoBD.write(jsonDoc.toJson());
    archivoBD.close();
}


//Slots

void MainWindow::on_correoLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    habilitarInicioPB();
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    habilitarInicioPB();
}

void MainWindow::on_usuarioLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    habilitarRegistroPB();
}

void MainWindow::on_correoNuevoLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    habilitarRegistroPB();
}

void MainWindow::on_passwordNuevoLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    habilitarRegistroPB();
}

void MainWindow::on_inicarSesionPB_clicked()
{
    inicioSesion();
    ui->correoLE->clear();
    ui->passwordLE->clear();
    ui->usuarioLE->clear();
    ui->correoNuevoLE->clear();
    ui->passwordNuevoLE->clear();
}

void MainWindow::on_registrarsePB_clicked()
{
    registro();
    ui->correoLE->clear();
    ui->passwordLE->clear();
    ui->usuarioLE->clear();
    ui->correoNuevoLE->clear();
    ui->passwordNuevoLE->clear();
}

void MainWindow::on_barraBuscarLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    habilitarBuscarPB();
}

void MainWindow::on_buscarPB_clicked()
{
    busquedaDirecta(ui->barraBuscarLE->text());
    ui->barraBuscarLE->clear();
}

void MainWindow::on_confirmarCheck_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    habilitarEnviarPB();
}

void MainWindow::on_enviarPB_clicked()
{
    realizarResenia();
    ui->evaluacionGB->setEnabled(false);
    ui->prestigioSB->setValue(0);
    ui->renombreSB->setValue(0);
    ui->calidadSB->setValue(0);
    ui->antiguedadSB->setValue(0);
}

void MainWindow::on_regresarPB_clicked()
{
    ui->categoriasSW->setCurrentIndex(1);
}

void MainWindow::on_cerrarSesionPB_clicked()
{
    ui->categoriasSW->setCurrentIndex(0);
}

void MainWindow::on_regresarCatPB_clicked()
{
    ui->categoriasSW->setCurrentIndex(1);
}

void MainWindow::on_artePB_clicked()
{
    ui->seleCategoriaCB->setCurrentText(ui->artePB->text());
    filtrarVectorNotas(ui->seleCategoriaCB->currentText());
    mostrarCategorias();
    ui->categoriasSW->setCurrentIndex(2);
}

void MainWindow::on_economiaPB_clicked()
{
    ui->seleCategoriaCB->setCurrentText(ui->economiaPB->text());
    filtrarVectorNotas(ui->seleCategoriaCB->currentText());
    mostrarCategorias();
    ui->categoriasSW->setCurrentIndex(2);
}

void MainWindow::on_historiaPB_clicked()
{
    ui->seleCategoriaCB->setCurrentText(ui->historiaPB->text());
    filtrarVectorNotas(ui->seleCategoriaCB->currentText());
    mostrarCategorias();
    ui->categoriasSW->setCurrentIndex(2);
}

void MainWindow::on_culturaFisicaPB_clicked()
{
    ui->seleCategoriaCB->setCurrentText(ui->culturaFisicaPB->text());
    filtrarVectorNotas(ui->seleCategoriaCB->currentText());
    mostrarCategorias();
    ui->categoriasSW->setCurrentIndex(2);
}

void MainWindow::on_medicinaPB_clicked()
{
    ui->seleCategoriaCB->setCurrentText(ui->medicinaPB->text());
    filtrarVectorNotas(ui->seleCategoriaCB->currentText());
    mostrarCategorias();
    ui->categoriasSW->setCurrentIndex(2);
}

void MainWindow::on_matematicasPB_clicked()
{
    ui->seleCategoriaCB->setCurrentText(ui->matematicasPB->text());
    filtrarVectorNotas(ui->seleCategoriaCB->currentText());
    mostrarCategorias();
    ui->categoriasSW->setCurrentIndex(2);
}

void MainWindow::on_geografiaPB_clicked()
{
    ui->seleCategoriaCB->setCurrentText(ui->geografiaPB->text());
    filtrarVectorNotas(ui->seleCategoriaCB->currentText());
    mostrarCategorias();
    ui->categoriasSW->setCurrentIndex(2);
}

void MainWindow::on_cienciaPB_clicked()
{
    ui->seleCategoriaCB->setCurrentText(ui->cienciaPB->text());
    filtrarVectorNotas(ui->seleCategoriaCB->currentText());
    mostrarCategorias();
    ui->categoriasSW->setCurrentIndex(2);
}

void MainWindow::on_seleCategoriaCB_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    QLayoutItem* child;

    while((child = ui->notasFiltroGL->takeAt(0))!=0){
        delete child->widget();
    }

    filtrarVectorNotas(ui->seleCategoriaCB->currentText());
    mostrarCategorias();
}

void MainWindow::dirigirNota(const QString &link)
{
    busquedaDirecta(link);
}
