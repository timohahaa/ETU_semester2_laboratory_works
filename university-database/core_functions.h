#ifndef CORE_FUNCTIONS_H
#define CORE_FUNCTIONS_H

#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <windows.h>
#include <QMessageBox>

//structure size is so large, because cyrillic symbols take 2 bytes in UTF-8
//#define is for actual structure size, comment under each struct is its size, if 1-byte symbols only are used
//basicaly cut in half every arrays size (except gender in Student)
//decided to convert to UTF-8 because that makes QT way easier to use
struct Student {
    char name[40];
    char surname[40];
    char fathername[40];
    char gender[4]; //no need to cut the size in half
    char education_type[24]; //paid or unpaid
}; //76 bytes
#define SIZE_Student 148

struct Group {
    int group_number;
    int student_count;
    Student head_student;
    Student* student_list;
};//84 bytes
#define SIZE_Group 156

struct Department {
    char department_name[128];
    char dep_head_name[40];
    char dep_head_surname[40];
    char dep_head_fathername[40];
    int group_count;
    Group* group_list;
}; //128 bytes
#define SIZE_Department 252

struct Faculty {
    char faculty_name[128];
    char fac_head_name[40];
    char fac_head_surname[40];
    char fac_head_fathername[40];
    int faculty_id;
    int department_count;
    Department* department_list;
}; //132 bytes
#define SIZE_Faculty 256

struct University {
    char university_name[200];
    char uni_head_name[40];
    char uni_head_surname[40];
    char uni_head_fathername[40];
    int faculty_count;
    Faculty* faculty_list;
}; //164 bytes
#define SIZE_University 324

const char datafile[] = "C:\\Users\\timab\\CLionProjects\\SinevVE_semester2\\laba3\\data";
const char testfile[] = "C:\\Users\\timab\\CLionProjects\\SinevVE_semester2\\laba3\\test";

#define NULL_INT (-1)
//none of the integer structure fields can be -1


void readStudent(Student* buf, FILE* fp) {
    int ret = fread((char*)buf, 1, SIZE_Student, fp);
    if (ret != SIZE_Student) {
        //cout<<"\nerror while reading student " << ret<<endl;
        QMessageBox::critical(0, "Ошибка!", "Не удалось прочитать информацию о студенте.\n"
                                               "Проверьте целостность файла или восстановите его из backup-а.");
    }
}

void readGroup(Group* buf, FILE* fp) {
    int ret = fread((char*)buf, 1, SIZE_Group, fp);
    if (ret != SIZE_Group) {
        //cout<<"\nerror while reading group " << ret<<endl;
        QMessageBox::critical(0, "Ошибка!", "Не удалось прочитать информацию о группе.\n"
                                               "Проверьте целостность файла или восстановите его из backup-а.");
        return;
    }
    buf->student_list = new Student[buf->student_count + 1]; //+1 because ill might need to add another student to the list
    //its ugly, but id rather sacrifise memory, than worry about malloc, realloc or rewriring everything in terms of vectors
    //it is absolytely ok to do what i did, because I am just doing a little uni-homework, but I understand, that doing it on a real project would have been TERRIBLE
    for (int i = 0; i < buf->student_count; i++) {
        readStudent(&buf->student_list[i], fp);
    }
}

void readDepartment(Department* buf, FILE* fp) {
    int ret = fread((char*)buf, 1, SIZE_Department, fp);
    if (ret != SIZE_Department) {
        //cout<<"\nerror while reading department " << ret<<endl;
        QMessageBox::critical(0, "Ошибка!", "Не удалось прочитать информацию о кафедре.\n"
                                               "Проверьте целостность файла или восстановите его из backup-а.");
        return;
    }
    buf->group_list = new Group[buf->group_count + 1];
    for (int i = 0; i < buf->group_count; i++) {
        readGroup(&buf->group_list[i], fp);
    }
}

void readFaculty(Faculty* buf, FILE* fp) {
    int ret = fread((char*)buf, 1, SIZE_Faculty, fp);
    if (ret != SIZE_Faculty) {
        //cout<<"\nerror while reading faculty " << ret<<endl;
        QMessageBox::critical(0, "Ошибка!", "Не удалось прочитать информацию о факультете.\n"
                                               "Проверьте целостность файла или восстановите его из backup-а.");
        return;
    }
    buf->department_list = new Department[buf->department_count + 1];
    for (int i = 0; i < buf->department_count; i++) {
        readDepartment(&buf->department_list[i], fp);
    }
}

