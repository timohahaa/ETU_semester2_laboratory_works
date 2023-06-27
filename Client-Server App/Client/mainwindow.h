
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

private slots:
    void socketReadyRead();
    void onConnected();
    void on_buttonCheckConnection_clicked();

    void on_buttonSelectAllFromDB_clicked();

    void on_buttonAddUser_clicked();

    void on_buttonSearchInDB_clicked();

    void on_buttonDeleteByID_clicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;

    QByteArray dataToJson();
    void processAnswer(qint8 method, QByteArray jsonData);
    void updateTable(QByteArray &jsonData);
};

#endif // MAINWINDOW_H
