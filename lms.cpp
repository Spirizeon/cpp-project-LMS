#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <ctime>

using namespace std;

// Utility functions
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnterToContinue() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Book class definition
class Book {
private:
    string bookNo;
    string bookName;
    string authorName;
    bool isIssued;

public:
    Book() : isIssued(false) {}

    void createBook() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nNEW BOOK ENTRY...\n";
        cout << "Enter Book No.: ";
        getline(cin, bookNo);
        cout << "Enter Book Name: ";
        getline(cin, bookName);
        cout << "Enter Author Name: ";
        getline(cin, authorName);
        isIssued = false;
        cout << "\nBook Created Successfully!";
    }

    void showBook() const {
        cout << "\nBook Number: " << bookNo;
        cout << "\nBook Name: " << bookName;
        cout << "\nAuthor Name: " << authorName;
        cout << "\nStatus: " << (isIssued ? "Issued" : "Available");
    }

    void modifyBook() {
        cout << "\nBook Number: " << bookNo;
        cout << "\nModify Book Name: ";
        getline(cin, bookName);
        cout << "Modify Author's Name: ";
        getline(cin, authorName);
    }

    string getBookNo() const { return bookNo; }
    bool getIsIssued() const { return isIssued; }
    void setIssuedStatus(bool status) { isIssued = status; }

    void report() const {
        cout << left << setw(10) << bookNo
             << setw(30) << bookName
             << setw(20) << authorName
             << setw(10) << (isIssued ? "Issued" : "Available") << endl;
    }
};

// Student class definition
class Student {
private:
    string admissionNo;
    string name;
    string issuedBookNo;
    time_t issueDate;
    bool hasBook;

public:
    Student() : hasBook(false), issueDate(0) {}

    void createStudent() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nNEW STUDENT ENTRY...\n";
        cout << "Enter Admission No.: ";
        getline(cin, admissionNo);
        cout << "Enter Student Name: ";
        getline(cin, name);
        hasBook = false;
        issuedBookNo = "";
        cout << "\nStudent Record Created Successfully!";
    }

    void showStudent() const {
        cout << "\nAdmission Number: " << admissionNo;
        cout << "\nStudent Name: " << name;
        cout << "\nBook Status: " << (hasBook ? "Issued" : "No book issued");
        if (hasBook) {
            cout << "\nIssued Book Number: " << issuedBookNo;
            cout << "\nIssue Date: " << ctime(&issueDate);
        }
    }

    void modifyStudent() {
        cout << "\nAdmission No.: " << admissionNo;
        cout << "\nModify Student Name: ";
        getline(cin, name);
    }

    string getAdmissionNo() const { return admissionNo; }
    string getIssuedBookNo() const { return issuedBookNo; }
    bool getHasBook() const { return hasBook; }
    time_t getIssueDate() const { return issueDate; }

    void issueBook(const string& bookNo) {
        issuedBookNo = bookNo;
        hasBook = true;
        issueDate = time(0);
    }

    void returnBook() {
        issuedBookNo = "";
        hasBook = false;
        issueDate = 0;
    }

    void report() const {
        cout << left << setw(15) << admissionNo
             << setw(25) << name
             << setw(15) << (hasBook ? "Yes" : "No")
             << setw(15) << (hasBook ? issuedBookNo : "-") << endl;
    }
};

// Global file handling functions
class LibrarySystem {
private:
    fstream file;

    bool findBook(const string& bookNo, Book& book) {
        file.open("books.dat", ios::binary | ios::in);
        bool found = false;
       
        while (file.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
            if (book.getBookNo() == bookNo) {
                found = true;
                break;
            }
        }
       
        file.close();
        return found;
    }

    bool findStudent(const string& admNo, Student& student) {
        file.open("students.dat", ios::binary | ios::in);
        bool found = false;
       
        while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
            if (student.getAdmissionNo() == admNo) {
                found = true;
                break;
            }
        }
       
        file.close();
        return found;
    }

