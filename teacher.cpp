#include "teacher.h"
#include "LMSDatabase.h"
#include "Course.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Teacher::Teacher(int id, const char* name, const char* username, const char* password) 
    : User(id, (char*)name, (char*)username, (char*)password), teachCount(0) 
{
    for(int i=0; i<10; ++i) coursesTaught[i] = 0;
}

void Teacher::addCourse(int courseId)
{
    if (teachCount >= 10)
    {
        cout << "Cannot add course: max reached.\n";
        return;
    }
    for (int i = 0; i < teachCount; ++i)
        if (coursesTaught[i] == courseId)
        {
            cout << "Course already added.\n";
            return;
        }
    coursesTaught[teachCount++] = courseId;
    cout << "Course " << courseId << " added.\n";
}

void Teacher::addCourseDirect(int cid) {
    if(teachCount < 10) {
        coursesTaught[teachCount++] = cid;
    }
}

void Teacher::viewMyCourses()
{
    cout << "Teacher " << getName() << " (" << getID() << ") courses:";
    if (teachCount == 0) cout << " (none)";
    for (int i = 0; i < teachCount; ++i) cout << ' ' << coursesTaught[i];
    cout << "\n";
}

std::string Teacher::serialize() const {
    stringstream ss;
    string safeName = getName();
    for(char &c : safeName) {
        if(c == ' ') c = '_';
    }

    ss << getID() << " " << safeName << " " << username << " " << password << " " << teachCount;
    for(int i=0; i<teachCount; ++i) {
        ss << " " << coursesTaught[i];
    }
    return ss.str();
}

