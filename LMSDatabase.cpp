#include "LMSDatabase.h"
#include "student.h"
#include "teacher.h"
#include "Course.h"
#include "OnlineCourse.h"
#include "OnsiteCourse.h"
#include "Admin.h"
#include <iostream>
#include <limits>
using namespace std;
LMSDatabase::LMSDatabase() 
{
    studentCount = 0;
    teacherCount = 0;
    adminCount = 0;
    totalCourses = 0;
    for (int i = 0; i < 100; i++) { students[i] = nullptr; courses[i] = nullptr; }
    for (int i = 0; i < 20; i++) teachers[i] = nullptr;
    for (int i = 0; i < 5; i++) admins[i] = nullptr;
}
void LMSDatabase::addStudent(Student* s) 
{
    if (studentCount < 100) {
        students[studentCount] = s;
        studentCount++;
      
    }
    else {
        cout << "Error: Student database is full (Max 100)." << endl;
    }
}

void LMSDatabase::addTeacher(Teacher* t) 
{
    if (teacherCount < 20) {
        teachers[teacherCount] = t;
        teacherCount++;
    }
    else {
        cout << "Error: Teacher database is full (Max 20)." << endl;
    }
}
void LMSDatabase::addCourse(Course* c) 
{
    if (totalCourses < 100) {
        courses[totalCourses] = c;
        totalCourses++;
    }
    else {
        cout << "Error: Course database is full (Max 100)." << endl;
    }
}

void LMSDatabase::addAdmin(Admin* a) {
    if (adminCount < 5) {
        admins[adminCount] = a;
        adminCount++;
    }
    else {
        cout << "Error: Admin database is full (Max 5)." << endl;
    }
}
void LMSDatabase::addAssignment(Assignment* a) {
    if (assignmentCount < 200) {
        assignments[assignmentCount++] = a;
    } else {
        cout << "Assignment DB full.\n";
    }
}

void LMSDatabase::addSubmission(Submission* s) {
    if (submissionCount < 500) {
        submissions[submissionCount++] = s;
    } else {
        cout << "Submission DB full.\n";
    }
}

void LMSDatabase::removeAssignment(int assignId) {
    for (int i = 0; i < assignmentCount; ++i) {
        if (assignments[i] && assignments[i]->getAssignID() == assignId) {
            delete assignments[i];
            assignments[i] = nullptr; 
            cout << "Assignment " << assignId << " removed from DB.\n";
            return;
        }
    }
    cout << "Assignment not found.\n";
    cout << "Assignment not found.\n";
}

void LMSDatabase::removeStudent(int id) {
    for(int i=0; i<studentCount; ++i) {
        if(students[i] && students[i]->getStudentId() == id) {
            delete students[i];
            for(int j=i; j<studentCount-1; ++j) {
                students[j] = students[j+1];
            }
            students[studentCount-1] = nullptr;
            studentCount--;
            cout << "Student " << id << " removed.\n";
            return;
        }
    }
    cout << "Student " << id << " not found.\n";
}

void LMSDatabase::removeTeacher(int id) {
    for(int i=0; i<teacherCount; ++i) {
        if(teachers[i] && teachers[i]->getTeacherId() == id) {
            delete teachers[i];
            // Shift left
            for(int j=i; j<teacherCount-1; ++j) {
                teachers[j] = teachers[j+1];
            }
            teachers[teacherCount-1] = nullptr;
            teacherCount--;
            cout << "Teacher " << id << " removed.\n";
            return;
        }
    }
    cout << "Teacher " << id << " not found.\n";
}

void LMSDatabase::removeCourse(int id) {
    for(int i=0; i<totalCourses; ++i) {
        if(courses[i] && courses[i]->getCourseID() == id) {
            delete courses[i];
            // Shift left
            for(int j=i; j<totalCourses-1; ++j) {
                courses[j] = courses[j+1];
            }
            courses[totalCourses-1] = nullptr;
            totalCourses--;
            cout << "Course " << id << " removed.\n";
            return;
        }
    }
    cout << "Course " << id << " not found.\n";
}

void ignoreLine(ifstream& in) 
{
    in.ignore(numeric_limits<streamsize>::max(), '\n');
}

