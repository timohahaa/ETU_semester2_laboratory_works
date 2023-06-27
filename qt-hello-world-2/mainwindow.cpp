#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QFileDialog>
#include <QDebug>
#include <QAxObject>
#include <QRandomGenerator>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startTimer(1000);
    //make tabs, that a user cant access right away, invisible, make them visible later, when a user can access them
    ui->tabWidget->setTabVisible(1, 0);
    ui->tabWidget->setTabVisible(2, 0);
    ui->tabWidget->setTabVisible(3, 0);

    QValidator *validator = new QRegularExpressionValidator(QRegularExpression("[ЁёА-я\- ]*"), this);
    ui->name_input->setValidator(validator);
    ui->table->setColumnCount(2);
    ui->table->setHorizontalHeaderLabels(QStringList() << "Предмет" << "Оценка");
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    this->setFixedSize(QSize(800, 600));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event) {
    ui->time->setText("Время: " + QTime::currentTime().toString("hh:mm:ss"));
}

void MainWindow::on_button_next_clicked() {
    QString name = ui->name_input->text();

    if (name.isEmpty()) {
        QMessageBox::critical(this, "Ошибка!", "Вы не заполнили поле \"ФИО\".\nЗаполните и попробуйте снова.");
    } else {
        //make the tabs visible again
        ui->tabWidget->setTabVisible(1, 1);
        ui->tabWidget->setTabVisible(2, 1);
        ui->tabWidget->setTabVisible(3, 1);
        ui->name_input->setReadOnly(true);
    }
}

void MainWindow::on_button_add_row_clicked() {
    if (ui->table->rowCount() == 25) {
        QMessageBox::warning(this, "Предупреждение!", "Настоятельно не рекомендуем загружать студента более чем 25-ю дисциплинами.\n"
                                                                    "Пожалейте его!");
        return;
    }
    ui->table->insertRow(ui->table->rowCount());
    ui->table->setItem(ui->table->rowCount()-1, 0, new QTableWidgetItem(" "));
    ui->table->setItem(ui->table->rowCount()-1, 1, new QTableWidgetItem(" "));
}

void MainWindow::on_button_sort_clicked() {
    if (ui->sorting_method->currentText() == "Оценка") {
        ui->table->sortByColumn(1, Qt::DescendingOrder);
    } else {
        ui->table->sortByColumn(0, Qt::AscendingOrder);
    }
}

void MainWindow::on_button_delete_row_clicked() {
    int row = ui->table->currentRow();
    ui->table->removeRow(row);
}

