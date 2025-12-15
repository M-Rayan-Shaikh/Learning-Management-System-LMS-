
#include "Assignment.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip> 

using namespace std;

Assignment::Assignment() : assignID(0), courseID(0), deadline(0), totalMarks(0) 
{
   strcpy_s(title, sizeof(title), "");
}
Assignment::Assignment(int id, int cID, const char* title, const char* desc, int marks)
    : assignID(id), courseID(cID), totalMarks(marks) {

    strncpy_s(this->title, sizeof(this->title), title, 50 - 1);
    this->title[50 - 1] = '\0';

    time(&deadline);
}


void Assignment::setDeadline(time_t newdeadline) {
    deadline = newdeadline;
    cout << "Deadline updated for Assignment " << assignID << "." << endl;
}

void Assignment::viewAssignment() {
    char deadline_buffer[30];
    struct tm timeinfo;
    localtime_s(&timeinfo, &deadline);
    strftime(deadline_buffer, sizeof(deadline_buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

    cout << "\n--- Assignment: " << title << " ---" << endl;
    cout << "  ID: " << assignID << ", Course ID: " << courseID << endl;
    cout << "  Marks: " << totalMarks << endl;
    cout << "  Deadline: " << deadline_buffer << endl;
}