public:
    void writeBook() {
        clearScreen();
        Book book;
        book.createBook();
       
        file.open("books.dat", ios::binary | ios::app);
        file.write(reinterpret_cast<char*>(&book), sizeof(Book));
        file.close();
    }

    void writeStudent() {
        clearScreen();
        Student student;
        student.createStudent();
       
        file.open("students.dat", ios::binary | ios::app);
        file.write(reinterpret_cast<char*>(&student), sizeof(Student));
        file.close();
    }

    void displayAllBooks() {
        clearScreen();
        cout << "\n\n\t\tBOOK LIST\n\n";
        cout << string(70, '=') << endl;
        cout << left << setw(10) << "Book No."
             << setw(30) << "Title"
             << setw(20) << "Author"
             << setw(10) << "Status" << endl;
        cout << string(70, '=') << endl;

        file.open("books.dat", ios::binary | ios::in);
        Book book;
       
        while (file.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
            book.report();
        }
       
        file.close();
        pressEnterToContinue();
    }

    void displayAllStudents() {
        clearScreen();
        cout << "\n\n\t\tSTUDENT LIST\n\n";
        cout << string(70, '=') << endl;
        cout << left << setw(15) << "Admission No."
             << setw(25) << "Name"
             << setw(15) << "Book Issued"
             << setw(15) << "Book No." << endl;
        cout << string(70, '=') << endl;

        file.open("students.dat", ios::binary | ios::in);
        Student student;
       
        while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
            student.report();
        }
       
        file.close();
        pressEnterToContinue();
    }

    void issueBook() {
        string admNo, bookNo;
        bool found = false;
       
        clearScreen();
        cout << "\n\nBOOK ISSUE...\n";
        cout << "\nEnter admission no. of student: ";
        cin >> admNo;

        Student student;
        Book book;
       
        if (!findStudent(admNo, student)) {
            cout << "\nStudent record not found!";
            pressEnterToContinue();
            return;
        }

        if (student.getHasBook()) {
            cout << "\nStudent already has a book issued!";
            cout << "\nPlease return the current book first.";
            pressEnterToContinue();
            return;
        }

        cout << "\nEnter book no. to issue: ";
        cin >> bookNo;

        if (!findBook(bookNo, book)) {
            cout << "\nBook not found!";
            pressEnterToContinue();
            return;
        }

        if (book.getIsIssued()) {
            cout << "\nBook is already issued!";
            pressEnterToContinue();
            return;
        }

        // Update book status
        book.setIssuedStatus(true);
        // Update student record
        student.issueBook(bookNo);

        // Update files
        updateBookFile(book);
        updateStudentFile(student);

        cout << "\nBook issued successfully!";
        pressEnterToContinue();
    }

    void returnBook() {
        string admNo;
       
        clearScreen();
        cout << "\n\nBOOK RETURN...\n";
        cout << "\nEnter admission no. of student: ";
        cin >> admNo;

        Student student;
        Book book;
       
        if (!findStudent(admNo, student)) {
            cout << "\nStudent record not found!";
            pressEnterToContinue();
            return;
        }

        if (!student.getHasBook()) {
            cout << "\nNo book is issued to this student!";
            pressEnterToContinue();
            return;
        }

        if (!findBook(student.getIssuedBookNo(), book)) {
            cout << "\nError: Book record not found!";
            pressEnterToContinue();
            return;
        }

        // Calculate fine if any (15 days limit)
        time_t now = time(0);
        double days = difftime(now, student.getIssueDate()) / (24*60*60);
        if (days > 15) {
            int fine = static_cast<int>((days - 15) * 1); // Rs. 1 per day
            cout << "\nFine: Rs. " << fine << " (Book returned after " << static_cast<int>(days) << " days)";
        }

        // Update book status
        book.setIssuedStatus(false);
        // Update student record
        student.returnBook();

        // Update files
        updateBookFile(book);
        updateStudentFile(student);

        cout << "\nBook returned successfully!";
        pressEnterToContinue();
    }

private:
    void updateBookFile(const Book& book) {
        fstream file("books.dat", ios::binary | ios::in | ios::out);
        Book tempBook;
        while (file.read(reinterpret_cast<char*>(&tempBook), sizeof(Book))) {
            if (tempBook.getBookNo() == book.getBookNo()) {
                int pos = -1 * static_cast<int>(sizeof(Book));
                file.seekp(pos, ios::cur);
                file.write(reinterpret_cast<const char*>(&book), sizeof(Book));
                break;
            }
        }
        file.close();
    }

    void updateStudentFile(const Student& student) {
        fstream file("students.dat", ios::binary | ios::in | ios::out);
        Student tempStudent;
        while (file.read(reinterpret_cast<char*>(&tempStudent), sizeof(Student))) {
            if (tempStudent.getAdmissionNo() == student.getAdmissionNo()) {
                int pos = -1 * static_cast<int>(sizeof(Student));
                file.seekp(pos, ios::cur);
                file.write(reinterpret_cast<const char*>(&student), sizeof(Student));
                break;
            }
        }
        file.close();
    }
};

// Menu functions
void adminMenu(LibrarySystem& lib) {
    char choice;
    do {
        clearScreen();
        cout << "\n\nADMINISTRATOR MENU";
        cout << "\n1. Add Book";
        cout << "\n2. Add Student";
        cout << "\n3. Display All Books";
        cout << "\n4. Display All Students";
        cout << "\n5. Back to Main Menu";
        cout << "\n\nEnter your choice (1-5): ";
        cin >> choice;

        switch (choice) {
            case '1': lib.writeBook(); break;
            case '2': lib.writeStudent(); break;
            case '3': lib.displayAllBooks(); break;
            case '4': lib.displayAllStudents(); break;
            case '5': return;
            default: cout << "\nInvalid choice!";
        }
    } while (true);
}

int main() {
    LibrarySystem lib;
    char choice;

    do {
        clearScreen();
        cout << "\n\nLIBRARY MANAGEMENT SYSTEM";
        cout << "\n1. Issue Book";
        cout << "\n2. Return Book";
        cout << "\n3. Administrator Menu";
        cout << "\n4. Exit";
        cout << "\n\nEnter your choice (1-4): ";
        cin >> choice;

        switch (choice) {
            case '1':
                lib.issueBook();
                break;
            case '2':
                lib.returnBook();
                break;
            case '3':
                adminMenu(lib);
                break;
            case '4':
                cout << "\nThank you for using Library Management System!\n";
                return 0;
            default:
                cout << "\nInvalid choice!";
                pressEnterToContinue();
        }
    } while (true);

    return 0;
}
