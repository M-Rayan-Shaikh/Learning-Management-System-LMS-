
#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <iostream>
#include <cstring>
#include <ctime> 

using namespace std;



class Submission {
private:
    int submissionID;
    int assignID;
    int studentID;
    char content[200];
    time_t dateSubmitted;
    int marks;

public:
    Submission();
    Submission(int subID, int aID, int sID, const char* content);


    void submitWork();
    void setMarks(int marks);

    int getMarks() const;
    
    int getAssignID() const { return assignID; }
    int getStudentID() const { return studentID; }
    const char* getContent() const { return content; }
};

#endif // SUBMISSION_H
