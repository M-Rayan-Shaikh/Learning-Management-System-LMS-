#include "User.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

User::User()
{
    id = 0;
    strcpy_s(name, sizeof(name), "    ");
    strcpy_s(username, sizeof(username), "   ");
    strcpy_s(password, sizeof(password), "0000");
}

User::User(int id, char* name, char* username, char* password) {
    this->id = id;
    strcpy_s(this->name, sizeof(this->name), name);
    strcpy_s(this->username, sizeof(this->username), username);
    strcpy_s(this->password, sizeof(this->password), password);
}

void User::setUser() 
{
    int choice;
    string file;
    string username, password;

    cout << "Select User Type to create:" << endl;
    cout << "1. Student" << endl;
    cout << "2. Teacher" << endl;
    cout << "3. Admin" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    switch (choice) 
    {
    case 1: file = "student.txt"; break;
    case 2: file = "teacher.txt"; break;
    case 3: file = "admin.txt"; break;
    default:
        cout << "Invalid choice. Account creation cancelled." << endl;
        return;
    }

    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    ofstream outfile(file, ios::app);

    if (outfile.is_open()) 
    {
        outfile << username << " " << password << endl;
        outfile.close();
        cout << "SUCCESS: New " << file.substr(0, file.find('.'))
            << " account created and saved." << endl;
    }
    else {
        cerr << "ERROR: Could not open file " << file << " for writing." << endl;
    }
}


int User::getID() const 
{
    return id;
}

const char* User::getName() const 
{
    return name;
}

bool User::login() 
{
    cout << "User logged in." << endl;
    return true;
}

void User::logout() {
    cout << "User logged out." << endl;
}
