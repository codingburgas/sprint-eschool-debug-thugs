#include <iostream>
#include <string>
#include <vector>


class User {
public:
    virtual bool login(const std::string& username, const std::string& password) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getType() const = 0;
    virtual ~User() {} 
};

class Subject {
public:
    enum Type { HISTORY, PHYSICS, MATHEMATICS };
    enum Grade { SIXTH, SEVENTH, EIGHTH };

private:
    Type type;
    Grade grade;
    std::string content;
    std::vector<std::string> questions;
    std::vector<std::string> answers;

public:
    Subject(Type t, Grade g, const std::string& cont)
        : type(t), grade(g), content(cont) {}

    void addTest(const std::string& question, const std::string& answer) {
        questions.push_back(question);
        answers.push_back(answer);
    }

    double takeTest() {
        int correctAnswers = 0;
        std::string userAnswer;

        std::cout << "Subject Test:\n";
        for (size_t i = 0; i < questions.size(); ++i) {
            std::cout << questions[i] << "\n";
            std::cout << "Your Answer: ";
            std::getline(std::cin, userAnswer);

            if (userAnswer == answers[i]) {
                correctAnswers++;
            }
        }

        return (static_cast<double>(correctAnswers) / questions.size()) * 100;
    }

    void displayContent() {
        std::cout << "Study Material:\n" << content << std::endl;
    }

    Type getType() const { return type; }
    Grade getGrade() const { return grade; }
};

class Student : public User {
private:
    std::string name;
    std::string username;
    std::string password;
    int grade;
    std::vector<std::pair<Subject*, double>> grades;

public:
    Student(const std::string& n, const std::string& user,
        const std::string& pass, int g)
        : name(n), username(user), password(pass), grade(g) {}

    bool login(const std::string& user, const std::string& pass) override {
        return username == user && password == pass;
    }

    std::string getName() const override { return name; }
    std::string getType() const override { return "Student"; }

    void studySubject(Subject* subject) {
        subject->displayContent();
    }

    void takeTest(Subject* subject) {
        double score = subject->takeTest();
        grades.push_back({ subject, score });
        std::cout << "Your score: " << score << "%\n";
    }

    void viewGrades() {
        std::cout << "Your Grades:\n";
        for (const auto& grade : grades) {
            std::string subjectName;
            switch (grade.first->getType()) {
            case Subject::HISTORY: subjectName = "History"; break;
            case Subject::PHYSICS: subjectName = "Physics"; break;
            case Subject::MATHEMATICS: subjectName = "Mathematics"; break;
            }
            std::cout << subjectName << ": " << grade.second << "%\n";
        }
    }

    const std::vector<std::pair<Subject*, double>>& getGradesVector() const {
        return grades;
    }
};

class Parent : public User {
private:
    std::string name;
    std::string username;
    std::string password;
    std::vector<Student*> children;

public:
    Parent(const std::string& n, const std::string& user, const std::string& pass)
        : name(n), username(user), password(pass) {}

    bool login(const std::string& user, const std::string& pass) override {
        return username == user && password == pass;
    }

    std::string getName() const override { return name; }
    std::string getType() const override { return "Parent"; }

    void addChild(Student* child) {
        children.push_back(child);
    }

    void viewChildGrades() {
        for (auto* child : children) {
            std::cout << "Grades for " << child->getName() << ":\n";
            for (const auto& grade : child->getGradesVector()) {
                std::string subjectName;
                switch (grade.first->getType()) {
                case Subject::HISTORY: subjectName = "History"; break;
                case Subject::PHYSICS: subjectName = "Physics"; break;
                case Subject::MATHEMATICS: subjectName = "Mathematics"; break;
                }
                std::cout << subjectName << ": " << grade.second << "%\n";
            }
        }
    }
};

class EducationalApp {
private:
    std::vector<User*> users;
    std::vector<Subject*> subjects;

    void initializeSubjects() {
       
        Subject* history6 = new Subject(Subject::HISTORY, Subject::SIXTH,
            "Ancient Civilizations: Mesopotamia was one of the earliest civilizations.");
        history6->addTest("Who were the first inhabitants of Mesopotamia?", "Sumerians");
        history6->addTest("What is a nomadic tribe?", "A group of people who move from place to place");

        
        Subject* physics7 = new Subject(Subject::PHYSICS, Subject::SEVENTH,
            "Electricity Basics: Understanding circuits and electrical components.");
        physics7->addTest("What are the basic components of an electrical circuit?", "Power source, conductors, load");
        physics7->addTest("What creates a magnetic field?", "Moving electrical charges");

       
        Subject* math8 = new Subject(Subject::MATHEMATICS, Subject::EIGHTH,
            "Geometry: Advanced geometric principles and trigonometry basics.");
        math8->addTest("What is the Pythagorean theorem?", "a² + b² = c² in a right-angled triangle");
        math8->addTest("What is a trigonometric function?", "A function that relates angles to sides of a triangle");

        subjects = { history6, physics7, math8 };
    }

public:
    EducationalApp() {
        initializeSubjects();
    }

