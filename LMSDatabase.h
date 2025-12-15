#ifndef LMS_DATABASE_SYSTEM_H
#define LMS_DATABASE_SYSTEM_H

#include <iostream>
#include <fstream>
#include "student.h"
#include "teacher.h"
#include "Course.h"
#include "OnlineCourse.h"
#include "OnsiteCourse.h"
#include "User.h"
#include "Assignment.h"
#include "Submission.h"

class Admin; 

using namespace std;

class LMSDatabase {
private:

    Student* students[100];
    Teacher* teachers[20];
    Admin* admins[5];
    Course* courses[100];
    int studentCount;
    int teacherCount;
    int adminCount;
    int totalCourses;

    Assignment* assignments[200];
    int assignmentCount;
    Submission* submissions[500];
    int submissionCount;

public:
    LMSDatabase();
    void addStudent(Student* s);
    void addTeacher(Teacher* t);
    void addCourse(Course* c);
    void addAdmin(Admin* a);
    
    void addAssignment(Assignment* a);
    void addSubmission(Submission* s);
    void removeAssignment(int assignId);

    void removeStudent(int id);
    void removeTeacher(int id);
    void removeCourse(int id);

    Assignment** getAssignments() { return assignments; }
    int getAssignmentCount() { return assignmentCount; }

    Submission** getSubmissions() { return submissions; }
    int getSubmissionCount() { return submissionCount; }

    void saveAll();
    void loadAll();

    void saveStudents();
    void saveTeachers();
    void saveAdmins();
    void saveCourses();
    void saveAssignments();
    void saveSubmissions();

    void loadStudents();
    void loadTeachers();
    void loadAdmins();
    void loadCourses();
    void loadAssignments();
    void loadSubmissions();

    Teacher** getTeachers() { return teachers; }
    int getTeacherCount() { return teacherCount; }

    Student** getStudents() { return students; }
    int getStudentCount() { return studentCount; }

    Course** getCourses() { return courses; }
    int getCourseCount() { return totalCourses; }
    
    Admin** getAdmins() { return admins; }
    int getAdminCount() { return adminCount; }
};

#endif // LMS_DATABASE_SYSTEM_H
