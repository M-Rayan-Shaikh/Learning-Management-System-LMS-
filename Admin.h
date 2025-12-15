#ifndef ADMIN_H
#define ADMIN_H

#include<iostream>
#include "User.h"

class LMSDatabase;  

class Admin : public User 
{
public:
    Admin(int id = 0, const char* name = "", const char* username = "", const char* password = "");

    void createUser();
    void deleteUser();
    void editUser();

    void createCourse();
    void removeCourse();
    void assignTeacherToCourse();

    void viewAllUsers();
    void viewAllCourses();

    void saveData();
    void loadData();
    static void setDatabase(LMSDatabase* database);  

    void menu() override;    
};

#endif 
