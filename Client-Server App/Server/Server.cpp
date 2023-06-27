#include "Server.h"

Server::Server(QObject *parent) : QObject(parent) {
    this->tcpServer = new QTcpServer();
    this->DB = new Database();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(processConnection()));
    if(!tcpServer->listen(QHostAddress::LocalHost, 6000)){
        std::cout << "Server can't start on port 6000, exiting the programm..." << std::endl;
        QCoreApplication::exit(1);
    } else {
        std::cout << "Server is up on port 6000" << std::endl;
    }
}

QByteArray Server::computeServerAnswer(qint8 method, QByteArray &jsonData) {
    QByteArray answer;
    QDataStream outputStream(&answer, QIODevice::WriteOnly);
    switch (method) {
    case Methods::checkConnection:
    {
        //basicaly an echo answer
        //leave space for the size of the block
        outputStream << (qint16)0 << Methods::checkConnection;
        //write the size of the block
        outputStream.device()->seek(0);
        outputStream << (qint16)(answer.size() - sizeof(qint16));
    }
    break;
    case Methods::addUser:
    {
        //add the user
        //json will hold one object - {name:, surname:, fathername:, gender:, group_number:}
        QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonData);
        QJsonObject user = jsonResponse.object();
        QString name = user.value("name").toVariant().toString();
        QString surname = user.value("surname").toVariant().toString();
        QString fathername = user.value("fathername").toVariant().toString();
        QString gender = user.value("gender").toVariant().toString();
        QString group_number = user.value("group_number").toVariant().toString();
        int ret = DB->addUserToDB(name, surname, fathername, gender, group_number);
        //the answer
        if (ret == -1) {
            outputStream << (qint16)0 << Methods::serverError;
        } else {
            outputStream << (qint16)0 << Methods::addUser;
        }
        outputStream.device()->seek(0);
        outputStream << (qint16)(answer.size() - sizeof(qint16));
    }
    break;
    case Methods::selectUsers:
    {
        //select users
        //json will hold one object - {name:, surname:, fathername:, gender:, group_number:}
        QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonData);
        QJsonObject user = jsonResponse.object();
        QString name = user.value("name").toVariant().toString();
        QString surname = user.value("surname").toVariant().toString();
        QString fathername = user.value("fathername").toVariant().toString();
        QString gender = user.value("gender").toVariant().toString();
        QString group_number = user.value("group_number").toVariant().toString();
        QByteArray users = DB->selectFromDB(name, surname, fathername, gender, group_number);
        //the answer
        outputStream << (qint16)0 << Methods::selectUsers << users;
        outputStream.device()->seek(0);
        outputStream << (qint16)(answer.size() - sizeof(qint16));
    }
    break;
    case Methods::selectAll:
    {
        //select all users
        QByteArray users = DB->selectAllFromDB();
        //the answer
        outputStream << (qint16)0 << Methods::selectAll << users;
        outputStream.device()->seek(0);
        outputStream << (qint16)(answer.size() - sizeof(qint16));
    }
    break;
    case Methods::deleteUser:
    {
        //delete the user
        //json will hold one object - {id:}
        QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonData);
        QJsonObject responseObject = jsonResponse.object();
        QString id = responseObject.value("id").toVariant().toString();
        int ret = DB->deleteUserFromDB(id);
        //the answer
        if (ret == -1) {
            outputStream << (qint16)0 << Methods::serverError;
        } else {
            outputStream << (qint16)0 << Methods::deleteUser;
        }
        outputStream.device()->seek(0);
        outputStream << (qint16)(answer.size() - sizeof(qint16));
    }
    break;
    }
    return answer;
}

void Server::socketReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    QDataStream inputStream(socket);
    std::string peerAdress = socket->peerAddress().toString().toStdString();
    std::string peerPort = QString::number(socket->peerPort()).toStdString();
    std::cout<< "reading from " << peerAdress << " on peer port " << peerPort <<std::endl;
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
    //read the method to use - what the server needs to do
    qint8 method;
    inputStream >> method;
    //read rest of the data - can be none (depends on the method)
    QByteArray jsonData;
    inputStream >> jsonData;

    QByteArray answer = computeServerAnswer(method, jsonData);
    socket->write(answer);
}

void Server::socketConnected(){
    std::cout << "connected" << std::endl;
}

void Server::socketDisconected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    std::string peerAdress = socket->peerAddress().toString().toStdString();
    std::string peerPort = QString::number(socket->peerPort()).toStdString();
    std::cout<< "disconected from " << peerAdress << " on peer port " << peerPort <<std::endl;
}

void Server::processConnection() {
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    if (socket->state() == QAbstractSocket::ConnectedState) {
        std::string peerAdress = socket->peerAddress().toString().toStdString();
        std::string peerPort = QString::number(socket->peerPort()).toStdString();
        std::cout<< "connected to " << peerAdress << " on peer port " << peerPort <<std::endl;
    }
    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconected()));

}
