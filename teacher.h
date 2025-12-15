#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include "User.h"

class LMSDatabase;

class Teacher : public User
{
private:
    int coursesTaught[10];
    int teachCount;

public:
    Teacher(int id = 0, const char* name = "", const char* username = "", const char* password = "");

    int getTeacherId() const { return getID(); }

    void addCourse(int courseId);
    void viewMyCourses();

    void menu() override;

    void createAssignment(int courseId);
    void removeAssignment(int courseId);
    void viewSubmissions(int courseId);
    void gradeSubmission(int courseId, int studentId);
    void saveTeacherData();
    void loadTeacherData();
    
    std::string serialize() const;
    
    int getTeachCount() const { return teachCount; }
    const int* getCoursesTaught() const { return coursesTaught; }
    void addCourseDirect(int cid);
    
    static void setDatabase(LMSDatabase* database);
};

#endif // TEACHER_H
