#include <iostream>
#include <string>
#include <cstring>
#include "Admin.h"
#include "student.h"
#include "teacher.h"
#include "OnsiteCourse.h"
#include "OnlineCourse.h"
#include "LMSDatabase.h" 
#include "Course.h"     
using namespace std;


static LMSDatabase* db = nullptr;

void Admin::setDatabase(LMSDatabase* database) 
{
    db = database;
}

Admin::Admin(int id, const char* name, const char* username, const char* password) 
    : User(id, (char*)name, (char*)username, (char*)password) 
{
}
void Admin::menu() {
    int choice;
    do {
        std::cout << "\n--- ADMIN DASHBOARD: " << getName() << " ---" << std::endl;
        std::cout << "1. Create User" << std::endl;
        std::cout << "2. Delete User" << std::endl;
        std::cout << "3. Create Course" << std::endl;
        std::cout << "4. Remove Course" << std::endl;
        std::cout << "5. Assign Teacher to Course" << std::endl;
        std::cout << "6. View All Users" << std::endl;
        std::cout << "7. View All Courses" << std::endl;
        std::cout << "8. Save & Exit" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: createUser(); break;
        case 2: deleteUser(); break;
        case 3: createCourse(); break;
        case 4: removeCourse(); break;
        case 5: assignTeacherToCourse(); break;
        case 6: viewAllUsers(); break;
        case 7: viewAllCourses(); break;
        case 8: saveData(); break;
        default: std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 8);
}

void Admin::createUser() {
    int type;
    std::cout << "Enter User Type (1. Student, 2. Teacher): ";
    std::cin >> type;

    int id;
    std::string name, user, pass;
    std::cout << "Enter ID: "; std::cin >> id;
    std::cin.ignore();
    std::cout << "Enter Name: "; std::getline(std::cin, name);
    std::cout << "Enter Username: "; std::cin >> user;
    std::cout << "Enter Password: "; std::cin >> pass;

    if (!db) 
    {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }
    if (type == 1) {
        Student* s = new Student(id, name.c_str(), user.c_str(), pass.c_str());
        db->addStudent(s);
    }
    else if (type == 2) {
        Teacher* t = new Teacher(id, name.c_str(), user.c_str(), pass.c_str());
        db->addTeacher(t);
    }
    else 
    {
        std::cout << "Invalid user type. User creation cancelled." << std::endl;
    }
}
void Admin::createCourse() {
    int type;
    std::cout << "Enter Course Type (1. Onsite, 2. Online): ";
    std::cin >> type;
    int id;
    std::string name;
    std::cout << "Enter Course ID: "; std::cin >> id;
    std::cin.ignore();
    std::cout << "Enter Course Name: "; std::getline(std::cin, name);

    if (!db) {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }
    if (type == 1) {
        std::string room;
        std::cout << "Enter Room Number: "; std::cin >> room;
        OnsiteCourse* c = new OnsiteCourse(id, name.c_str(), room);
        db->addCourse(c);
    }
    else if (type == 2) {
        std::string link;
        std::cout << "Enter Meeting Link: "; std::cin >> link;
        OnlineCourse* c = new OnlineCourse(id, name, link);
        db->addCourse(c);
    }
    else {
        std::cout << "Invalid course type. Course creation cancelled." << std::endl;
    }
}
void Admin::assignTeacherToCourse() {
    int teacherId, courseId;
    std::cout << "  Assign Teacher to Course  " << std::endl;
    std::cout << "Enter Teacher ID to assign: ";
    std::cin >> teacherId;
    std::cout << "Enter Course ID: ";
    std::cin >> courseId;

    if (!db) {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }
    Teacher* foundTeacher = nullptr;
    Course* foundCourse = nullptr;

    Teacher** allTeachers = db->getTeachers();
    int numTeachers = db->getTeacherCount();
    for (int i = 0; i < numTeachers; ++i) {
        foundTeacher = allTeachers[i]; 
        break; 
    }

    Course** allCourses = db->getCourses();
    int numCourses = db->getCourseCount();

    for (int i = 0; i < numCourses; ++i) {
        if (allCourses[i] && allCourses[i]->getCourseID() == courseId) {
            foundCourse = allCourses[i];
            break;
        }
    }

    if (foundTeacher == nullptr) {
        std::cout << "Error: Teacher with ID " << teacherId << " not found." << std::endl;
        return;
    }

    if (foundCourse == nullptr) {
        std::cout << "Error: Course with ID " << courseId << " not found." << std::endl;
        return;
    }

    foundTeacher->addCourse(courseId);

    std::cout << "Teacher assigned to Course " << foundCourse->getCourseName() << "." << std::endl;
}

void Admin::deleteUser() {
    int idToDelete;
    int type;
    std::cout << "\n--- Delete User ---" << std::endl;
    std::cout << "Enter User Type (1. Student, 2. Teacher): ";
    std::cin >> type;
    std::cout << "Enter ID of user to delete: ";
    std::cin >> idToDelete;

    if (!db) {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }

    if (type == 1) {
        db->removeStudent(idToDelete);
    } 
    else if (type == 2) {
        db->removeTeacher(idToDelete);
    }
    else {
        std::cout << "Invalid user type." << std::endl;
    }
}

