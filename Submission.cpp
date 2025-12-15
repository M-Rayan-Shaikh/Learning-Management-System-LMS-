// Submission.cpp

#include "Submission.h"
#include <iostream>
#include <cstring> 
#include <ctime>   

using namespace std;
Submission::Submission(): submissionID(0), assignID(0), studentID(0), dateSubmitted(0), marks(-1) 
{ 
    strcpy_s(content, sizeof(content), "");
}
Submission::Submission(int subID, int aID, int sID, const char* input_content) : submissionID(subID), assignID(aID), studentID(sID), marks(-1) {
    strncpy_s(this->content, sizeof(this->content), input_content, 200 - 1);
    this->content[200 - 1] = '\0';

    time(&dateSubmitted);
}


void Submission::submitWork() 
{
    char new_content[200];
    cout << "Enter new submission content (max " << 200 - 1 << " chars):" << endl;

    cin.ignore();
    cin.getline(new_content, 200);

    strncpy_s(this->content, sizeof(this->content), new_content, 200 - 1);
    this->content[200 - 1] = '\0';

    time(&dateSubmitted);
    char timeStr[26];
    ctime_s(timeStr, sizeof(timeStr), &dateSubmitted);
    cout << "Submission ID " << submissionID << " updated successfully on " << timeStr;
}

void Submission::setMarks(int newMarks) 
{
    marks = newMarks;
   
}

int Submission::getMarks() const 
{
    return marks;
}