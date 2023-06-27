#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRegularExpressionValidator>
#include "core_functions.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QRegularExpression regexp("[0-9]*");
    QValidator *validator = new QRegularExpressionValidator(regexp, this);
    ui->student_group->setValidator(validator);
    ui->group_number->setValidator(validator);
    ui->faculty_id->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void GroupToTable(Group* gr, Ui::MainWindow *ui) {
    ui->table->setRowCount(gr->student_count);
    ui->table->setColumnCount(5);
    ui->table->setHorizontalHeaderLabels(QStringList() << "Имя" << "Фамилия" << "Отчество" << "Пол" << "Тип обучения");
    for (int i = 0; i < ui->table->rowCount(); i++) {
        QTableWidgetItem *name = new QTableWidgetItem(QString(gr->student_list[i].name));
        QTableWidgetItem *surname = new QTableWidgetItem(QString(gr->student_list[i].surname));
        QTableWidgetItem *fathername = new QTableWidgetItem(QString(gr->student_list[i].fathername));
        QTableWidgetItem *gender = new QTableWidgetItem(QString(gr->student_list[i].gender));
        QTableWidgetItem *education_type = new QTableWidgetItem(QString(gr->student_list[i].education_type));
        ui->table->setItem(i, 0, name);
        ui->table->setItem(i, 1, surname);
        ui->table->setItem(i, 2, fathername);
        ui->table->setItem(i, 3, gender);
        ui->table->setItem(i, 4, education_type);
    }
}

void DepartmentToTable(Department* dp, Ui::MainWindow *ui) {
    ui->table->setRowCount(dp->group_count);
    ui->table->setColumnCount(2);
    ui->table->setHorizontalHeaderLabels(QStringList() << "Номер группы" << "Количество студентов");
    for (int i = 0; i < ui->table->rowCount(); i++) {
        QTableWidgetItem *number = new QTableWidgetItem(QString::number(dp->group_list[i].group_number));
        QTableWidgetItem *student_count = new QTableWidgetItem(QString::number(dp->group_list[i].student_count));
        ui->table->setItem(i, 0, number);
        ui->table->setItem(i, 1, student_count);
    }
}

void FacultyToTable(Faculty* fac, Ui::MainWindow *ui) {
    ui->table->setRowCount(fac->department_count);
    ui->table->setColumnCount(2);
    ui->table->setHorizontalHeaderLabels(QStringList() << "Название кафедры" << "Количество групп");
    for (int i = 0; i < ui->table->rowCount(); i++) {
        QTableWidgetItem *dep_name = new QTableWidgetItem(QString(fac->department_list[i].department_name));
        QTableWidgetItem *group_count = new QTableWidgetItem(QString::number(fac->department_list[i].group_count));
        ui->table->setItem(i, 0, dep_name);
        ui->table->setItem(i, 1, group_count);
    }
}

void UniversityToTable(University* uni, Ui::MainWindow *ui) {
    ui->table->setRowCount(uni->faculty_count);
    ui->table->setColumnCount(3);
    ui->table->setHorizontalHeaderLabels(QStringList() << "Название факультета" << "id факультета" << "Количество кафедр");
    for (int i = 0; i < ui->table->rowCount(); i++) {
        QTableWidgetItem *fac_name = new QTableWidgetItem(QString(uni->faculty_list[i].faculty_name));
        QTableWidgetItem *fac_id = new QTableWidgetItem(QString::number(uni->faculty_list[i].faculty_id));
        QTableWidgetItem *dep_count = new QTableWidgetItem(QString::number(uni->faculty_list[i].department_count));
        ui->table->setItem(i, 0, fac_name);
        ui->table->setItem(i, 1, fac_id);
        ui->table->setItem(i, 2, dep_count);
    }
}


void MainWindow::on_button_university_info_clicked() {
    QString output;
    University* university = new University;
    memset(university, 0, sizeof(University));
    int ret = readStruct(university);
    ui->output_display->clear();
    if (ret == -1) {
        QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
        delete university;
        return;
    }
    if (ret == 1) {
        QMessageBox::critical(0, "Ошибка!", "Файл с данными пустой!\n"
                                               "Восстановите файл из backup-а.");
        delete university;
        return;
    }
    if (ret == 0) {
        output = QString::fromStdString(UniversityToStdString(university));
        if (output.isEmpty()) {
            QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                   "Проверьте целостность файла или восстановите его из backup-а.");
        } else {
            ui->output_display->append(output);
            UniversityToTable(university, ui);
        }
    }
    delete university;
}