void MainWindow::on_button_export_to_excel_clicked() {
    //set empty cells to " " so program wont crash
    for (int i = 0; i < ui->table->rowCount(); i++) {
        if (ui->table->item(i, 0)->text().isEmpty()) {
            QTableWidgetItem *temp = new QTableWidgetItem(" ");
            ui->table->setItem(i, 0, temp);
        }
        if (ui->table->item(i, 1)->text().isEmpty()) {
            QTableWidgetItem *temp = new QTableWidgetItem(" ");
            ui->table->setItem(i, 1, temp);
        }
    }

    QString name = ui->name_input->text().replace(" ", "_") + ui->group->currentText();
    QString filter = "(*.xlsx)";
    QString filepath = QFileDialog::getSaveFileName(this, "Выберите, куда сохранить Excel файл", "C:\\Users\\timab\\OneDrive\\Рабочий стол\\" + name, filter, &filter);
    if (!filepath.endsWith(".xlsx")) {
        QMessageBox::critical(this, "Ошибка!", "Вы указали не то расширение файла.\nФайл для сохранения таблицы должен иметь тип \".xlsx\".");
        return;
    }

    int rowCount = ui->table->rowCount();
    QAxObject *excel = new QAxObject(this);
    excel->setControl("Excel.Application"); // Connect Excel control
    excel->dynamicCall("SetVisible (bool Visible)", "false");// Do not display the form
    excel->setProperty("DisplayAlerts", false);// Do not display any

    QAxObject *workbooks = excel->querySubObject("WorkBooks"); // Get a workbook collection
    workbooks->dynamicCall("Add");// Create a new workbook
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook"); // Get the current workbook
    QAxObject *worksheets = workbook->querySubObject("Sheets");// Get a collection of worksheets
    QAxObject *worksheet = worksheets->querySubObject("Item(int)", 1); // Get a worksheet 1 for worksheets 1, ie Sheet1

    QAxObject *cellA, *cellB, *anyCell;
    QString columnA = "A1";
    QString columnB = "B1";
    cellA = worksheet->querySubObject("Range(QVariant, QVariant)", columnA);// Get cell
    cellB = worksheet->querySubObject("Range(QVariant, QVariant)", columnB);

    //set name, group and gender
    anyCell = worksheet->querySubObject("Range(QVariant, QVariant)", QString("C1"));
    anyCell->dynamicCall("SetValue(const QVariant&)", QVariant("ФИО"));
    anyCell = worksheet->querySubObject("Range(QVariant, QVariant)", QString("C2"));
    anyCell->dynamicCall("SetValue(const QVariant&)", QVariant(ui->name_input->text()));
    anyCell = worksheet->querySubObject("Range(QVariant, QVariant)", QString("D1"));
    anyCell->dynamicCall("SetValue(const QVariant&)", QVariant("Группа"));
    anyCell = worksheet->querySubObject("Range(QVariant, QVariant)", QString("D2"));
    anyCell->dynamicCall("SetValue(const QVariant&)", QVariant(ui->group->currentText()));
    anyCell = worksheet->querySubObject("Range(QVariant, QVariant)", QString("E1"));
    anyCell->dynamicCall("SetValue(const QVariant&)", QVariant("Пол"));
    anyCell = worksheet->querySubObject("Range(QVariant, QVariant)", QString("E2"));
    QString gender = (ui->gender_male->isChecked()) ? ("М"):("Ж");
    anyCell->dynamicCall("SetValue(const QVariant&)", QVariant(gender));

    //set column titles
    cellA->dynamicCall("SetValue(const QVariant&)", QVariant(ui->table->horizontalHeaderItem(0)->text())); // Set the value of the cell
    cellB->dynamicCall("SetValue(const QVariant&)", QVariant(ui->table->horizontalHeaderItem(1)->text()));
    //fill the columns
    for (int i = 0; i < rowCount; i++) {
        columnA = "A" + QString::number(i + 2);
        columnB = "B" + QString::number(i + 2);
        //get cells
        cellA = worksheet->querySubObject("Range(QVariant, QVariant)", columnA);// Get cell
        cellB = worksheet->querySubObject("Range(QVariant, QVariant)", columnB);
        //write to the cells
        qDebug() <<ui->table->item(i, 0)->text();
        qDebug() <<ui->table->item(i, 1)->text();
        cellA->dynamicCall("SetValue(const QVariant&)", QVariant(ui->table->item(i, 0)->text()));
        cellB->dynamicCall("SetValue(const QVariant&)", QVariant(ui->table->item(i, 1)->text()));
    }


    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filepath)); //Save to filepath
    workbook->dynamicCall("Close()");// Close the workbook
    excel->dynamicCall("Quit()");// Close Excel
    delete excel;
    ui->name_input->setReadOnly(false);
    ui->tabWidget->setTabVisible(1, 0);
    ui->tabWidget->setTabVisible(2, 0);
    ui->tabWidget->setTabVisible(3, 0);
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_button_show_random_image_clicked() {
    int num = QRandomGenerator::global()->bounded(1, 10);
    qDebug() << num;
    QPixmap image(":/pictures/pic" + QString::number(num) + ".jpg");
    if (num == 3 || num == 2) {
        ui->image_label->setPixmap(image.scaledToHeight(250));
    } else {
         ui->image_label->setPixmap(image.scaledToHeight(500));
    }
}

