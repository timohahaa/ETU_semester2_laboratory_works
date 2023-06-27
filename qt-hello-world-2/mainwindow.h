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
protected:
    void timerEvent(QTimerEvent *event);


private slots:
    void on_button_next_clicked();

    void on_button_add_row_clicked();

    void on_button_sort_clicked();

    void on_button_delete_row_clicked();

    void on_button_export_to_excel_clicked();

    void on_button_show_random_image_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
