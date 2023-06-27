#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ok_button_clicked() {
    QString FIO = ui->FIO_input->text();
    ui->textBrowser->clear();
    QString input;
    QTextStream ts(&input);
    ts << "Привет, " << FIO << "!\nРад тебя видеть! :)!\n"
       <<"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
         "⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣶⠾⠛⠛⠋⠉⠛⠛⠷⢶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
         "⠀⠀⠀⠀⠀⠀⣠⣾⠟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣷⣄⠀⠀⠀⠀⠀⠀\n"
         "⠀⠀⠀⠀⠀⣴⠟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣧⠀⠀⠀⠀⠀\n"
         "⠀⠀⠀⠀⣸⡏⠀⠀⠀⠀⣴⣶⡆⠀⠀⠀⠀⢠⣶⣦⠀⠀⠀⠀⢹⣇⠀⠀⠀⠀\n"
         "⠀⠀⠀⢠⣿⠀⠀⠀⠀⠀⠛⠟⠃⠀⠀⠀⠀⠘⠻⠛⠀⠀⠀⠀⠀⣿⡄⠀⠀⠀\n"
         "⠀⠀⠀⢸⣿⠀⠀⠀⠀⢠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡄⠀⠀⠀⠀⢸⡇⠀⠀⠀\n"
         "⠀⠀⠀⠘⣿⠀⠀⠀⠀⠸⣧⠀⠀⠀⠀⠀⠀⠀⠀⣼⡏⠀⠀⠀⠀⣿⠇⠀⠀⠀\n"
         "⠀⠀⠀⠀⢻⣇⠀⠀⠀⠀⠙⢷⣦⣀⡀⠀⣀⣠⡾⠋⠀⠀⠀⠀⢰⡟⠀⠀⠀⠀\n"
         "⠀⠀⠀⠀⠀⢻⣦⠀⠀⠀⠀⠀⠈⠉⠙⠋⠉⠉⠀⠀⠀⠀⠀⣰⡟⠁⠀⠀⠀⠀\n"
         "⠀⠀⠀⠀⠀⠀⠙⢷⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⠏⠀⠀⠀⠀⠀⠀\n"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⢶⣤⣄⣀⣀⣀⣀⣠⣤⡶⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀\n"
         "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀";
    ui->textBrowser->append(input);
}

