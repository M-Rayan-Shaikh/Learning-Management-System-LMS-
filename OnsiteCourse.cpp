#include "OnsiteCourse.h"
#include "Course.h"
#include "Teacher.h"
#include "User.h"
#include "student.h"
#include <cstring>
#include <string.h>
#include <iostream>
using namespace std;

OnsiteCourse::OnsiteCourse(int id, const char* name, string roomNumber) : Course(id, name), room(roomNumber) {}

void OnsiteCourse::displayCourse() 
{
    Course::displayCourse();
    cout << "Type: Onsite" << endl;
    cout << "Room: " << room << endl;
}