#include <iostream>

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
//#include <QObject>
#include "Database.h"
#include "ApiMethods.h"

class Server : public QObject {

    Q_OBJECT

private:
    QTcpServer *tcpServer;
    Database *DB;
    QByteArray computeServerAnswer(qint8 method, QByteArray& jsonData);

private slots:
    void socketReadyRead();
    void socketConnected();
    void socketDisconected();
    void processConnection();

signals:
    void newSocket(QTcpSocket*);

public:
    Server(QObject *parent = nullptr);

};

#endif // SERVER_H