    void run() {
        while (true) {
            std::cout << "\nEducational App\n";
            std::cout << "1. Register Student\n";
            std::cout << "2. Register Parent\n";
            std::cout << "3. Student Login\n";
            std::cout << "4. Parent Login\n";
            std::cout << "5. Exit\n";
            std::cout << "Choose option: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
            case 1: registerStudent(); break;
            case 2: registerParent(); break;
            case 3: studentLogin(); break;
            case 4: parentLogin(); break;
            case 5: return;
            default: std::cout << "Invalid option\n";
            }
        }
    }

    void registerStudent() {
        std::string name, username, password;
        int grade;

        std::cout << "Name: ";
        std::getline(std::cin, name);
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);
        std::cout << "Grade (6/7/8): ";
        std::cin >> grade;
        std::cin.ignore();

        users.push_back(new Student(name, username, password, grade));
        std::cout << "Student registered successfully!\n";
    }

    void registerParent() {
        std::string name, username, password;

        std::cout << "Name: ";
        std::getline(std::cin, name);
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        Parent* parent = new Parent(name, username, password);

        char addChild;
        std::cout << "Add a child? (y/n): ";
        std::cin >> addChild;
        std::cin.ignore();

        if (addChild == 'y' || addChild == 'Y') {
            for (auto* user : users) {
                if (user->getType() == "Student") {
                    Student* student = dynamic_cast<Student*>(user);
                    if (student) {
                        std::cout << "Student: " << student->getName() << "\n";
                        char choice;
                        std::cout << "Add this student? (y/n): ";
                        std::cin >> choice;
                        std::cin.ignore();
                        if (choice == 'y' || choice == 'Y') {
                            parent->addChild(student);
                        }
                    }
                }
            }
        }

        users.push_back(parent);
        std::cout << "Parent registered successfully!\n";
    }

    void studentLogin() {
        std::string username, password;
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        for (auto* user : users) {
            if (user->getType() == "Student" && user->login(username, password)) {
                Student* student = dynamic_cast<Student*>(user);
                if (student) {
                    studentMenu(student);
                    return;
                }
            }
        }
        std::cout << "Login failed.\n";
    }

    void studentMenu(Student* student) {
        while (true) {
            std::cout << "\nStudent Menu:\n";
            std::cout << "1. Study Subject\n";
            std::cout << "2. Take Test\n";
            std::cout << "3. View Grades\n";
            std::cout << "4. Logout\n";
            std::cout << "Choose option: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
            case 1: selectSubjectToStudy(student); break;
            case 2: selectSubjectToTest(student); break;
            case 3: student->viewGrades(); break;
            case 4: return;
            default: std::cout << "Invalid option\n";
            }
        }
    }

    void selectSubjectToStudy(Student* student) {
        std::cout << "Available Subjects:\n";
        for (size_t i = 0; i < subjects.size(); ++i) {
            std::cout << i + 1 << ". ";
            switch (subjects[i]->getType()) {
            case Subject::HISTORY: std::cout << "History "; break;
            case Subject::PHYSICS: std::cout << "Physics "; break;
            case Subject::MATHEMATICS: std::cout << "Mathematics "; break;
            }
            std::cout << "Grade " << subjects[i]->getGrade() + 6 << std::endl;
        }

        std::cout << "Choose subject: ";
        int subjectChoice;
        std::cin >> subjectChoice;
        std::cin.ignore();

        if (subjectChoice > 0 && subjectChoice <= subjects.size()) {
            student->studySubject(subjects[subjectChoice - 1]);
        }
    }

    void selectSubjectToTest(Student* student) {
        std::cout << "Available Subjects:\n";
        for (size_t i = 0; i < subjects.size(); ++i) {
            std::cout << i + 1 << ". ";
            switch (subjects[i]->getType()) {
            case Subject::HISTORY: std::cout << "History "; break;
            case Subject::PHYSICS: std::cout << "Physics "; break;
            case Subject::MATHEMATICS: std::cout << "Mathematics "; break;
            }
            std::cout << "Grade " << subjects[i]->getGrade() + 6 << std::endl;
        }

        std::cout << "Choose subject to test: ";
        int subjectChoice;
        std::cin >> subjectChoice;
        std::cin.ignore();

        if (subjectChoice > 0 && subjectChoice <= subjects.size()) {
            student->takeTest(subjects[subjectChoice - 1]);
        }
    }

    void parentLogin() {
        std::string username, password;
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        for (auto* user : users) {
            if (user->getType() == "Parent" && user->login(username, password)) {
                Parent* parent = dynamic_cast<Parent*>(user);
                if (parent) {
                    parentMenu(parent);
                    return;
                }
            }
        }
        std::cout << "Login failed.\n";
    }

    void parentMenu(Parent* parent) {
        while (true) {
            std::cout << "\nParent Menu:\n";
            std::cout << "1. View Child Grades\n";
            std::cout << "2. Logout\n";
            std::cout << "Choose option: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
            case 1: parent->viewChildGrades(); break;
            case 2: return;
            default: std::cout << "Invalid option\n";
            }
        }
    }

    ~EducationalApp() {
        for (auto* user : users) delete user;
        for (auto* subject : subjects) delete subject;
    }
};

int main() {
    EducationalApp app;
    app.run();
    return 0;
}