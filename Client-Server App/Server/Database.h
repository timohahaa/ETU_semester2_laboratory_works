#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


class Database {
private:
    QSqlDatabase db;
    QByteArray queryToJSON(QString &sqlQuery);

public:
    Database();

    int addUserToDB(QString name, QString surname, QString fathername, QString gender, QString group_number);
    QByteArray selectFromDB(QString name, QString surname, QString fathername, QString gender, QString group_number);
    QByteArray selectAllFromDB();
    //as a security measurnment, you can delete users only by id
    int deleteUserFromDB(QString id);
};

#endif // DATABASE_H