void MainWindow::on_button_find_faculty_clicked() {
    QString fac_name = ui->faculty_name->text();
    QString fac_id = ui->faculty_id->text();
    QString output;

    int do_deletion = 0;
    if (ui->delete_faculty->isChecked()) {
        do_deletion = 1;
    }


    if (fac_name.isEmpty()) {
        QMessageBox::critical(this, "Ошибка!", "Введите полное название факультета без слова \"Факультет\"");
        return;
    }

    University* university = new University;
    memset(university, 0, sizeof(University));

    int ret = readStruct(university);
    ui->output_display->clear();
    if (ret == -1) {
        QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
        delete university;
        return;
    }
    if (ret == 1) {
        QMessageBox::critical(0, "Ошибка!", "Файл с данными пустой!\n"
                                               "Восстановите файл из backup-а.");
        delete university;
        return;
    }
    if (ret == 0) {
        /*
        if (fac_id.isEmpty()) {
            output = QString::fromStdString(FacultyToStdString(findFaculty(university, fac_name.toUtf8().data())));
        } else {
            output = QString::fromStdString(FacultyToStdString(findFaculty(university, fac_name.toUtf8().data(), fac_id.toInt())));
        }
        */
        output = QString::fromStdString(FacultyToStdString(findFaculty(university, fac_name.toUtf8().data(), fac_id.toInt())));
        if (output.isEmpty()) {
            ui->output_display->append("Нет такого факультета!");
        } else {
            if (do_deletion) {
                deleteFaculty(findFaculty(university, fac_name.toUtf8().data(), fac_id.toInt()), university);
                FILE* fp = openDatafile();
                if (fp == nullptr) {
                    QMessageBox::critical(0, "Ошибка!", "Не удается открыть файл для записи.\n"
                                                        "Проверьте целостность файла или восстановите его из backup-а.");
                    delete university;
                    return;
                }
                writeUniversity((char*)university, fp, university->faculty_list, university->faculty_count);
                fclose(fp);
                ui->output_display->append(QString("УДАЛЕНО\n"));
            }
            ui->output_display->append(output);
            FacultyToTable(findFaculty(university, fac_name.toUtf8().data(), fac_id.toInt()), ui);
        }
    }
    delete university;
}


void MainWindow::on_button_find_department_clicked() {
    QString dep_name = ui->department_name->text();
    QString fac_name = ui->department_faculty_name->text();
    QString output;

    int do_deletion = 0;
    if (ui->delete_department->isChecked()) {
        do_deletion = 1;
    }

    if (dep_name.isEmpty()) {
        QMessageBox::critical(this, "Ошибка!", "Введите полное название кафедры!");
        return;
    }

    University* university = new University;
    memset(university, 0, sizeof(University));

    int ret = readStruct(university);
    ui->output_display->clear();
    if (ret == -1) {
        QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
        delete university;
        return;
    }
    if (ret == 1) {
        QMessageBox::critical(0, "Ошибка!", "Файл с данными пустой!\n"
                                               "Восстановите файл из backup-а.");
        delete university;
        return;
    }
    if (ret == 0) {
        /*
        if (fac_name.isEmpty()) {
            output = QString::fromStdString(DepartmentToStdString(findDepartment(university, dep_name.toUtf8().data())));
        } else {
            output = QString::fromStdString(DepartmentToStdString(findDepartment(university, dep_name.toUtf8().data(), fac_name.toUtf8().data())));
        }
        */
        output = QString::fromStdString(DepartmentToStdString(findDepartment(university, dep_name.toUtf8().data(), fac_name.toUtf8().data())));
        if (output.isEmpty()) {
            ui->output_display->append("Нет такой кафедры!");
        } else {
            if (do_deletion) {
                deleteDepartment(findDepartment(university, dep_name.toUtf8().data(), fac_name.toUtf8().data()), university, fac_name.toUtf8().data());
                FILE* fp = openDatafile();
                if (fp == nullptr) {
                    QMessageBox::critical(0, "Ошибка!", "Не удается открыть файл для записи.\n"
                                                        "Проверьте целостность файла или восстановите его из backup-а.");
                    delete university;
                    return;
                }
                writeUniversity((char*)university, fp, university->faculty_list, university->faculty_count);
                fclose(fp);
                ui->output_display->append(QString("УДАЛЕНО\n"));
            }
            ui->output_display->append(output);
            DepartmentToTable(findDepartment(university, dep_name.toUtf8().data(), fac_name.toUtf8().data()), ui);
        }
    }
    delete university;
}

