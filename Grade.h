
#ifndef GRADE_H
#define GRADE_H

#include <iostream>
#include <cstring>

using namespace std;

class Grade {
private:
    int studentID;
    int courseID;
    int totalMarks;
    int totalAssignments;

public:

    Grade();
    Grade(int sID, int cID);

    int getStudentID() const { return studentID; }
    int getCourseID() const { return courseID; }


    void addMarks(int marks);
    float calculateTotal() const;
    void displayGrade();
};

#endif 
