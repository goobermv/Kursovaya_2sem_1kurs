#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>    
#include <fstream>
#include <sstream>

struct Student { // Структура Студент в которой содержится вся необходимая информация о студенте
    char name[50];     
    int group_number;
    int marks[5];
    int scholarship;
    std::string email;

    Student(const char* _name, int _group_number, int _marks[], int _scholarship, const std::string& _email)
        : group_number(_group_number), scholarship(_scholarship), email(_email) {
        std::strcpy(name, _name);
        for (int i = 0; i < 5; ++i) {
            marks[i] = _marks[i];
        }
    }
};

int hashFunction(int groupNumber, int tableSize) { // Хэш функция для номера группы
    return groupNumber % tableSize;
}

struct Node { // Структура узлов связанного списка в хэш таблице
    Student student;
    Node* next;

    Node(const Student& s) : student(s), next(nullptr) {}
};

class HashTable { // Класс хэш таблицы
private:
    std::vector<Node*> table;
    int size;
    int count; // Количество элементов в таблице

    void resize() { //Метод для увеличения размера хэш таблицы
        int newSize = size + 1;
        std::vector<Node*> newTable(newSize, nullptr);

        for (int i = 0; i < size; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* next = current->next;

                int newIndex = hashFunction(current->student.group_number, newSize);
                current->next = newTable[newIndex];
                newTable[newIndex] = current;

                current = next;
            }
        }

        table = std::move(newTable);
        size = newSize;
    }