string getFilePath(const string& filename) 
{
#ifdef DATA_DIR
    return string(DATA_DIR) + "/" + filename;
#else
    return filename;
#endif
}

void LMSDatabase::saveAll() {
    saveStudents();
    saveTeachers();
    saveAdmins();
    saveCourses();
    saveAssignments();
    saveSubmissions();
}

void LMSDatabase::saveStudents() 
{
    ofstream sFile(getFilePath("students.txt"));
    if (sFile) {
        sFile << studentCount << endl;
        for(int i=0; i<studentCount; ++i) {
            if(students[i]) sFile << students[i]->serialize() << endl;
        }
        sFile.close();
    } else cout << "Error saving students.txt to " << getFilePath("students.txt") << endl;
}

void LMSDatabase::saveTeachers() {
    ofstream tFile(getFilePath("teachers.txt"));
    if (tFile) {
        tFile << teacherCount << endl;
        for(int i=0; i<teacherCount; ++i) {
            if(teachers[i]) tFile << teachers[i]->serialize() << endl;
        }
        tFile.close();
    } else cout << "Error saving teachers.txt" << endl;
}

void LMSDatabase::saveAdmins() {
    ofstream aFile(getFilePath("admins.txt"));
    if (aFile) {
        aFile << adminCount << endl;
        for(int i=0; i<adminCount; ++i) {
            if(admins[i]) {
                aFile << admins[i]->getID() << " " << admins[i]->getName() << " " 
                      << admins[i]->getUsername() << " " << admins[i]->getPassword() << endl;
            }
        }
        aFile.close();
    } else cout << "Error saving admins.txt" << endl;
}

void LMSDatabase::saveCourses() {
    ofstream cFile(getFilePath("courses.txt"));
    if (cFile) {
        cFile << totalCourses << endl;
        for(int i=0; i<totalCourses; ++i) {
            if(courses[i]) {
                cFile << courses[i]->getCourseType() << " " 
                      << courses[i]->getCourseID() << " " 
                      << "\"" << courses[i]->getCourseName() << "\" " 
                      << courses[i]->getExtraDetails() << endl;
            }
        }
    } else cout << "Error saving courses.txt" << endl;
}

void LMSDatabase::saveAssignments() {
    ofstream aFile(getFilePath("assignments.txt"));
    if (aFile) {
        aFile << assignmentCount << endl;
        for(int i=0; i<assignmentCount; ++i) {
            if(assignments[i]) {
                // Format: ID CourseID Title Marks (Title can have spaces, handle safely)
                string safeTitle = assignments[i]->getTitle();
                for(char &c : safeTitle) if(c==' ') c='_';
                
                aFile << assignments[i]->getAssignID() << " " 
                      << assignments[i]->getCourseID() << " " 
                      << safeTitle << " " 
                      << assignments[i]->getTotalMarks() << endl;
            }
        }
        aFile.close();
    } else cout << "Error saving assignments.txt" << endl;
}

void LMSDatabase::saveSubmissions() {
    ofstream sFile(getFilePath("submissions.txt"));
    if (sFile) {
        sFile << submissionCount << endl;
        for(int i=0; i<submissionCount; ++i) {
            if(submissions[i]) {
                // Format: ID AssignID StudentID Content Marks (Content spaces -> _)
                string safeContent = submissions[i]->getContent();
                for(char &c : safeContent) if(c==' ') c='_';

                sFile << i << " " // Use index as ID if getter missing, or if ID stored iterate
                      << submissions[i]->getAssignID() << " "
                      << submissions[i]->getStudentID() << " "
                      << safeContent << " "
                      << submissions[i]->getMarks() << endl;
            }
        }
        sFile.close();
    } else cout << "Error saving submissions.txt" << endl;
}

void LMSDatabase::loadAll() {
    for (int i = 0; i < studentCount; ++i) delete students[i];
    for (int i = 0; i < teacherCount; ++i) delete teachers[i];
    for (int i = 0; i < adminCount; ++i) delete admins[i];
    for (int i = 0; i < totalCourses; ++i) delete courses[i];
    
    studentCount=0; teacherCount=0; adminCount=0; totalCourses=0;
    assignmentCount=0; submissionCount=0;

    loadStudents();
    loadTeachers();
    loadAdmins();
    loadCourses();
    loadAssignments();
    loadSubmissions();

}

