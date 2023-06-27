#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ApiMethods.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->socket = new QTcpSocket();
    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));

    QHostAddress host(QHostAddress::LocalHost);
    socket->connectToHost(host, 6000);

    ui->setupUi(this);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setFixedSize(QSize(800, 600));

    QValidator *validator = new QRegularExpressionValidator(QRegularExpression("[0-9]*"), this);
    ui->leGroupNumber->setValidator(validator);
    ui->leID->setValidator(validator);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::socketReadyRead() {
    qDebug() <<"reading";
    QDataStream inputStream(socket);

    qint16 blockSize = 0;
    //block size is 2 bytes, if recieved less - do nothing
    if (socket->bytesAvailable() < sizeof(quint16)) {
        return;
    }
    //read the block size
    inputStream >> blockSize;
    //if the full block have not been recieved yet - do nothing
    if (socket->bytesAvailable() < blockSize) {
        return;
    }
    //read the method that was used on the server
    qint8 method;
    inputStream >> method;
    //read rest of the data - can be none (depends on the method)
    QByteArray jsonData;
    inputStream >> jsonData;

    processAnswer(method, jsonData);
}

//converts all of the lineEdits data to json (except leID)
QByteArray MainWindow::dataToJson() {
    QJsonValue name(ui->leName->text());
    QJsonValue surname(ui->leSurname->text());
    QJsonValue fathername(ui->leFathername->text());

    QString genderFromUi = (ui->genderMale->isChecked()) ? ("М") : ("Ж");
    QJsonValue gender(genderFromUi);
    QJsonValue group_number(ui->leGroupNumber->text());

    QJsonObject jsonObject;
    jsonObject.insert("name", name);
    jsonObject.insert("surname", surname);
    jsonObject.insert("fathername", fathername);
    jsonObject.insert("gender", gender);
    jsonObject.insert("group_number", group_number);

    QJsonDocument json(jsonObject);
    return json.toJson();
}

void MainWindow::updateTable(QByteArray &jsonData) {
    //json is in the format of array of objects
    //each object: {id:, name:, surname:, fathername:, gender:, group_number:}
    QJsonDocument json = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = json.array();
    QJsonArray::iterator i;
    //clear the table
    ui->table->setRowCount(0);
    foreach (const QJsonValue &i, jsonArray) {
        //add a row for a new user
        ui->table->insertRow(ui->table->rowCount());
        QString id = i.toObject().value("id").toVariant().toString();
        QString name = i.toObject().value("name").toVariant().toString();
        QString surname = i.toObject().value("surname").toVariant().toString();
        QString fathername = i.toObject().value("fathername").toVariant().toString();
        QString gender = i.toObject().value("gender").toVariant().toString();
        QString group_number = i.toObject().value("group_number").toVariant().toString();

        QTableWidgetItem* idItem = new QTableWidgetItem(id);
        QTableWidgetItem* nameItem = new QTableWidgetItem(name);
        QTableWidgetItem* surnameItem = new QTableWidgetItem(surname);
        QTableWidgetItem* fathernameItem = new QTableWidgetItem(fathername);
        QTableWidgetItem* genderItem = new QTableWidgetItem(gender);
        QTableWidgetItem* group_numberItem = new QTableWidgetItem(group_number);

        ui->table->setItem(ui->table->rowCount()-1, 0, idItem);
        ui->table->setItem(ui->table->rowCount()-1, 1, nameItem);
        ui->table->setItem(ui->table->rowCount()-1, 2, surnameItem);
        ui->table->setItem(ui->table->rowCount()-1, 3, fathernameItem);
        ui->table->setItem(ui->table->rowCount()-1, 4, genderItem);
        ui->table->setItem(ui->table->rowCount()-1, 5, group_numberItem);
    }
}

void MainWindow::processAnswer(qint8 method, QByteArray jsonData) {
    switch (method) {
    case Methods::checkConnection:
    {
        QMessageBox::information(this, "Успешно", "Сервер работает и слушает ваши запросы.");
    }
    break;
    case Methods::addUser:
    {
        QMessageBox::information(this, "Успешно", "Студент добавлен в БД.");
    }
    break;
    case Methods::selectUsers:
    {
        updateTable(jsonData);
    }
    break;
    case Methods::selectAll:
    {
        updateTable(jsonData);
    }
    break;
    case Methods::deleteUser:
    {
        QMessageBox::information(this, "Успешно", "Студент удален из БД.");
    }
    break;
    case Methods::serverError:
    {
        QMessageBox::critical(this, "Ошибка!", "Проблемы на сервере.\nСервер не может ответить на ваш запрос.");
    }
    break;
    }
}

void MainWindow::on_buttonCheckConnection_clicked() {
    QByteArray message;
    QDataStream ds(&message, QIODevice::WriteOnly);

    ds << (qint16)0 << Methods::checkConnection;
    ds.device()->seek(0);
    ds << (qint16)(message.size() - sizeof(qint16));

    if (socket->state() == QAbstractSocket::UnconnectedState) {
        QHostAddress host(QHostAddress::LocalHost);
        socket->connectToHost(host, 6000);
        if (socket->waitForConnected()) {
            socket->write(message);
        } else {
            QMessageBox::critical(this, "Ошибка!", "В данный момент сервер не работает.");
        }
    } else {
        socket->write(message);
    }
}

void MainWindow::onConnected() {
    qDebug() <<"connected";
}

void MainWindow::on_buttonSelectAllFromDB_clicked() {
    QByteArray message;
    QDataStream ds(&message, QIODevice::WriteOnly);

    ds << (qint16)0 << Methods::selectAll;
    ds.device()->seek(0);
    ds << (qint16)(message.size() - sizeof(qint16));
    socket->write(message);
}


void MainWindow::on_buttonAddUser_clicked() {
    QString name = ui->leName->text();
    QString surname = ui->leSurname->text();
    QString fathername = ui->leFathername->text();
    QString group_number = ui->leGroupNumber->text();
    //all of them have to be filled
    if (name.isEmpty() && surname.isEmpty() && fathername.isEmpty() && group_number.isEmpty()) {
        QMessageBox::critical(this, "Ошибка!", "Чтобы добавить студента заполните все поля.");
        return;
    }

    QByteArray message;
    QDataStream ds(&message, QIODevice::WriteOnly);

    ds << (qint16)0 << Methods::addUser << dataToJson();
    ds.device()->seek(0);
    ds << (qint16)(message.size() - sizeof(qint16));
    socket->write(message);
}


void MainWindow::on_buttonSearchInDB_clicked() {
    QByteArray message;
    QDataStream ds(&message, QIODevice::WriteOnly);

    ds << (qint16)0 << Methods::selectUsers << dataToJson();
    ds.device()->seek(0);
    ds << (qint16)(message.size() - sizeof(qint16));
    socket->write(message);
}


void MainWindow::on_buttonDeleteByID_clicked() {
    QString id = ui->leID->text();
    if (id.isEmpty()) {
        QMessageBox::critical(this, "Ошибка!", "Чтобы удалить студента введите его id.");
        return;
    }

    QJsonDocument json;
    QJsonObject idKey;
    idKey.insert("id", QJsonValue(id));
    json.setObject(idKey);

    QByteArray message;
    QDataStream ds(&message, QIODevice::WriteOnly);

    ds << (qint16)0 << Methods::deleteUser << json.toJson();
    ds.device()->seek(0);
    ds << (qint16)(message.size() - sizeof(qint16));
    socket->write(message);
}