public:
    HashTable() : size(1), count(0) { // Конструктор хэш таблицы
        table.resize(size, nullptr);
    }

    ~HashTable() { // Деструктор хэш таблицы
        for (int i = 0; i < size; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
    }

    void addStudent(const Student& student) { // Метод добавления студента в базу данных
        if (count == size) {
            resize(); // Увеличиваем размер таблицы при если она заполняется
        }

        int index = hashFunction(student.group_number, size);
        Node* newNode = new Node(student);
        if (table[index] == nullptr) {
            table[index] = newNode;
        } 
        else {
            Node* current = table[index];
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++count;
    }

    Node* findStudent(int groupNumber, const char* studentName) const { // Метод поиска студента в базе данных
        int index = hashFunction(groupNumber, size);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->student.group_number == groupNumber && std::strcmp(current->student.name, studentName) == 0) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void removeStudent(int groupNumber, const char* studentName) { // Метод удаления студента из базы данных
        int index = hashFunction(groupNumber, size);
        Node* current = table[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->student.group_number == groupNumber && std::strcmp(current->student.name, studentName) == 0) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } 
                else {
                    prev->next = current->next;
                }
                delete current;
                --count;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    void removeGroup(int groupNumber) { // Метод удаления группы из базы данных
        int index = hashFunction(groupNumber, size);
        Node* current = table[index];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
            --count;
        }
        table[index] = nullptr;
    }

    void printStudentInfo(const Student& student) const { // Метод вывода информации студента
        std::cout << "Name: " << student.name << "\n";
        std::cout << "Group number: " << student.group_number << "\n";
        std::cout << "Marks: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << student.marks[i] << " ";
        }
        std::cout << "\nScholarship: " << student.scholarship << "\n";
        std::cout << "Email: " << student.email << "\n";
    }

    void modifyStudent(int groupNumber, const char* studentName) { //  Метод изменения данных о студенте
        Node* studentNode = findStudent(groupNumber, studentName);
        if (studentNode == nullptr) {
            std::cout << "Student not found.\n";
            return;
        }

        std::cout << "Choose a field to edit:\n";
        std::cout << "1. Name\n";
        std::cout << "2. Group number\n";
        std::cout << "3. Marks\n";
        std::cout << "4. Scholarship\n";
        std::cout << "5. Email\n";

        int choice;
        std::cin >> choice;
        std::cin.ignore(); 

        switch (choice) {
            case 1: {
                std::cout << "Введите новое имя: ";
                char newName[50];
                std::cin.getline(newName, 50);
                std::strcpy(studentNode->student.name, newName);
                break;
            }
            case 2: {
                std::cout << "Введите новый номер группы: ";
                int newGroupNumber;
                std::cin >> newGroupNumber;
                studentNode->student.group_number = newGroupNumber;
                break;
            }
            case 3: {
                std::cout << "Введите новые оценки (5 штук через пробел): ";
                int newMarks[5];
                for (int i = 0; i < 5; ++i) {
                    std::cin >> newMarks[i];
                    studentNode->student.marks[i] = newMarks[i];
                }
                break;
            }
            case 4: {
                std::cout << "Введите новую стипендию: ";
                int newScholarship;
                std::cin >> newScholarship;
                studentNode->student.scholarship = newScholarship;
                break;
            }
            case 5: {
                std::cout << "Введите новый email: ";
                std::string newEmail;
                std::cin.ignore();
                std::getline(std::cin, newEmail);
                studentNode->student.email = newEmail;
                break;
            }
            default: {
                std::cout << "Неверный выбор.\n";
                break;
            }
        }
    }

    void addStudentFromInput() {// Метод для добавления студента
        char name[50];
        int groupNumber;
        int marks[5];
        int scholarship;
        std::string email;

        std::cout << "Enter student name: ";
        std::cin.getline(name, 50);

        std::cout << "Enter group number: ";
        std::cin >> groupNumber;

        std::cout << "Enter marks (5 numbers with spaces in between): ";
        for (int i = 0; i < 5; ++i) {
            std::cin >> marks[i];
        }

        std::cout << "Enter the scholarship amount: ";
        std::cin >> scholarship;

        std::cout << "Enter email: ";
        std::cin.ignore(); 
        std::getline(std::cin, email);

        Student newStudent(name, groupNumber, marks, scholarship, email);
        addStudent(newStudent);
    }

    void printGroupsByCondition(double threshold) { // Метод для сравнения соотношения стипендии/баллов и введеным числом 
        struct GroupStats {
            int groupNumber;
            int totalScholarship;
            int totalMarks;
        };

        std::vector<GroupStats> groupStats;

        for (int i = 0; i < size; ++i) { // Собираем статистику по группам
            Node* current = table[i];
            while (current != nullptr) {
                int groupNumber = current->student.group_number;
                int totalMarks = 0;
                for (int j = 0; j < 5; ++j) {
                    totalMarks += current->student.marks[j];
                }

                bool groupFound = false;
                for (auto& stats : groupStats) {
                    if (stats.groupNumber == groupNumber) {
                        stats.totalScholarship += current->student.scholarship;
                        stats.totalMarks += totalMarks;
                        groupFound = true;
                        break;
                    }
                }

                if (!groupFound) {
                    groupStats.push_back({groupNumber, current->student.scholarship, totalMarks});
                }

                current = current->next;
            }
        }

        std::vector<int> filteredGroups; // Фильтруем и сортируем группы
        for (const auto& stats : groupStats) {
            if (stats.totalMarks > 0 && static_cast<double>(stats.totalScholarship) / stats.totalMarks > threshold) {
                filteredGroups.push_back(stats.groupNumber);
            }
        }

        std::sort(filteredGroups.begin(), filteredGroups.end()); // Сортировка групп

        std::cout << "Groups who apply to the condition:\n"; // Выводим результаты
        for (int groupNumber : filteredGroups) {
            std::cout << "Group number: " << groupNumber << "\n";
        }
    }

    void addStudentsFromFile(const std::string& filename) { // Метод для добавления студентов из файла
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error when opening file.\n";
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int groupNumber;
            int marks[5];
            int scholarship;
            std::string email;

            std::getline(iss, name, ',');
            iss >> groupNumber;
            for (int i = 0; i < 5; ++i) {
                iss >> marks[i];
            }
            iss >> scholarship;
            iss.ignore();
            std::getline(iss, email);

            Student newStudent(name.c_str(), groupNumber, marks, scholarship, email);
            addStudent(newStudent);
        }

        file.close();
    }

    void outputStudentsIntoFile(int groupNumber, const char* studentName, const std::string& filename){ // Метод для вывода студентов в файла
        Node* studentNode = findStudent(groupNumber, studentName);
        if (studentNode == nullptr) {
            std::cout << "Student not found.\n";
            return;
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file.\n";
            return;
        }

        const Student& student = studentNode->student;
        file << "Name: " << student.name << "\n";
        file << "Group number: " << student.group_number << "\n";
        file << "Marks: ";
        for (int i = 0; i < 5; ++i) {
            file << student.marks[i] << " ";
        }
        file << "\n";
        file << "Scholarship: " << student.scholarship << "\n";
        file << "Email: " << student.email << "\n\n";

        file.close();
        std::cout << "Student information written to file: " << filename << "\n";
    }
};