void Admin::removeCourse() {
    int courseId;
    std::cout << "\n--- Remove Course ---" << std::endl;
    std::cout << "Enter Course ID to remove: ";
    std::cin >> courseId;

    if (!db) {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }
    db->removeCourse(courseId);
}

void Admin::editUser() {
    if (!db) {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }

    int type;
    int idToEdit;
    std::cout << "\n--- Edit User ---" << std::endl;
    std::cout << "Enter User Type (1. Student, 2. Teacher): ";
    std::cin >> type;
    std::cout << "Enter ID of user to edit: ";
    std::cin >> idToEdit;

    if (type == 1) {
        Student** allStudents = db->getStudents();
        int numStudents = db->getStudentCount();
        Student* foundStudent = nullptr;

        for (int i = 0; i < numStudents; ++i) {
            if (allStudents[i] && allStudents[i]->getStudentId() == idToEdit) {
                foundStudent = allStudents[i];
                break;
            }
        }

        if (foundStudent == nullptr) {
            std::cout << "Error: Student with ID " << idToEdit << " not found." << std::endl;
            return;
        }

        std::cout << "\nStudent found. Current information:" << std::endl;
        foundStudent->viewEnrolledCourses();

        int choice;
        std::cout << "\nWhat would you like to edit?" << std::endl;
        std::cout << "1. Enroll in a course" << std::endl;
        std::cout << "2. Drop a course" << std::endl;
        std::cout << "0. Cancel" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int courseId;
            std::cout << "Enter Course ID to enroll: ";
            std::cin >> courseId;
            foundStudent->enrollCourse(courseId);
            std::cout << "Student updated successfully." << std::endl;
            break;
        }
        case 2: {
            int courseId;
            std::cout << "Enter Course ID to drop: ";
            std::cin >> courseId;
            foundStudent->dropCourse(courseId);
            std::cout << "Student updated successfully." << std::endl;
            break;
        }
        case 0:
            std::cout << "Edit cancelled." << std::endl;
            break;
        default:
            std::cout << "Invalid choice." << std::endl;
        }
    }
    else if (type == 2) {
        Teacher** allTeachers = db->getTeachers();
        int numTeachers = db->getTeacherCount();
        Teacher* foundTeacher = nullptr;

        for (int i = 0; i < numTeachers; ++i) {
            if (allTeachers[i] && allTeachers[i]->getTeacherId() == idToEdit) {
                foundTeacher = allTeachers[i];
                break;
            }
        }

        if (foundTeacher == nullptr) {
            std::cout << "Error: Teacher with ID " << idToEdit << " not found." << std::endl;
            return;
        }

        std::cout << "\nTeacher found. Current information:" << std::endl;
        foundTeacher->viewMyCourses();

        int choice;
        std::cout << "\nWhat would you like to edit?" << std::endl;
        std::cout << "1. Add a course" << std::endl;
        std::cout << "0. Cancel" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int courseId;
            std::cout << "Enter Course ID to add: ";
            std::cin >> courseId;
            foundTeacher->addCourse(courseId);
            std::cout << "Teacher updated successfully." << std::endl;
            break;
        }
        case 0:
            std::cout << "Edit cancelled." << std::endl;
            break;
        default:
            std::cout << "Invalid choice." << std::endl;
        }
    }
    else {
        std::cout << "Invalid user type. Edit cancelled." << std::endl;
    }
}

void Admin::viewAllUsers() {
    if (!db) {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }

    std::cout << "\n--- ALL USERS ---" << std::endl;
    Student** allStudents = db->getStudents();
    int numStudents = db->getStudentCount();
    std::cout << "\n--- Students (" << numStudents << ") ---" << std::endl;
    for (int i = 0; i < numStudents; ++i) {
        if (allStudents[i]) {
            std::cout << "ID: " << allStudents[i]->getID() 
                      << " | Name: " << allStudents[i]->getName() << std::endl;
        }
    }

    Teacher** allTeachers = db->getTeachers();
    int numTeachers = db->getTeacherCount();
    std::cout << "\n--- Teachers (" << numTeachers << ") ---" << std::endl;
    for (int i = 0; i < numTeachers; ++i) {
        if (allTeachers[i]) {
            std::cout << "ID: " << allTeachers[i]->getID() 
                      << " | Name: " << allTeachers[i]->getName() 
                      << " | Courses: " << allTeachers[i]->getTeachCount() << std::endl;
        }
    }
    std::cout << "--------------------" << std::endl;
}

void Admin::viewAllCourses() 
{
    if (!db) {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }
    std::cout << "\n--- ALL COURSES ---" << std::endl;
    Course** allCourses = db->getCourses();
    int numCourses = db->getCourseCount();

    if (numCourses == 0) {
        std::cout << "No courses found." << std::endl;
        return;
    }

    for (int i = 0; i < numCourses; ++i) {
        if (allCourses[i]) {
            allCourses[i]->displayCourse();
            std::cout << "---" << std::endl;
        }
    }
}

void Admin::saveData() {
    if (!db) {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }
    db->saveAll();
}

void Admin::loadData() {
    if (!db) {
        std::cout << "Error: Database not initialized." << std::endl;
        return;
    }
    db->loadAll();
}
