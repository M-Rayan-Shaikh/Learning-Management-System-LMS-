
#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <iostream>
#include <cstring>
#include <ctime> 

using namespace std;


class Assignment 
{
private:
    int assignID;
    int courseID;
    char title[50];
    time_t deadline;
    int totalMarks;

public:
    Assignment();
    Assignment(int id, int cID, const char* title, const char* desc, int marks);

    int getAssignID() const { return assignID; }
    int getCourseID() const { return courseID; }
    char* getTitle() { return title; }
    int getTotalMarks() const { return totalMarks; }
    void viewAssignment();
    void setDeadline(time_t new_deadline);
};

#endif 