void Menu(HashTable& hashtable){ // Функция меню действий
    std::cout << "\nChoose an action:\n";
    std::cout << "1 - Enter a new student via keyboard\n";
    std::cout << "2 - Enter a new student from a file\n";
    std::cout << "3 - Display the information of a student\n";
    std::cout << "4 - Edit student information\n";
    std::cout << "5 - Delete a student or group\n";
    std::cout << "6 - Display the groups whos scholarship/marks ratio is higher than an inputed number\n";
    std::cout << "7 - Output the information of a student into a file\n";
    std::cout << "0 - Exit\n";

    int action;
    std::cin >> action;
    std::cin.ignore();
    switch (action){
        case 1: {// Ввод информации о студенте с клавиатуры
            hashtable.addStudentFromInput();
            Menu(hashtable);
            break;
        }
        case 2: {// Ввод информации о студенте из файла(формат в файле: Имя,НомерГруппы,Оценка1 Оценка2 Оценка3 Оценка4 Оценка5,Стипендия,Email)   
            std::string filename;
            std::cout << "Enter file name: ";
            std::cin >> filename;
            hashtable.addStudentsFromFile(filename);
            Menu(hashtable);
            break;
        }
        case 3: {// Поиск и вывод информации о студенте в консоль
            char searchName[50];
            int searchGroupNumber;
            std::cout << "Enter the name of the student to search for: ";
            std::cin.getline(searchName, 50);

            std::cout << "Enter the group number of the student to search for: ";
            std::cin >> searchGroupNumber;

            Node* foundStudent = hashtable.findStudent(searchGroupNumber, searchName);
            if (foundStudent != nullptr)
                hashtable.printStudentInfo(foundStudent->student);
            else
                std::cout << "Student not found.\n";
            Menu(hashtable);
            break;
        }
        case 4: {// Редактирование информации студента    
            char searchName[50];
            int searchGroupNumber;
            std::cout << "Enter the name of the student who information will be edited: ";
            std::cin.getline(searchName, 50);

            std::cout << "Enter the group number of the student whos information will be edited: ";
            std::cin >> searchGroupNumber;

            hashtable.modifyStudent(searchGroupNumber, searchName);
            Menu(hashtable);
            break;
        }
        case 5: {// Удаление студента или группы из базы данных
            std::cout << "Choose which you would like to delete:\n";
            std::cout << "1. Student\n";
            std::cout << "2. Group\n";
            std::cout << "0. Back to previous menu\n";

            int del_action;
            std::cin >> del_action;
            std::cin.ignore();
            switch (del_action){
                case 1:{
                    char searchName[50];
                    int searchGroupNumber;
                    std::cout << "Enter the name of the student to be deleted: ";
                    std::cin.getline(searchName, 50);

                    std::cout << "Enter the group number of the student: ";
                    std::cin >> searchGroupNumber;

                    hashtable.removeStudent(searchGroupNumber, searchName);
                    break;
                }   
                case 2:{ 
                    int groupNumber;
                    std::cout << "Enter the group to be deleted: ";
                    std::cin >> groupNumber;
                    hashtable.removeGroup(groupNumber);   
                    break;
                }
                case 0:
                    break;
                default:
                    break;
            }
            Menu(hashtable);
            break;
        }
        case 6: {// Индивидуальное задание вывода групп которые где «суммарный объем стипендии» / «общее количество баллов» 
            double threshold; // превышает заданое значение
            std::cout << "Enter a number for the ratio 'total scholarship sum'/'total sum of marks: ";
            std::cin >> threshold;
            hashtable.printGroupsByCondition(threshold);
            Menu(hashtable);
            break;
        }
        case 7:{ // Вывод информации о студенте в файл
            char searchName[50];
            int searchGroupNumber;
            std::cout << "Enter the name of the student to write to file: ";
            std::cin.getline(searchName, 50);

            std::cout << "Enter the group number of the student: ";
            std::cin >> searchGroupNumber;

            std::string filename;
            std::cout << "Enter the filename to write to: ";
            std::cin >> filename;

            hashtable.outputStudentsIntoFile(searchGroupNumber, searchName, filename);
            Menu(hashtable);
            break;
        }
        case 0: // Выход из программы
            return;
        default:
            std::cout << "Wrong input.\n";
            return;
    }
}

int main(){
    HashTable hashtable;
    std::cout << "Welcome to the Student Database!\n";
    Menu(hashtable);
    return 0;
}