void MainWindow::on_button_find_group_clicked() {
    QString group_number = ui->group_number->text();
    QString dep_name = ui->group_department_name->text();
    QString output;

    int do_deletion = 0;
    if (ui->delete_group->isChecked()) {
        do_deletion = 1;
    }

    if (group_number.isEmpty()) {
        QMessageBox::critical(this, "Ошибка!", "Введите номер группы!");
        return;
    }

    University* university = new University;
    memset(university, 0, sizeof(University));

    int ret = readStruct(university);
    ui->output_display->clear();
    if (ret == -1) {
        QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
        delete university;
        return;
    }
    if (ret == 1) {
        QMessageBox::critical(0, "Ошибка!", "Файл с данными пустой!\n"
                                               "Восстановите файл из backup-а.");
        delete university;
        return;
    }
    if (ret == 0) {
        /*
        if (dep_name.isEmpty()) {
            output = QString::fromStdString(GroupToStdString(findGroup(university, group_number.toInt())));
        } else {
            output = QString::fromStdString(GroupToStdString(findGroup(university, group_number.toInt(), dep_name.toUtf8().data())));
        }
        */
        output = QString::fromStdString(GroupToStdString(findGroup(university, group_number.toInt(), dep_name.toUtf8().data())));
        if (output.isEmpty()) {
            ui->output_display->append("Нет такой группы!");
        } else {
            if (do_deletion) {
                deleteGroup(findGroup(university, group_number.toInt(), dep_name.toUtf8().data()), university, dep_name.toUtf8().data());
                FILE* fp = openDatafile();
                if (fp == nullptr) {
                    QMessageBox::critical(0, "Ошибка!", "Не удается открыть файл для записи.\n"
                                                        "Проверьте целостность файла или восстановите его из backup-а.");
                    delete university;
                    return;
                }
                writeUniversity((char*)university, fp, university->faculty_list, university->faculty_count);
                fclose(fp);
                ui->output_display->append(QString("УДАЛЕНО\n"));
            }
            ui->output_display->append(output);
            GroupToTable(findGroup(university, group_number.toInt(), dep_name.toUtf8().data()), ui);
        }
    }
    delete university;
}


void MainWindow::on_button_find_student_clicked() {
    QString name = ui->student_name->text();
    QString surname = ui->student_surname->text();
    QString fathername = ui->student_fathername->text();
    QString group = ui->student_group->text();
    QString output;

    int do_deletion = 0;
    if (ui->delete_student->isChecked()) {
        do_deletion = 1;
    }

    if (name.isEmpty()) {
        QMessageBox::critical(this, "Ошибка!", "Введите имя студента!");
        return;
    }
    else if (surname.isEmpty()) {
        QMessageBox::critical(this, "Ошибка!", "Введите фамилию студента!");
        return;
    }
    else if (fathername.isEmpty()) {
        QMessageBox::critical(this, "Ошибка!", "Введите отчество студента!");
        return;
    }

    University* university = new University;
    memset(university, 0, sizeof(University));

    int ret = readStruct(university);
    ui->output_display->clear();
    if (ret == -1) {
        QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
        delete university;
    }
    if (ret == 1) {
        QMessageBox::critical(0, "Ошибка!", "Файл с данными пустой!\n"
                                               "Восстановите файл из backup-а.");
        delete university;
    }
    if (ret == 0) {
        /*
        if (group.isEmpty()) {
            output = QString::fromStdString(StudentToStdString(findStudent(university, name.toUtf8().data(), surname.toUtf8().data(), fathername.toUtf8().data())));
        } else {
            output = QString::fromStdString(StudentToStdString(findStudent(university, name.toUtf8().data(), surname.toUtf8().data(), fathername.toUtf8().data(), group.toInt())));
        }
        */
        output = QString::fromStdString(StudentToStdString(findStudent(university, name.toUtf8().data(), surname.toUtf8().data(), fathername.toUtf8().data(), group.toInt())));
        if (output.isEmpty()) {
            ui->output_display->append("Нет такого студента!");
        } else {
            if (do_deletion) {
                deleteStudent(findStudent(university, name.toUtf8().data(), surname.toUtf8().data(), fathername.toUtf8().data(), group.toInt()), university, group.toInt());
                FILE* fp = openDatafile();
                if (fp == nullptr) {
                    QMessageBox::critical(0, "Ошибка!", "Не удается открыть файл для записи.\n"
                                                        "Проверьте целостность файла или восстановите его из backup-а.");
                    delete university;
                    return;
                }
                writeUniversity((char*)university, fp, university->faculty_list, university->faculty_count);
                fclose(fp);
                ui->output_display->append(QString("УДАЛЕНО\n"));
            }
            ui->output_display->append(output);
        }
    }
    delete university;
}


