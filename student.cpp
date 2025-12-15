#include "student.h"
#include "LMSDatabase.h"
#include "Course.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;


Student::Student(int id, const char* name, const char* username, const char* password)  : User(id, (char*)name, (char*)username, (char*)password), courseCount(0) 
{
    
    for(int i=0; i<10; ++i) enrolledCourses[i] = 0;
}

void Student::enrollCourse(int courseId)
{
    if (courseCount >= 10)
    {
        cout << "Cannot enroll: max courses reached.\n";
        return;
    }
    for (int i = 0; i < courseCount; ++i)
        if (enrolledCourses[i] == courseId)
        {
            cout << "Already enrolled in course " << courseId << ".\n";
            return;
        }
    enrolledCourses[courseCount++] = courseId;
    cout << "Enrolled in course " << courseId << ".\n";
}

void Student::enrollCourse(int courseId, const char* note)
{
    enrollCourse(courseId);
    cout << "Note: " << note << "\n";
}

void Student::dropCourse(int courseId)
{
    for (int i = 0; i < courseCount; ++i)
    {
        if (enrolledCourses[i] == courseId)
        {
            for(int j = i; j < courseCount - 1; j++) {
                enrolledCourses[j] = enrolledCourses[j+1];
            }
            enrolledCourses[courseCount - 1] = 0;
            --courseCount;
            cout << "Dropped course " << courseId << ".\n";
            return;
        }
    }
    cout << "Course " << courseId << " not found.\n";
}

void Student::viewEnrolledCourses()
{
    cout << "Student " << getName() << " (" << getID() << ") enrolled courses:";
    if (courseCount == 0) cout << " (none)";
    for (int i = 0; i < courseCount; ++i) cout << ' ' << enrolledCourses[i];
    cout << "\n";
}

bool Student::operator==(const Student& other) const
{
    return getID() == other.getID();
}

void Student::addEnrolledCourseDirect(int cid) {
    if(courseCount < 10) {
        enrolledCourses[courseCount++] = cid;
    }
}

std::string Student::serialize() const 
{
    stringstream ss;
    string safeName = getName();
    for(char &c : safeName) {
        if(c == ' ') c = '_';
    }
    
    ss << getID() << " " << safeName << " " << username << " " << password << " " << courseCount;
    for(int i=0; i<courseCount; ++i) {
        ss << " " << enrolledCourses[i];
    }
    return ss.str();
}

void Student::menu()
{
    int choice;
    do {
        cout << "\n===== STUDENT MENU: " << getName() << " =====\n";
        cout << "1. Enroll in course\n";
        cout << "2. Enroll in course with note\n";
        cout << "3. Drop course\n";
        cout << "4. View enrolled courses\n";
        cout << "5. View Assignments\n";
        cout << "6. Submit Assignment\n";
        cout << "7. View Grades\n";
        cout << "0. Logout\n";
        cout << "Select option: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int cid; cout << "Course ID: "; cin >> cid;
            enrollCourse(cid);
            break;
        }
        case 2:
        {
            int cid; cin >> ws;
            cout << "Course ID: "; cin >> cid;
            cin.ignore();
            string note;
            cout << "Note: "; getline(cin, note);
            enrollCourse(cid, note.c_str());
            break;
        }
        case 3:
        {
            int cid; cout << "Course ID to drop: "; cin >> cid;
            dropCourse(cid);
            break;
        }
        case 4:
            viewEnrolledCourses();
            break;
        case 5:
        {
            int cid; cout << "Course ID: "; cin >> cid;
            viewAssignments(cid);
            break;
        }
        case 6:
        {
            int cid; cout << "Course ID: "; cin >> cid;
            submitAssignment(cid);
            break;
        }
        case 7:
            viewGrades(); 
            break;
        case 0:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}


static LMSDatabase* db = nullptr;
void Student::setDatabase(LMSDatabase* database) 
{
    db = database;
}

