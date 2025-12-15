#pragma once
#include <string>
#include "Course.h"

using namespace std;

class OnsiteCourse : public Course 
{
private:
    string room;
public:
    OnsiteCourse(int id, const char* name, string roomNumber);

    string getRoom() const { return room; }

    void displayCourse() override;
    std::string getCourseType() const override { return "Onsite"; }
    std::string getExtraDetails() const override { return room; }
};

