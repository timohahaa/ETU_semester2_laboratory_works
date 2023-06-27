#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_button_find_faculty_clicked();

    void on_button_find_department_clicked();

    void on_button_university_info_clicked();

    void on_button_find_group_clicked();

    void on_button_find_student_clicked();

    void on_button_add_faculty_clicked();

    void on_button_add_department_clicked();

    void on_button_add_group_clicked();

    void on_button_add_student_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