void MainWindow::on_button_add_faculty_clicked() {
    QString head_name = ui->faculty_head_name->text();
    QString head_surname = ui->faculty_head_surname->text();
    QString head_fathername = ui->faculty_head_fathername->text();
    QString fac_name = ui->faculty_name_add->text();
    QString fac_id = ui->faculty_id_add->text();

    if (fac_name.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите название факультета!");
        return;
    } else if (fac_id.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите id факультета!");
        return;
    } else if (head_name.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите имя декана!");
        return;
    } else if (head_surname.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите фамилию декана!");
        return;
    } else if (head_fathername.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите отчество декана!");
        return;
    }

    University* university = new University;
    memset(university, 0, sizeof(University));

    int ret = readStruct(university);

    if (ret == -1) {
        QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
        delete university;
        return;
    }
    if (ret == 1) {
        QMessageBox::critical(0, "Ошибка!", "Файл с данными пустой!\n"
                                               "Восстановите файл из backup-а.");
        delete university;
        return;
    }
    if (ret == 0) {
        if (findFaculty(university, "", fac_id.toInt()) != nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Факультет с таким id уже существует");
            delete university;
            return;
        }
        if (findFaculty(university, fac_name.toUtf8().data()) != nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Факультет с таким названием уже существует");
            delete university;
            return;
        }
        FILE* fp = openDatafile();
        if (fp == nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Не удается открыть файл для записи.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
            delete university;
            return;
        }
        addFaculty(head_name.toUtf8().data(), head_surname.toUtf8().data(), head_fathername.toUtf8().data(), fac_name.toUtf8().data(), fac_id.toInt(), university);
        writeUniversity((char*)university, fp, university->faculty_list, university->faculty_count);
        fclose(fp);
    }
    delete university;
}


void MainWindow::on_button_add_department_clicked() {
    QString head_name = ui->department_head_name->text();
    QString head_surname = ui->department_head_surname->text();
    QString head_fathername = ui->department_head_fathername->text();
    QString dep_name = ui->department_name_add->text();
    QString fac_name = ui->department_faculty_name_add->text();

    if (dep_name.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите название кафедры!");
        return;
    } else if (fac_name.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите название факультета!");
        return;
    } else if (head_name.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите имя заведующего кафедрой!");
        return;
    } else if (head_surname.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите фамилию заведующего кафедрой!");
        return;
    } else if (head_fathername.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите отчество заведующего кафедрой!");
        return;
    }

    University* university = new University;
    memset(university, 0, sizeof(University));

    int ret = readStruct(university);

    if (ret == -1) {
        QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
        delete university;
        return;
    }
    if (ret == 1) {
        QMessageBox::critical(0, "Ошибка!", "Файл с данными пустой!\n"
                                               "Восстановите файл из backup-а.");
        delete university;
        return;
    }
    if (ret == 0) {
        Faculty* fac = findFaculty(university, fac_name.toUtf8().data());
        if (findDepartment(university, dep_name.toUtf8().data()) != nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Кафедра с таким именем уже существует");
            delete university;
            return;
        }
        if (fac == nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Факультета с таким именем не существует!");
            delete university;
            return;
        }
        FILE* fp = openDatafile();
        if (fp == nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Не удается открыть файл для записи.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
            delete university;
            return;
        }
        addDepartment(head_name.toUtf8().data(), head_surname.toUtf8().data(), head_fathername.toUtf8().data(), dep_name.toUtf8().data(), fac, university);
        writeUniversity((char*)university, fp, university->faculty_list, university->faculty_count);
        fclose(fp);
    }
    delete university;
}


