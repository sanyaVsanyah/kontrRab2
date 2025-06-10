#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>

struct Student {
    char name[30];
    int group;
    double grade;
};


void CheckInputFile(std::ifstream&);
void CheckOutputFile(std::ofstream&);
void sortStudents(Student*, int);
int readStudentsFromFile(const char*, Student*, int );
void saveSortedStudents(const char*, Student*, int);
void searchBySurname(Student*, int, const std::string&);
char showMenuAndGetChoice();


int main()
{
    const int MAX_STUDENTS = 100;
    Student students[MAX_STUDENTS];  

    try 
    {
        int count = readStudentsFromFile("students.txt", students, MAX_STUDENTS);
        if (count == 0) 
        {
            return 1;
        }

        char choice;
        while (true)
        {
            choice = showMenuAndGetChoice();

            if (choice == 'q')
            {
                break;
            }
            switch (choice) {
            case 'a':
                sortStudents(students, count);
                saveSortedStudents("sorted.txt", students, count);
                break;
            case 'b': {
                std::string surname;
                std::cout << "Enter surname to search: ";
                std::cin >> surname;
                searchBySurname(students, count, surname);
                break;
            }
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        }
    }
    catch (const char* e)
    {
        std::cerr << "Error occurred: " << e << std::endl;
        return 1;
    }

    return 0;
}


void CheckInputFile(std::ifstream& fin) 
{
    if (!fin.good()) 
    {
        throw "File doesn't exist or cannot be opened";
    }
    if (!fin) 
    {
        throw "Input file error";
    }
    if (fin.peek() == EOF)
    {
        throw "File is empty";
    }
}

void CheckOutputFile(std::ofstream& fout) 
{
    if (!fout.good())
    {
        throw "Output file cannot be created";
    }
    if (!fout)
    {
        throw "Output file error";
    }
}

void sortStudents(Student* students, int count)
{
    for (int i = 0; i < count - 1; ++i)
    {
        for (int j = 0; j < count - i - 1; ++j) 
        {
            if (std::strcmp(students[j].name, students[j + 1].name) > 0)
            {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

int readStudentsFromFile(const char* filename, Student* students, int maxCount) {
    std::ifstream fin(filename);
    try
    {
        CheckInputFile(fin);
    }
    catch (const char* e) 
    {
        std::cerr << "Error: " << e << std::endl;
        return 0;
    }

    int count = 0;
    std::string line;

    while (count < maxCount && std::getline(fin, line))
    {
        if (!line.empty())
        {
            size_t firstDelim = line.find(';');
            size_t secondDelim = line.find(';', firstDelim + 1);

            if (firstDelim != std::string::npos && secondDelim != std::string::npos) 
            {
                std::string nameField = line.substr(0, firstDelim);
                std::strncpy(students[count].name, nameField.c_str(), sizeof(students[count].name) - 1);
                students[count].name[sizeof(students[count].name) - 1] = '\0';

                std::string groupField = line.substr(firstDelim + 1, secondDelim - firstDelim - 1);
                students[count].group = std::atoi(groupField.c_str());

                std::string gradeField = line.substr(secondDelim + 1);
                students[count].grade = std::atof(gradeField.c_str());

                count++;
            }
        }
    }

    fin.close();
    return count;
}

void saveSortedStudents(const char* filename, Student* students, int count) 
{
    std::ofstream fout(filename);
    try 
    {
        CheckOutputFile(fout);
    }
    catch (const char* e) 
    {
        std::cerr << "Error: " << e << std::endl;
        return;
    }

    for (int i = 0; i < count; ++i)
    {
        fout << students[i].name << ";" << students[i].group << ";" << students[i].grade << "\n";
    }
    fout.close();
    std::cout << "Data saved to file " << filename << std::endl;
}

void searchBySurname(Student* students, int count, const std::string& surname) {
    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (std::strncmp(students[i].name, surname.c_str(), surname.length()) == 0) 
        {
            std::cout << students[i].name << ";" << students[i].group << ";" << students[i].grade << std::endl;
            found = true;
        }
    }
    if (!found) 
    {
        std::cout << "Record with surname \"" << surname << "\" not found." << std::endl;
    }
}

char showMenuAndGetChoice()
{
    char choice;
    std::cout << "\nMenu:\n";
    std::cout << "a) Sort by name and save to sorted.txt\n";
    std::cout << "b) Search by surname\n";
    std::cout << "q) Exit\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    return choice;
}