void readUniversity(University* buf, FILE* fp) {
    int ret = fread((char*)buf, 1, SIZE_University, fp);
    if (ret != SIZE_University) {
        //cout<<"\nerror while reading university " << ret<<endl;
        QMessageBox::critical(0, "Ошибка!", "Не удалось прочитать информацию об университете.\n"
                                               "Проверьте целостность файла или восстановите его из backup-а.");
        return;
    }
    buf->faculty_list = new Faculty[buf->faculty_count + 1];
    for (int i = 0; i < buf->faculty_count; i++) {
        readFaculty(&buf->faculty_list[i], fp);
    }
}

Faculty* findFaculty(University* uni, char* faculty_name, int faculty_id = NULL_INT) {
        if (faculty_id != NULL_INT) {
            for (int i = 0; i < uni->faculty_count; i++) {
                if (uni->faculty_list[i].faculty_id == faculty_id) {
                    return &uni->faculty_list[i];
                }
            }
        }
        for (int i = 0; i < uni->faculty_count; i++) {
            if (strcmp(uni->faculty_list[i].faculty_name, faculty_name) == 0) {
                return &uni->faculty_list[i];
            }
        }
    return nullptr;
}

Department* findDepartment(University* uni, char* department_name, char* faculty_name = nullptr/*, Faculty* faculty = nullptr*/) {
    /*
    if (faculty != nullptr) {
        for (int i = 0; i < faculty->department_count; i++) {
            if (strcmp(faculty->department_list[i].department_name, department_name) == 0) {
                return &faculty->department_list[i];
            }
        }
    }
    */
    if (faculty_name != nullptr) {
        Faculty* fac;
        fac = findFaculty(uni, faculty_name);
        if (fac != nullptr) {
            for (int i = 0; i < fac->department_count; i++) {
                if (strcmp(fac->department_list[i].department_name, department_name) == 0) {
                    return &fac->department_list[i];
                }
            }
        }
    }
    for (int i = 0; i < uni->faculty_count; i++) {
        for (int j = 0; j < uni->faculty_list[i].department_count; j++) {
            if (strcmp(uni->faculty_list[i].department_list[j].department_name, department_name) == 0) {
                return &uni->faculty_list[i].department_list[j];
            }
        }
    }
    return nullptr;
}

Group* findGroup(University* uni, int group_number, char* department_name = nullptr/*, Department* department = nullptr*/) {
    /*
    if (department != nullptr) {
        for (int i = 0; i < department->group_count; i++) {
            if (department->group_list[i].group_number == group_number) {
                return &department->group_list[i];
            }
        }
    }
    */
    if (department_name != nullptr) {
        Department* dep = findDepartment(uni, department_name);
        if (dep != nullptr) {
            for (int i = 0; i < dep->group_count; i++) {
                if (dep->group_list[i].group_number == group_number) {
                    return &dep->group_list[i];
                }
            }
        }
    }
    for (int i = 0; i < uni->faculty_count; i++) {
        for (int j = 0; j < uni->faculty_list[i].department_count; j++) {
            for (int k = 0; k < uni->faculty_list[i].department_list[j].group_count; k++) {
                if (uni->faculty_list[i].department_list[j].group_list[k].group_number == group_number) {
                    return &uni->faculty_list[i].department_list[j].group_list[k];
                }
            }
        }
    }
    return nullptr;
}

Student* findStudent(University* uni, char* name, char* surname, char* fathername, int group_number = NULL_INT) {
    if (group_number != NULL_INT) {
        Group* gr = findGroup(uni, group_number);
        if (gr != nullptr) {
            for (int i = 0; i < gr->student_count; i++) {
                int name_match = strcmp(gr->student_list[i].name, name);
                int surname_match = strcmp(gr->student_list[i].surname, surname);
                int fathername_match = strcmp(gr->student_list[i].fathername, fathername);
                if (!(name_match || surname_match || fathername_match)) {
                    return &gr->student_list[i];
                }
            }
        }
    }
    for (int i = 0; i < uni->faculty_count; i++) {
        for (int j = 0; j < uni->faculty_list[i].department_count; j++) {
            for (int k = 0; k < uni->faculty_list[i].department_list[j].group_count; k++) {
                for (int p = 0; p < uni->faculty_list[i].department_list[j].group_list[k].student_count; p++) {
                    int name_match = strcmp(uni->faculty_list[i].department_list[j].group_list[k].student_list[p].name, name);
                    int surname_match = strcmp(uni->faculty_list[i].department_list[j].group_list[k].student_list[p].surname, surname);
                    int fathername_match = strcmp(uni->faculty_list[i].department_list[j].group_list[k].student_list[p].fathername, fathername);
                    if (!(name_match || surname_match || fathername_match)) {
                        return &uni->faculty_list[i].department_list[j].group_list[k].student_list[p];
                    }
                }
            }
        }
    }
    return nullptr;
}

