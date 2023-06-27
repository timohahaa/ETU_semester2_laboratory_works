#include <QtCore>
#ifndef APIMETHODS_H
#define APIMETHODS_H

enum Methods: qint8 {
    checkConnection = 8,
    addUser,
    selectUsers,
    selectAll,
    deleteUser,
    serverError
};

#endif // APIMETHODS_H