void LMSDatabase::loadStudents() {
    ifstream sFile(getFilePath("students.txt"));
    if (sFile) {
        int count; sFile >> count;
        for(int i=0; i<count; ++i) {
            int id = 0, cCountInner = 0;
            string name, user, pass;
            sFile >> id >> name >> user >> pass >> cCountInner;
            
            if(sFile.fail()) break;

            // Restore spaces in name
            for(char &c : name) {
                if(c == '_') c = ' ';
            }

            Student* s = new Student(id, name.c_str(), user.c_str(), pass.c_str());
            for(int j=0; j<cCountInner; ++j) {
                int cid; sFile >> cid;
                s->addEnrolledCourseDirect(cid);
            }
            addStudent(s);
        }
        sFile.close();
    }
}

void LMSDatabase::loadTeachers() {
    ifstream tFile(getFilePath("teachers.txt"));
    if (tFile) {
        int count; tFile >> count;
        for(int i=0; i<count; ++i) {
            int id = 0, cCountInner = 0;
            string name, user, pass;
            tFile >> id >> name >> user >> pass >> cCountInner;
            
            if(tFile.fail()) break;
            
            // Restore spaces in name
            for(char &c : name) {
                if(c == '_') c = ' ';
            }
            
            Teacher* t = new Teacher(id, name.c_str(), user.c_str(), pass.c_str());
            for(int j=0; j<cCountInner; ++j) {
                int cid; tFile >> cid;
                t->addCourseDirect(cid);
            }
            addTeacher(t);
        }
        tFile.close();
    }
}

void LMSDatabase::loadAdmins() {
    ifstream aFile(getFilePath("admins.txt"));
    if (aFile) {
        int count; aFile >> count;
        for(int i=0; i<count; ++i) {
            int id;
            string name, user, pass;
            aFile >> id >> name >> user >> pass;
            Admin* a = new Admin(id, name.c_str(), user.c_str(), pass.c_str());
            addAdmin(a);
        }
        aFile.close();
    }
}

void LMSDatabase::loadCourses() {
    ifstream cFile(getFilePath("courses.txt"));
    if (cFile) {
        int count; cFile >> count;
        for(int i=0; i<count; ++i) {
            string type, nameStr, extra;
            int id;
            cFile >> type >> id >> nameStr >> extra;
            
            if(nameStr.size() >= 2 && nameStr.front() == '"' && nameStr.back() == '"') {
                nameStr = nameStr.substr(1, nameStr.size() - 2);
            }

            if (type == "Online") {
                addCourse(new OnlineCourse(id, nameStr, extra));
            } else if (type == "Onsite") {
                addCourse(new OnsiteCourse(id, nameStr.c_str(), extra));
            } else {
                addCourse(new Course(id, nameStr.c_str()));
            }
        }
        cFile.close();
    }
}

void LMSDatabase::loadAssignments() {
    ifstream aFile(getFilePath("assignments.txt"));
    if (aFile) {
        int count; aFile >> count;
        for(int i=0; i<count; ++i) {
            int id=0, cid=0, marks=0;
            string title;
            aFile >> id >> cid >> title >> marks;
            
            if(aFile.fail()) break;

            for(char &c : title) if(c=='_') c=' ';
            
            addAssignment(new Assignment(id, cid, title.c_str(), "", marks));
        }
        aFile.close();
    }
}

void LMSDatabase::loadSubmissions() 
{
    ifstream sFile(getFilePath("submissions.txt"));
    if (sFile) {
        int count; sFile >> count;
        for(int i=0; i<count; ++i) {
            int id=0, aid=0, sid=0, marks=0;
            string content;
            sFile >> id >> aid >> sid >> content >> marks;
            
            if(sFile.fail()) break;

            for(char &c : content) if(c=='_') c=' ';
            
            Submission* s = new Submission(id, aid, sid, content.c_str());
            s->setMarks(marks);
            addSubmission(s);
        }
        sFile.close();
    }
}