#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>


using namespace std;

// ===================== Part A & B: Classes =====================

class Person {
protected:
    string name;
    int age;
    string ID;
    string contact;

public:
    Person(string name, int age, string ID, string contact) {
        setName(name);
        setAge(age);
        this->ID = ID;
        this->contact = contact;
    }

    virtual ~Person() {}

    // Encapsulation: Setters with validation
    void setName(string name) {
        if (name.empty()) throw invalid_argument("Name cannot be empty.");
        this->name = name;
    }

    void setAge(int age) {
        if (age <= 0 || age > 120) throw invalid_argument("Invalid age.");
        this->age = age;
    }

    void setContact(string contact) { this->contact = contact; }
    void setID(string ID) { this->ID = ID; }

    // Getters
    string getName() const { return name; }
    int getAge() const { return age; }
    string getID() const { return ID; }
    string getContact() const { return contact; }

    virtual void displayDetails() const {
        cout << "Name: " << name << ", Age: " << age << ", ID: " << ID << ", Contact: " << contact << endl;
    }

    virtual double calculatePayment() const {
        return 0.0;
    }
};

class Student : public Person {
private:
    string enrollmentDate;
    string program;
    double GPA;

public:
    Student(string name, int age, string ID, string contact, string enrollmentDate, string program, double GPA)
        : Person(name, age, ID, contact) {
        this->enrollmentDate = enrollmentDate;
        this->program = program;
        setGPA(GPA);
    }

    void setGPA(double GPA) {
        if (GPA < 0.0 || GPA > 4.0) throw invalid_argument("GPA must be between 0.0 and 4.0");
        this->GPA = GPA;
    }

    double getGPA() const { return GPA; }

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Program: " << program << ", GPA: " << GPA << endl;
    }

    double calculatePayment() const override {
        return 15000.0; // Tuition fee
    }
};

class Professor : public Person {
private:
    string department;
    string specialization;
    string hireDate;

public:
    Professor(string name, int age, string ID, string contact, string department, string specialization, string hireDate)
        : Person(name, age, ID, contact), department(department), specialization(specialization), hireDate(hireDate) {}

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Department: " << department << ", Specialization: " << specialization << endl;
    }

    double calculatePayment() const override {
        return 50000.0; // Salary
    }
};

class Course {
private:
    string code;
    string title;
    int credits;
    string description;

public:
    Course(string code, string title, int credits, string description) {
        this->code = code;
        this->title = title;
        setCredits(credits);
        this->description = description;
    }

    void setCredits(int credits) {
        if (credits <= 0) throw invalid_argument("Credits must be positive.");
        this->credits = credits;
    }

    void displayCourse() const {
        cout << "Course Code: " << code << ", Title: " << title << ", Credits: " << credits << endl;
    }
};

class Department {
private:
    string name;
    string location;
    double budget;

public:
    Department(string name, string location, double budget) {
        this->name = name;
        this->location = location;
        this->budget = budget;
    }

    void displayDepartment() const {
        cout << "Department: " << name << ", Location: " << location << ", Budget: $" << budget << endl;
    }
};

// ===================== GradeBook Class =====================

class GradeBook {
private:
    map<string, double> studentGrades;

public:
    void addGrade(const string& studentID, double grade) {
        studentGrades[studentID] = grade;
    }

    double calculateAverageGrade() const {
        if (studentGrades.empty()) return 0.0;
        double total = 0;
        for (const auto& entry : studentGrades)
            total += entry.second;
        return total / studentGrades.size();
    }

    double getHighestGrade() const {
        double highest = 0.0;
        for (const auto& entry : studentGrades)
            if (entry.second > highest) highest = entry.second;
        return highest;
    }

    vector<string> getFailingStudents(double passGrade = 50.0) const {
        vector<string> failing;
        for (const auto& entry : studentGrades)
            if (entry.second < passGrade) failing.push_back(entry.first);
        return failing;
    }
};

// ===================== EnrollmentManager Class =====================

class EnrollmentManager {
private:
    map<string, vector<string>> courseEnrollments;

public:
    void enrollStudent(const string& courseCode, const string& studentID) {
        courseEnrollments[courseCode].push_back(studentID);
    }

    void dropStudent(const string& courseCode, const string& studentID) {
        auto& students = courseEnrollments[courseCode];
        students.erase(remove(students.begin(), students.end(), studentID), students.end());
    }

    int getEnrollmentCount(const string& courseCode) const {
        auto it = courseEnrollments.find(courseCode);
        return (it != courseEnrollments.end()) ? it->second.size() : 0;
    }
};

// ===================== Test Program =====================

void testPolymorphism(Person* p) {
    p->displayDetails();
    cout << "Payment: $" << p->calculatePayment() << endl;
}

int main() {
    // Test Object Creation
    Student s1("Alice", 20, "S1001", "alice@mail.com", "2022-09-01", "Computer Science", 3.5);
    Student s2("Bob", 22, "S1002", "bob@mail.com", "2021-09-01", "Mathematics", 2.8);
    Professor p1("Dr. Smith", 45, "P2001", "smith@mail.com", "Physics", "Quantum Mechanics", "2010-08-15");
    Professor p2("Dr. Lee", 50, "P2002", "lee@mail.com", "CS", "AI", "2005-01-12");
    Course c1("CS101", "Intro to Programming", 3, "Basic programming concepts.");
    Course c2("MATH202", "Linear Algebra", 4, "Matrix theory.");
    Department d1("Computer Science", "Building A", 100000.0);
    Department d2("Mathematics", "Building B", 85000.0);

    // GradeBook Test
    GradeBook gb;
    gb.addGrade("S1001", 85);
    gb.addGrade("S1002", 45);
    cout << "\nAverage Grade: " << gb.calculateAverageGrade() << endl;
    cout << "Highest Grade: " << gb.getHighestGrade() << endl;

    auto failing = gb.getFailingStudents();
    for (auto& id : failing)
        cout << "Failing: " << id << endl;

    // EnrollmentManager Test
    EnrollmentManager em;
    em.enrollStudent("CS101", "S1001");
    em.enrollStudent("CS101", "S1002");
    cout << "Enrollment in CS101: " << em.getEnrollmentCount("CS101") << endl;
    em.dropStudent("CS101", "S1002");
    cout << "Enrollment in CS101 after drop: " << em.getEnrollmentCount("CS101") << endl;

    // Polymorphism Test
    vector<Person*> people = { &s1, &s2, &p1, &p2 };
    for (Person* p : people) {
        testPolymorphism(p);
        cout << "------------------" << endl;
    }

    return 0;
}
