#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>

class LMSDatabase;

class Student : public User 
{

private:
    int enrolledCourses[10];
    int courseCount;

public:
    Student(int id = 0, const char* name = "", const char* username = "", const char* password = "");

    int getStudentId() const { return getID(); } 
    void enrollCourse(int courseId);
    void enrollCourse(int courseId, const char* note);
    void dropCourse(int courseId);
    void viewEnrolledCourses();

    bool operator==(const Student& other) const;

    void menu() override; 
    
    
    void viewAssignments(int courseId);
    void submitAssignment(int courseId);
    void viewGrades();
    void saveProgress();
    void loadProgress(); 
    
  
    std::string serialize() const;
    void deserialize(const std::string& data);

   
    int getCourseCount() const { return courseCount; }
    const int* getEnrolledCourses() const { return enrolledCourses; }
    void addEnrolledCourseDirect(int cid); 
    
    static void setDatabase(LMSDatabase* database);
};

#endif // STUDENT_H