//these conditions are cheked when writing to a file
//if true - dont write this structure to a file
void deleteStudent(Student* st, University* uni, int group_number) {
    Group* gr = findGroup(uni, group_number);
    if (gr == nullptr) {
        QMessageBox::critical(0, "Ошибка!", "Нет такой группы! Нельзя удалить.");
        return;
    }
    for (int i = 0; i < gr->student_count; i++) {
        int name_match = strcmp(gr->student_list[i].name, st->name);
        int surname_match = strcmp(gr->student_list[i].surname, st->surname);
        int fathername_match = strcmp(gr->student_list[i].fathername, st->fathername);
        if (!(name_match || surname_match || fathername_match)) {
            st->name[0] = NULL_INT;
            gr->student_count -= 1;
            return;
        }
    }
    QMessageBox::critical(0, "Ошибка!", "Нет такого студента в группе! Нельзя удалить.");
}

void deleteGroup(Group* gr, University* uni, char* department_name) {
    Department* dp = findDepartment(uni, department_name);
    if (dp == nullptr) {
        QMessageBox::critical(0, "Ошибка!", "Нет такой кафедры! Нельзя удалить.");
        return;
    }
    for (int i = 0; i < dp->group_count; i++) {
        if (dp->group_list[i].group_number == gr->group_number) {
            gr->group_number = NULL_INT;
            dp->group_count -= 1;
            return;
        }
    }
    QMessageBox::critical(0, "Ошибка!", "Нет такой группы на кафедре! Нельзя удалить.");
}

void deleteDepartment(Department* dp, University* uni, char* faculty_name) {
    Faculty* fac = findFaculty(uni, faculty_name);
    if (fac == nullptr) {
        QMessageBox::critical(0, "Ошибка!", "Нет такого факультета! Нельзя удалить.");
        return;
    }
    for (int i = 0; i < fac->department_count; i++) {
        if (strcmp(fac->department_list[i].department_name, dp->department_name) == 0) {
            dp->group_count = NULL_INT;
            fac->department_count -= 1;
            return;
        }
    }
    QMessageBox::critical(0, "Ошибка!", "Нет такой кафедры на факультете! Нельзя удалить.");
}

void deleteFaculty(Faculty* fac, University* uni) {
    fac->department_count = NULL_INT;
    uni->faculty_count -= 1;
}

void writeGroup(char* gr, FILE* fp, Student* student_list, int student_count) {
    int ret;
    ret = fwrite(gr, 1, SIZE_Group, fp);
    if (ret != SIZE_Group) {
        //cout << "\nerror while writing group " << ret <<endl;
        QMessageBox::critical(0, "Ошибка!", "Не удалось записать информацию о группе.\n"
                                               "Проверьте целостность файла или восстановите его из backup-а.");
        return;
    }
    for (int i = 0; i < student_count; i++) {
        if (student_list[i].name[0] == NULL_INT) { //if deleted, dont write to file
            ++student_count;
            continue;
        }
        ret = fwrite(student_list + i, 1, SIZE_Student, fp);
        if (ret != SIZE_Student) {
            //cout << "\nerror while writing student " << ret <<endl;
            QMessageBox::critical(0, "Ошибка!", "Не удалось записать информацию о студенте.\n"
                                                   "Проверьте целостность файла или восстановите его из backup-а.");
            return;
        }
    }
}

void writeDepartment(char* dp, FILE* fp, Group* group_list, int group_count) {
    int ret;
    ret = fwrite(dp, 1, SIZE_Department, fp);
    if (ret != SIZE_Department) {
        //cout << "\nerror while writing department " << ret <<endl;
        QMessageBox::critical(0, "Ошибка!", "Не удалось записать информацию о кафедре.\n"
                                               "Проверьте целостность файла или восстановите его из backup-а.");
        return;
    }
    for (int i = 0; i < group_count; i++) {
        if (group_list[i].group_number == NULL_INT) { //if deleted, dont write to file
            ++group_count;
            continue;
        }
        writeGroup((char*)&group_list[i], fp, group_list[i].student_list, group_list[i].student_count);
    }

}

