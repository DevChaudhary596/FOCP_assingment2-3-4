
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <fstream>
using namespace std;


class UniversitySystemException : public exception {
protected:
    string message;

public:
    UniversitySystemException(const string& msg) : message(msg) {}
    virtual const char* what() const noexcept override { return message.c_str(); }
};

class EnrollmentException : public UniversitySystemException {
public:
    EnrollmentException(const string& msg) : UniversitySystemException("Enrollment Error: " + msg) {}
};

class GradeException : public UniversitySystemException {
public:
    GradeException(const string& msg) : UniversitySystemException("Grade Error: " + msg) {}
};

class PaymentException : public UniversitySystemException {
public:
    PaymentException(const string& msg) : UniversitySystemException("Payment Error: " + msg) {}
};

void logError(const string& error) {
    ofstream log("errors.log", ios::app);
    log << error << endl;
    log.close();
}


class Person {
protected:
    string name;
    int age;
    string ID;
    string contact;

public:
    Person(string name, int age, string ID, string contact)
        : name(name), age(age), ID(ID), contact(contact) {
        if (ID.empty()) throw UniversitySystemException("Invalid ID provided");
        if (contact.find('@') == string::npos) throw UniversitySystemException("Invalid contact info");
    }

    virtual void displayDetails() const {
        cout << "Name: " << name << ", Age: " << age << ", ID: " << ID << ", Contact: " << contact << endl;
    }

    virtual double calculatePayment() const = 0;

    virtual ~Person() {}
};

class Student : public Person {
protected:
    string enrollmentDate, program;
    double GPA;

public:
    Student(string name, int age, string ID, string contact,
            string enrollmentDate, string program, double GPA)
        : Person(name, age, ID, contact), enrollmentDate(enrollmentDate), program(program), GPA(GPA) {}

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Program: " << program << ", GPA: " << GPA << endl;
    }

    double calculatePayment() const override {
        if (GPA < 0.0 || GPA > 4.0) throw PaymentException("Invalid GPA for payment calculation");
        return 10000.0;
    }
};

class UndergraduateStudent : public Student {
private:
    string major, minor, expectedGraduation;

public:
    UndergraduateStudent(string name, int age, string ID, string contact,
                         string enrollmentDate, string program, double GPA,
                         string major, string minor, string gradDate)
        : Student(name, age, ID, contact, enrollmentDate, program, GPA),
          major(major), minor(minor), expectedGraduation(gradDate) {}

    void displayDetails() const override {
        Student::displayDetails();
        cout << "Major: " << major << ", Minor: " << minor << ", Grad Date: " << expectedGraduation << endl;
    }

    double calculatePayment() const override {
        return 12000.0;
    }
};

class GraduateStudent : public Student {
private:
    string researchTopic, advisor, thesisTitle;

public:
    GraduateStudent(string name, int age, string ID, string contact,
                    string enrollmentDate, string program, double GPA,
                    string topic, string advisor, string thesis)
        : Student(name, age, ID, contact, enrollmentDate, program, GPA),
          researchTopic(topic), advisor(advisor), thesisTitle(thesis) {}

    void displayDetails() const override {
        Student::displayDetails();
        cout << "Research: " << researchTopic << ", Advisor: " << advisor << ", Thesis: " << thesisTitle << endl;
    }

    double calculatePayment() const override {
        return 8000.0;
    }

    void addTAship(double hours) {
        cout << "TAship: " << hours << " hrs logged." << endl;
    }
};


class Professor : public Person {
protected:
    string department, specialization;
    string hireDate;

public:
    Professor(string name, int age, string ID, string contact,
              string department, string specialization, string hireDate)
        : Person(name, age, ID, contact), department(department), specialization(specialization), hireDate(hireDate) {}

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Dept: " << department << ", Specialization: " << specialization << ", Hire Date: " << hireDate << endl;
    }
};

class AssistantProfessor : public Professor {
public:
    AssistantProfessor(string name, int age, string ID, string contact,
                       string department, string specialization, string hireDate)
        : Professor(name, age, ID, contact, department, specialization, hireDate) {}

    double calculatePayment() const override {
        return 50000.0;
    }
};

class AssociateProfessor : public Professor {
public:
    AssociateProfessor(string name, int age, string ID, string contact,
                       string department, string specialization, string hireDate)
        : Professor(name, age, ID, contact, department, specialization, hireDate) {}

    double calculatePayment() const override {
        return 65000.0;
    }
};

class FullProfessor : public Professor {
public:
    FullProfessor(string name, int age, string ID, string contact,
                  string department, string specialization, string hireDate)
        : Professor(name, age, ID, contact, department, specialization, hireDate) {}

    double calculatePayment() const override {
        return 80000.0;
    }
};


class Course {
private:
    string code, title, description;
    int credits, maxStudents = 30;
    Professor* instructor;
    vector<Student*> students;

public:
    Course(string code, string title, int credits, string description)
        : code(code), title(title), credits(credits), description(description), instructor(nullptr) {}

    void setInstructor(Professor* prof) { instructor = prof; }
    void enrollStudent(Student* student) {
        if (students.size() >= maxStudents)
            throw EnrollmentException("Course is full: " + code);
        students.push_back(student);
    }
};

class Department {
private:
    string name;
    vector<Professor*> professors;
    vector<Course> courses;

public:
    Department(string name) : name(name) {}

    void addProfessor(Professor* prof) { professors.push_back(prof); }
    void addCourse(Course course) { courses.push_back(course); }
};

class University {
private:
    vector<Department> departments;

public:
    void addDepartment(const Department& dept) {
        departments.push_back(dept);
    }
};

class Classroom {
private:
    string roomNumber;
    int capacity;

public:
    Classroom(string room, int cap) : roomNumber(room), capacity(cap) {}
};

class Schedule {
private:
    map<string, pair<string, string>> courseSchedule; // courseCode -> (timeSlot, room)

public:
    void addSchedule(string courseCode, string time, string room) {
        courseSchedule[courseCode] = {time, room};
    }
};

class GradeBook {
private:
    map<string, double> grades; // studentID -> grade

public:
    void addGrade(string studentID, double grade) {
        if (grade < 0 || grade > 100)
            throw GradeException("Invalid grade entry: " + to_string(grade));
        grades[studentID] = grade;
    }

    double calculateAverageGrade() {
        double sum = 0;
        for (auto& g : grades) sum += g.second;
        return grades.empty() ? 0 : sum / grades.size();
    }
};

class EnrollmentManager {
private:
    map<string, vector<string>> courseEnrollments; // courseCode -> list of studentIDs

public:
    void enrollStudent(string courseCode, string studentID) {
        courseEnrollments[courseCode].push_back(studentID);
    }
};


int main() {
    try {
        UndergraduateStudent u("Alice", 20, "S123", "alice@email.com", "2022", "CS", 3.5, "CS", "Math", "2025");
        GraduateStudent g("Bob", 25, "S124", "bob@email.com", "2021", "Physics", 3.8, "Quantum", "Dr. Smith", "Dark Matter");
        AssistantProfessor ap("Dr. Jane", 40, "P123", "jane@email.com", "Science", "Biology", "2015");

        Department d("Computer Science");
        d.addProfessor(&ap);

        Course c("CS101", "Intro to CS", 3, "Basics of programming");
        c.setInstructor(&ap);
        c.enrollStudent(&u);

        University uni;
        uni.addDepartment(d);

        GradeBook gb;
        gb.addGrade("S123", 90);

        cout << "University System Initialized." << endl;
    } catch (UniversitySystemException& e) {
        cerr << "Error: " << e.what() << endl;
        logError(e.what());
    }

    return 0;
}
