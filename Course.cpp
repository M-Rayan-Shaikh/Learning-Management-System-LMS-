#include "Course.h"
#include <iostream>
#include <cstring>  
using namespace std;

Course::Course() {
    courseID = 0;
    strcpy_s(courseName, sizeof(courseName), " ");
    teacherID = 0;
    enrolledStudents = 0;
    assignmentCount = 0;
}

Course::Course(int id, const char* name) 
{
    courseID = id;
    strncpy_s(courseName, sizeof(courseName), name, 49);
    courseName[49] = '\0';
    teacherID = 0;
    enrolledStudents = 0;
    assignmentCount = 0;
}

void Course::setCourseID(int CourseID) 
{
    courseID = CourseID;
}

void Course::setCourseName(const char* CourseName) {
    strncpy_s(courseName, sizeof(courseName), CourseName, 49);
    courseName[49] = '\0';
}

int Course::getCourseID() const {
    return courseID;
}

const char* Course::getCourseName() const 
{
    return courseName;
}

void Course::enrollStudent() 
{
    enrolledStudents++;
    cout << "Student enrolled in " << getCourseName() << ". Total assignments: " << assignmentCount << endl;
}

void Course::addAssignment() 
{
    assignmentCount++;
    cout << "Assignment added to " << getCourseName() << ". Total assignments: " << assignmentCount << endl;
}

void Course::removeStudent() 
{
    if (enrolledStudents > 0) 
    {
        enrolledStudents--;
        cout << "Student removed from " << getCourseName() << ". Total students: " << enrolledStudents << endl;
    }
    else {
        cout << "Error: No students enrolled to remove." << endl;
    }
}

void Course::displayCourse() 
{
    cout << "Course ID: " << courseID << endl;
    cout << "Course Name: " << courseName << endl;
    cout << "Teacher ID: " << teacherID << endl;
    cout << "Enrolled Students: " << enrolledStudents << endl;
    cout << "Assignments: " << assignmentCount << endl;
}