void writeFaculty(char* fac, FILE* fp, Department* department_list, int department_count) {
    int ret;
    ret = fwrite(fac, 1, SIZE_Faculty, fp);
    if (ret != SIZE_Faculty) {
        //cout << "\nerror while writing faculty " << ret <<endl;
        QMessageBox::critical(0, "Ошибка!", "Не удалось записать информацию о факультете.\n"
                                               "Проверьте целостность файла или восстановите его из backup-а.");
        return;
    }
    for (int i = 0; i < department_count; i++) {
        if (department_list[i].group_count == NULL_INT) { //if deleted, dont write to file
            ++department_count;
            continue;
        }
        writeDepartment((char*)&department_list[i], fp, department_list[i].group_list, department_list[i].group_count);
    }
}

void writeUniversity(char* uni, FILE* fp, Faculty* faculty_list, int faculty_count) {
    int ret;
    ret = fwrite(uni, 1, SIZE_University, fp);
    if (ret != SIZE_University) {
        //cout << "\nerror while writing university " << ret <<endl;
        QMessageBox::critical(0, "Ошибка!", "Не удалось записать информацию об университете.\n"
                                               "Проверьте целостность файла или восстановите его из backup-а.");
        return;
    }
    for (int i = 0; i < faculty_count; i++) {
        if (faculty_list[i].department_count == NULL_INT) { //if deleted, dont write to file
            ++faculty_count;
            continue;
        }
        writeFaculty((char*)&faculty_list[i], fp, faculty_list[i].department_list, faculty_list[i].department_count);
    }
}


void addStudent(char* name, char* surname, char* fathername, char* gender, char* education_type ,Group* gr, University* uni, int make_student_head_student = 0) {
    if (findStudent(uni, name, surname, fathername) != nullptr) {
        return;
    }
    gr->student_count += 1;
    strcpy(gr->student_list[gr->student_count - 1].name, name);
    strcpy(gr->student_list[gr->student_count - 1].surname, surname);
    strcpy(gr->student_list[gr->student_count - 1].fathername, fathername);
    strcpy(gr->student_list[gr->student_count - 1].gender, gender);
    strcpy(gr->student_list[gr->student_count - 1].education_type, education_type);
    if (make_student_head_student) {
        strcpy(gr->head_student.name, name);
        strcpy(gr->head_student.surname, surname);
        strcpy(gr->head_student.fathername, fathername);
        strcpy(gr->head_student.gender, gender);
        strcpy(gr->head_student.education_type, education_type);
    }
}

void addGroup(int group_number, Department* dp, University* uni) {
    if (findGroup(uni, group_number, dp->department_name) != nullptr) {
        return;
    }
    dp->group_count += 1;
    dp->group_list[dp->group_count - 1].group_number = group_number;
    dp->group_list[dp->group_count - 1].student_count = 0;
    memset(&dp->group_list[dp->group_count - 1].head_student, 0, SIZE_Student);
}

void addDepartment(char* head_name, char* head_surname, char* head_fathername, char* department_name , Faculty* fac, University* uni) {
    if (findDepartment(uni, department_name) != nullptr) {
        return;
    }
    fac->department_count += 1;
    fac->department_list[fac->department_count - 1].group_count = 0;
    strcpy(fac->department_list[fac->department_count - 1].department_name, department_name);
    strcpy(fac->department_list[fac->department_count - 1].dep_head_name, head_name);
    strcpy(fac->department_list[fac->department_count - 1].dep_head_surname, head_surname);
    strcpy(fac->department_list[fac->department_count - 1].dep_head_fathername, head_fathername);
}

void addFaculty(char* head_name, char* head_surname, char* head_fathername, char* faculty_name , int faculty_id, University* uni) {
    if (findFaculty(uni, "", faculty_id) != nullptr) {
        return;
    }
    uni->faculty_count += 1;
    uni->faculty_list[uni->faculty_count - 1].department_count = 0;
    uni->faculty_list[uni->faculty_count - 1].faculty_id = faculty_id;
    strcpy(uni->faculty_list[uni->faculty_count - 1].faculty_name, faculty_name);
    strcpy(uni->faculty_list[uni->faculty_count - 1].fac_head_name, head_name);
    strcpy(uni->faculty_list[uni->faculty_count - 1].fac_head_surname, head_surname);
    strcpy(uni->faculty_list[uni->faculty_count - 1].fac_head_fathername, head_fathername);
}