void MainWindow::on_button_add_group_clicked() {
    QString group_number = ui->group_number_add->text();
    QString dep_name = ui->group_department_name_add->text();

    if (group_number.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите номер группы!");
        return;
    } else if (dep_name.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите название кафедры!");
        return;
    }

    University* university = new University;
    memset(university, 0, sizeof(University));

    int ret = readStruct(university);

    if (ret == -1) {
        QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
        delete university;
        return;
    }
    if (ret == 1) {
        QMessageBox::critical(0, "Ошибка!", "Файл с данными пустой!\n"
                                               "Восстановите файл из backup-а.");
        delete university;
        return;
    }
    if (ret == 0) {
        Department* dp = findDepartment(university, dep_name.toUtf8().data());
        if (findGroup(university, group_number.toInt()) != nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Такая группа уже существует!");
            delete university;
            return;
        }
        if (dp == nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Кафедры с таким именем не существует!");
            delete university;
            return;
        }
        FILE* fp = openDatafile();
        if (fp == nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Не удается открыть файл для записи.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
            delete university;
            return;
        }
        addGroup(group_number.toInt(), dp, university);
        writeUniversity((char*)university, fp, university->faculty_list, university->faculty_count);
        fclose(fp);
    }
    delete university;
}


void MainWindow::on_button_add_student_clicked() {
    QString name = ui->student_name_add->text();
    QString surname = ui->student_surname_add->text();
    QString fathername = ui->student_fathername_add->text();
    QString group_number = ui->student_group_add->text();
    QString gender;
    QString education_type;
    int make_student_head_student = 0;

    if (ui->gender_male->isChecked()) {
        gender = "М";
    } else if (ui->gender_female->isChecked()) {
        gender = "Ж";
    } else {
        gender = "";
    }

    if(ui->education_free->isChecked()) {
        education_type = "Бюджет";
    } else if(ui->education_paid->isChecked()) {
        education_type = "Контракт";
    } else if(ui->education_special->isChecked()) {
        education_type = "Целевое";
    } else {
        education_type = "";
    }

    if (ui->is_head_student_checker->isChecked()) {
        make_student_head_student = 1;
    }

    if (name.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите имя студента!");
        return;
    } else if (surname.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите фамилию студента!");
        return;
    } else if (fathername.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите отчество студента!");
        return;
    } else if (group_number.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Введите номер группы!");
        return;
    } else if (gender.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Выберите пол!");
        return;
    } else if (education_type.isEmpty()) {
        QMessageBox::critical(0, "Ошибка!", "Выберите тип обучения!");
        return;
    }

    University* university = new University;
    memset(university, 0, sizeof(University));

    int ret = readStruct(university);

    if (ret == -1) {
        QMessageBox::critical(0, "Ошибка!", "Не удалось открыть файл с данными.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
        delete university;
        return;
    }
    if (ret == 1) {
        QMessageBox::critical(0, "Ошибка!", "Файл с данными пустой!\n"
                                               "Восстановите файл из backup-а.");
        delete university;
        return;
    }
    if (ret == 0) {
        Group* gr = findGroup(university, group_number.toInt());
        if (findStudent(university, name.toUtf8().data(), surname.toUtf8().data(), fathername.toUtf8().data()) != nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Такой студент уже существует!");
            delete university;
            return;
        }
        if (gr == nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Группы с таким номером не существует!");
            delete university;
            return;
        }
        FILE* fp = openDatafile();
        if (fp == nullptr) {
            QMessageBox::critical(0, "Ошибка!", "Не удается открыть файл для записи.\n"
                                                "Проверьте целостность файла или восстановите его из backup-а.");
            delete university;
            return;
        }
        addStudent(name.toUtf8().data(), surname.toUtf8().data(), fathername.toUtf8().data(), gender.toUtf8().data(), education_type.toUtf8().data(), gr, university, make_student_head_student);
        writeUniversity((char*)university, fp, university->faculty_list, university->faculty_count);
        fclose(fp);
    }
    delete university;
}