void Student::viewAssignments(int courseId) {
    if(!db) { cout << "Error: DB not connected.\n"; return; }
    
    bool enrolled = false;
    for(int i=0; i<courseCount; ++i) {
        if(enrolledCourses[i] == courseId) enrolled = true;
    }
    
    if(!enrolled) {
        cout << "You are not enrolled in Course " << courseId << ".\n";
        return;
    }

    cout << "--- Assignments for Course " << courseId << " ---\n";
    Assignment** allAssigns = db->getAssignments();
    int aCount = db->getAssignmentCount();
    bool found = false;

    for(int i=0; i<aCount; ++i) {
        if(allAssigns[i] && allAssigns[i]->getCourseID() == courseId) {
            cout << "ID: " << allAssigns[i]->getAssignID() 
                 << " Title: " << allAssigns[i]->getTitle() 
                 << " Max Marks: " << allAssigns[i]->getTotalMarks() << "\n";
            found = true;
        }
    }
    if(!found) cout << "No assignments found.\n";
}

void Student::submitAssignment(int courseId) {
    if(!db) { cout << "Error: DB not connected.\n"; return; }
    
    bool enrolled = false;
    for(int i=0; i<courseCount; ++i) {
        if(enrolledCourses[i] == courseId) enrolled = true;
    }
    if(!enrolled) { cout << "Not enrolled.\n"; return; }
    
    viewAssignments(courseId);

    int assignId;
    cout << "Enter Assignment ID to submit to: ";
    cin >> assignId;

    Assignment** allAssigns = db->getAssignments();
    int aCount = db->getAssignmentCount();
    bool valid = false;
    for(int i=0; i<aCount; ++i) {
        if(allAssigns[i] && allAssigns[i]->getAssignID() == assignId && allAssigns[i]->getCourseID() == courseId) {
            valid = true;
            break;
        }
    }

    if(!valid) 
    {
        cout << "Invalid Assignment ID for this course.\n";
        return;
    }

    Submission** subs = db->getSubmissions();
    int sCount = db->getSubmissionCount();
    for(int i=0; i<sCount; ++i) {
        if(subs[i] && subs[i]->getAssignID() == assignId && subs[i]->getStudentID() == getID()) {
            cout << "You have already submitted for this assignment.\n";
            return;
        }
    }

    string content;
    cout << "Enter submission text: ";
    cin.ignore();
    getline(cin, content); 

   
    int newSubId = sCount; 
    Submission* newSub = new Submission(newSubId, assignId, getID(), content.c_str());
    db->addSubmission(newSub);

    cout << "Assignment submitted successfully.\n";
}

void Student::viewGrades() 
{
    if(!db) { cout << "Error: DB not connected.\n"; return; }

    cout << "\n--- Grades for Student " << getName() << " ---\n";
    
    Submission** subs = db->getSubmissions();
    int sCount = db->getSubmissionCount();
    Assignment** assigns = db->getAssignments();
    int aCount = db->getAssignmentCount();
    
    bool hasGrades = false;

  
    for(int i=0; i<sCount; ++i) {
        if(subs[i] && subs[i]->getStudentID() == getID()) {
            int aId = subs[i]->getAssignID();
            string title = "Unknown";
            int cId = -1;
            
            for(int k=0; k<aCount; ++k) {
                if(assigns[k] && assigns[k]->getAssignID() == aId) {
                    title = assigns[k]->getTitle();
                    cId = assigns[k]->getCourseID();
                    break;
                }
            }

            cout << "Course: " << cId << " | Assignment: " << title 
                 << " | Marks: " << subs[i]->getMarks() << "\n";
            hasGrades = true;
        }
    }

    if(!hasGrades) cout << "No grades/submissions found.\n";
}

void Student::saveProgress() {
    if(db) {
        db->saveStudents();
        cout << "Student progress saved to students.txt\n";
    } else {
        cout << "Error: Database link missing.\n";
    }
}

void Student::loadProgress() 
{
    if(db) 
    {
        db->loadStudents();
        cout << "Student progress loaded from students.txt\n";
    } else {
        cout << "Error: Database link missing.\n";
    }
}
