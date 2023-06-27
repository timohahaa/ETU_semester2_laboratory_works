#include "Database.h"
#include <iostream>

Database::Database() {
    this->db = QSqlDatabase::addDatabase("QPSQL", "mydb");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("laba6");
    db.setUserName("timabd");
    db.setPassword("secure_password228"); //very secure indeed

    if (!db.open()) {
        qDebug() << db.lastError().text();
    }
}


QByteArray Database::queryToJSON(QString &sqlQuery) {
    QSqlQuery query(db);
    if (!query.exec(sqlQuery)) {
        std::cout << "DB error: " << query.lastError().text().toStdString() << std::endl;
        return QByteArray();
    }

    QJsonDocument json;
    QJsonArray usersArray;
    while (query.next()) {
        QJsonObject user;
        QSqlRecord row = query.record();
        for (int i = 0; i < row.count(); i++) {
            user.insert(row.fieldName(i), QJsonValue::fromVariant(row.value(i)));
        }
        usersArray.push_back(user);
    }
    json.setArray(usersArray);

    return json.toJson();
}


int Database::addUserToDB(QString name, QString surname, QString fathername, QString gender, QString group_number) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (name, surname, fathername, gender, group_number) VALUES (:name, :surname, :fathername, :gender, :group_number)");
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":fathername", fathername);
    query.bindValue(":gender", gender);
    query.bindValue(":group_number", group_number);
    if (!query.exec()) {
        std::cout << "DB error: " << query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    return 0;
}

QByteArray Database::selectFromDB(QString name, QString surname, QString fathername, QString gender, QString group_number) {
    QString sqlQuery("SELECT * FROM users WHERE");
    if (!name.isEmpty()) {
        sqlQuery += " name = \'" + name + "\' and";
    }
    if (!surname.isEmpty()) {
        sqlQuery += " surname = \'" + surname + "\' and";
    }
    if (!fathername.isEmpty()) {
        sqlQuery += " fathername = \'" + fathername + "\' and";
    }
    if (!gender.isEmpty()) {
        sqlQuery += " gender = \'" + gender + "\' and";
    }
    if (!group_number.isEmpty()) {
        sqlQuery += " group_number = " + group_number + " and";
    }
    //remove last and
    sqlQuery.chop(4);
    return queryToJSON(sqlQuery);
}

QByteArray Database::selectAllFromDB() {
    QString queryAll("SELECT * FROM users");
    return queryToJSON(queryAll);
}

//as a security measurnment, you can delete users only by id
int Database::deleteUserFromDB(QString id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        std::cout << "DB error: " << query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    return 0;
}
