#include "Grade.h"
#include <iostream>
#include <cmath> 

using namespace std;

Grade::Grade()
    : studentID(0), courseID(0), totalMarks(0), totalAssignments(0) {
}

Grade::Grade(int sID, int cID)
    : studentID(sID), courseID(cID), totalMarks(0), totalAssignments(0) {
}

void Grade::addMarks(int marks) {
    totalMarks += marks;
    totalAssignments++;
    cout << "Added " << marks << " marks for Student " << studentID << ". New total: " << totalMarks << endl;
}

float Grade::calculateTotal() const {
    return totalMarks;
}

void Grade::displayGrade() {
    cout << "\n--- Grade for Student " << studentID << " in Course " << courseID << " ---" << endl;
    cout << "  Total Earned Marks: " << totalMarks << endl;
    cout << "  Assignments Graded: " << totalAssignments << endl;
}