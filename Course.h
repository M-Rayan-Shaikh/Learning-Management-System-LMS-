#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course {
private:
    int courseID;
    char courseName[50];
    int teacherID;
    int enrolledStudents;
    int assignmentCount;

public:
    Course();                 
    Course(int id, const char* name);  
    virtual ~Course() {} 

    void setCourseID(int courseID);
    void setCourseName(const char* courseName);
    int getCourseID() const;
    const char* getCourseName() const;
    void enrollStudent();    
    void addAssignment();     
    void removeStudent();     
    
    virtual void displayCourse();   
   
    virtual std::string getCourseType() const { return "Base"; }
    virtual std::string getExtraDetails() const { return ""; }
};

#endif 



