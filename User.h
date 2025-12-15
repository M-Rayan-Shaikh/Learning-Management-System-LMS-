#ifndef USER_H
#define USER_H

#include <cstring>

class User {
protected:
    int id;
    char name[50];
    char username[30];
    char password[30];

public:
    User();

    User(int id, char* name, char* username, char* password);
    void setUser();

    int getID() const;
    const char* getName() const; 
    const char* getUsername() const { return username; } 
    const char* getPassword() const { return password; } 

    void setName(const char* n) { strcpy_s(name, sizeof(name), n); }
    void setUsername(const char* u) { strcpy_s(username, sizeof(username), u); }
    void setPassword(const char* p) { strcpy_s(password, sizeof(password), p); } 

 
    bool checkCredentials(const char* u, const char* p) {
        return (strcmp(username, u) == 0 && strcmp(password, p) == 0);
    }

    bool login();
    void logout();

    virtual void menu() = 0;
};

#endif // USER_H