std::string StudentToStdString(Student* st) {
    if (st == nullptr) {return "";}
    std::stringstream ss;
    ss << "Студент"<< "\nИмя: "<< st->name << "\nФамилия: " << st->surname << "\nОтчество: " << st->fathername <<
    "\nПол: " << st->gender << "\nТип обучения: " << st->education_type;
    return ss.str();
}
std::string StudentToListEntry(Student* st) {
    std::stringstream ss;
    ss << st->surname << " " << st->name << " " << st->fathername <<
       " Пол: " << st->gender << " Тип обучения: " << st->education_type << "\n";
    return ss.str();
}

std::string GroupToStdString(Group* gr) {
    if (gr == nullptr) {return "";}
    std::stringstream ss;
    ss << "Группа" << "\nНомер группы: " << gr->group_number << "\nКоличество студентов: " << gr->student_count << "\nСтароста: " <<
    gr->head_student.surname << " " << gr->head_student.name << " " << gr->head_student.fathername << "\nСписок студентов:\n";
    /*
    for (int i = 0; i < gr->student_count; i++) {
        ss << i + 1 << ") " << StudentToListEntry(&gr->student_list[i]);
    }
    */
    return ss.str();
}

std::string DepartmentToStdString(Department* dp) {
    if (dp == nullptr) {return "";}
    std::stringstream ss;
    ss << "Кафедра" << "\nНазвание кафедры: " << dp->department_name << "\nЗаведующий кафедрой: " << dp->dep_head_surname <<
    " " << dp->dep_head_name << " " << dp->dep_head_fathername << "\nКоличество групп: " << dp->group_count << "\nСписок групп:\n";
    /*
    for (int i = 0; i < dp->group_count; i++) {
        ss << i + 1 << ") " << dp->group_list[i].group_number << "\n";
    }
    */
    return ss.str();
}

std::string FacultyToStdString(Faculty* fac) {
    if (fac == nullptr) {return "";}
    std::stringstream ss;
    ss << "Факультет" << "\nНазвание факультета: " << fac->faculty_name << "\nid факультета в рамках ВУЗа: " << fac->faculty_id <<
    "\nДекан: " << fac->fac_head_surname << " " << fac->fac_head_name << " " << fac->fac_head_fathername << "\nКоличество кафедр: " <<
    fac->department_count << "\nСписок кафедр:\n";
    /*
    for (int i = 0; i < fac->department_count; i++) {
        ss << i + 1 << ") " << fac->department_list[i].department_name << "\n";
    }
    */
    return ss.str();
}

std::string UniversityToStdString(University* uni) {
    if (uni == nullptr) {return "";}
    std::stringstream ss;
    ss << "Университет" << "\nНазвание: " << uni->university_name << "\nРектор: " << uni->uni_head_surname << " " << uni->uni_head_name <<
    " " << uni->uni_head_fathername << "\nКоличество факультетов: " << uni->faculty_count << "\nСписок факультетов:\n";
    /*
    for (int i = 0; i < uni->faculty_count; i++) {
        ss << i + 1 << ") " << uni->faculty_list[i].faculty_name << ", id: " << uni->faculty_list[i].faculty_id << "\n";
    }
    */
    return ss.str();
}


int emptyTheFile() {
    //clears everything except the university info - aka univeristy name and head of university, sets faculty_count to 0
    FILE *file = fopen(datafile, "wb+");
    if (!file) {
        //cout<< "Error while opening file";
        fclose(file);
        return 1; //error
    }

    University* uni = new University;
    uni->faculty_count = 0;
    strcpy(uni->university_name, "СПбГЭТУ \"ЛЭТИ\"");
    strcpy(uni->uni_head_name, "Виктор");
    strcpy(uni->uni_head_surname, "Шелудько");
    strcpy(uni->uni_head_fathername, "Иванович");
    uni->faculty_list = new Faculty[uni->faculty_count];

    writeUniversity((char*)uni, file, uni->faculty_list, uni->faculty_count);

    fclose(file);
    delete uni;
    return 0; //all good
}

int readStruct(University* buf) {
    FILE* fp = fopen(datafile, "rb+");
    if (!fp) {
        return -1; //error while reading
    }
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0){
        return 1; //empty file
    }
    fseek(fp, 0, SEEK_SET);
    readUniversity(buf, fp);
    fclose(fp);
    return 0; //all fine
}

FILE* openDatafile() {
    FILE* fp = fopen(datafile, "rb+");
    if (!fp) {
        return nullptr; //error while reading
    }
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0){
        return nullptr; //empty file
    }
    fseek(fp, 0, SEEK_SET);
    return fp;
}


#endif // CORE_FUNCTIONS_H
