#include "OnlineCourse.h"
#include "Teacher.h"
#include "Course.h"
#include <string>
#include <iostream>
using namespace std;

OnlineCourse::OnlineCourse(int id, string name, string link) : Course(id, name.c_str()), meetingLink(link) {}

void OnlineCourse::displayCourse() {
    Course::displayCourse();
    cout << "Type: Online" << endl;
    cout << "Meeting Link: " << meetingLink << endl;
}