void Teacher::menu()
{
    int choice;
    do {
        cout << "\n===== TEACHER MENU: " << getName() << " =====\n";
        cout << "1. Add course\n";
        cout << "2. View my courses\n";
        cout << "3. Create Assignment\n";
        cout << "4. View Submissions\n";
        cout << "5. Grade Submission\n";
        cout << "6. Remove Assignment\n";
        cout << "7. Save Teacher Data\n";
        cout << "8. Load Teacher Data\n";
        cout << "0. Logout\n";
        cout << "Select option: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int cid; cout << "Course ID: "; cin >> cid;
            addCourse(cid);
            break;
        }
        case 2:
            viewMyCourses();
            break;
        case 3: {
            int cid; cout << "Course ID: "; cin >> cid;
            createAssignment(cid);
            break;
        }
        case 4: {
            int cid; cout << "Course ID: "; cin >> cid;
            viewSubmissions(cid);
            break;
        }
        case 5: {
            int cid; cout << "Course ID: "; cin >> cid;
            int sid; cout << "Student ID: "; cin >> sid;
            gradeSubmission(cid, sid);
            break;
        }
        case 6: {
            int cid; cout << "Course ID: "; cin >> cid;
            removeAssignment(cid);
            break;
        }
        case 7:
            saveTeacherData();
            break;
        case 8:
            loadTeacherData();
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
void Teacher::setDatabase(LMSDatabase* database) {
    db = database;
}

void Teacher::createAssignment(int courseId) {
    if(!db) { cout << "Error: DB not connected.\n"; return; }
    
    Course** courses = db->getCourses();
    int cCount = db->getCourseCount();
    Course* target = nullptr;
    for(int i=0; i<cCount; ++i) {
        if(courses[i] && courses[i]->getCourseID() == courseId) {
            target = courses[i];
            break;
        }
    }
    
    if(target) 
    {
        char title[50];
        char desc[100];
        int marks;
        cout << "Enter Assignment Title: "; 
        cin.ignore();
        cin.getline(title, 50);
        cout << "Enter Description: ";
        cin.getline(desc, 100);
        cout << "Enter Total Marks: ";
        cin >> marks;

        int newId = db->getAssignmentCount() + 1000; 

        Assignment* newAssign = new Assignment(newId, courseId, title, desc, marks);
        db->addAssignment(newAssign);
        
        target->addAssignment(); 
        cout << "Assignment Created! ID: " << newId << "\n";

    } else {
        cout << "Course " << courseId << " not found.\n";
    }
}

void Teacher::removeAssignment(int courseId) {
     if(!db) { cout << "Error: DB not connected.\n"; return; }
     
     cout << "\n--- Assignments for Course " << courseId << " ---\n";
     Assignment** allAssigns = db->getAssignments();
     int count = db->getAssignmentCount();
     bool found = false;

     for(int i=0; i<count; ++i) {
         if(allAssigns[i] && allAssigns[i]->getCourseID() == courseId) {
             allAssigns[i]->viewAssignment();
             found = true;
         }
     }

     if(!found) {
         cout << "No assignments found for this course.\n";
         return;
     }

     int delId;
     cout << "Enter Assignment ID to remove: ";
     cin >> delId;

     db->removeAssignment(delId);
}

void Teacher::viewSubmissions(int courseId) {
    if(!db) { cout << "Error: DB not connected.\n"; return; }

    cout << "Select Assignment ID from Course " << courseId << " to view submissions:\n";
    Assignment** allAssigns = db->getAssignments();
    int aCount = db->getAssignmentCount();
    for(int i=0; i<aCount; ++i) {
        if(allAssigns[i] && allAssigns[i]->getCourseID() == courseId) {
            cout << " - ID: " << allAssigns[i]->getAssignID() << " Title: " << allAssigns[i]->getTitle() << "\n";
        }
    }

    int targetAId;
    cout << "Enter Assignment ID: ";
    cin >> targetAId;

    Submission** subs = db->getSubmissions();
    int sCount = db->getSubmissionCount();
    bool sFound = false;

    cout << "\n--- Submissions ---\n";
    for(int i=0; i<sCount; ++i) {
        if(subs[i] && subs[i]->getAssignID() == targetAId) {
            cout << "Submission ID: " << i << " (System Index) \n"; // Assuming ID logic or just index
            cout << " - Student ID: " << subs[i]->getStudentID() << "\n";
            cout << " - Content: " << subs[i]->getContent() << "\n";
            cout << " - Marks: " << subs[i]->getMarks() << "\n";
            sFound = true;
        }
    }
    if(!sFound) cout << "No submissions found for this assignment.\n";
}

void Teacher::gradeSubmission(int courseId, int studentId) {
    if(!db) { cout << "Error: DB not connected.\n"; return; }

    cout << "To grade a student (" << studentId << ") in Course " << courseId << ", please provide the Assignment ID.\n";
    
    Assignment** allAssigns = db->getAssignments();
    int aCount = db->getAssignmentCount();
    cout << "Assignments in Course " << courseId << ":\n";
    for(int i=0; i<aCount; ++i) {
        if(allAssigns[i] && allAssigns[i]->getCourseID() == courseId) {
            cout << " - ID: " << allAssigns[i]->getAssignID() << " " << allAssigns[i]->getTitle() << "\n"; 
        }
    }

    int assignId;
    cout << "Enter Assignment ID: ";
    cin >> assignId;

    Submission** subs = db->getSubmissions();
    int sCount = db->getSubmissionCount();
    Submission* targetSub = nullptr;

    for(int i=0; i<sCount; ++i) {
        if(subs[i] && subs[i]->getAssignID() == assignId && subs[i]->getStudentID() == studentId) {
            targetSub = subs[i];
            break;
        }
    }

    if(targetSub) {
        cout << "Current Marks: " << targetSub->getMarks() << "\n";
        int newMarks;
        cout << "Enter New Marks: ";
        cin >> newMarks;
        targetSub->setMarks(newMarks);
        cout << "Marks updated.\n";
    } else {
        cout << "Submission not found for Student " << studentId << " on Assignment " << assignId << ".\n";
    }
}



void Teacher::saveTeacherData() {
    if(db) {
        db->saveTeachers(); 
        cout << "Teacher data saved to teachers.txt\n";
    } else {
        cout << "Error: Database link missing.\n";
    }
}

void Teacher::loadTeacherData() {
    if(db) {
        db->loadTeachers(); 
        cout << "Teacher data loaded from teachers.txt\n";
    } else {
        cout << "Error: Database link missing.\n";
    }
}
