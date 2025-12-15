#pragma once
#include <string>
#include "Course.h"

using namespace std;

class OnlineCourse : public Course {
private:
    string meetingLink;

public:
    OnlineCourse(int id, string name, string link);
    string getMeetingLink() const { return meetingLink; }

    void displayCourse() override;
    
    std::string getCourseType() const override { return "Online"; }
    std::string getExtraDetails() const override { return meetingLink; }
};

