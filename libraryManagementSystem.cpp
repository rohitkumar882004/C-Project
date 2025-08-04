#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Book {
public:
    int bookID;
    string title;
    string author;
    bool issued;

    void input() {
        cout << "Enter Book ID: "; cin >> bookID;
        cin.ignore();
        cout << "Enter Title: "; getline(cin, title);
        cout << "Enter Author: "; getline(cin, author);
        issued = false;
    }

    void display() const {
        cout << "\nBook ID: " << bookID
             << "\nTitle: " << title
             << "\nAuthor: " << author
             << "\nIssued: " << (issued ? "Yes" : "No") << endl;
    }
};

void addBook() {
    Book book;
    ofstream outFile("books.dat", ios::binary | ios::app);
    book.input();
    outFile.write(reinterpret_cast<char*>(&book), sizeof(book));
    outFile.close();
    cout << "Book added successfully.\n";
}

void displayAllBooks() {
    Book book;
    ifstream inFile("books.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&book), sizeof(book))) {
        book.display();
    }
    inFile.close();
}

void searchBook(int id) {
    Book book;
    ifstream inFile("books.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&book), sizeof(book))) {
        if (book.bookID == id) {
            book.display();
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found)
        cout << "Book not found.\n";
}

void issueBook(int id) {
    fstream file("books.dat", ios::binary | ios::in | ios::out);
    Book book;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&book), sizeof(book)) && !found) {
        if (book.bookID == id && !book.issued) {
            book.issued = true;
            int pos = -1 * static_cast<int>(sizeof(book));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&book), sizeof(book));
            found = true;
            cout << "Book issued successfully.\n";
        }
    }
    file.close();
    if (!found)
        cout << "Book not found or already issued.\n";
}

void returnBook(int id) {
    fstream file("books.dat", ios::binary | ios::in | ios::out);
    Book book;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&book), sizeof(book)) && !found) {
        if (book.bookID == id && book.issued) {
            book.issued = false;
            int pos = -1 * static_cast<int>(sizeof(book));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&book), sizeof(book));
            found = true;
            cout << "Book returned successfully.\n";
        }
    }
    file.close();
    if (!found)
        cout << "Book not found or not issued.\n";
}

int main() {
    int choice, id;

    do {
        cout << "\n--- Library Management System ---\n";
        cout << "1. Add Book\n2. Display All Books\n3. Search Book\n4. Issue Book\n5. Return Book\n6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayAllBooks(); break;
            case 3:
                cout << "Enter Book ID to search: ";
                cin >> id;
                searchBook(id);
                break;
            case 4:
                cout << "Enter Book ID to issue: ";
                cin >> id;
                issueBook(id);
                break;
            case 5:
                cout << "Enter Book ID to return: ";
                cin >> id;
                returnBook(id);
                break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}
