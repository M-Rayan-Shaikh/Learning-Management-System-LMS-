#include "StudentDashboard.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include "Course.h"
#include "Assignment.h"
#include "Submission.h"

StudentDashboard::StudentDashboard(LMSDatabase* db, QWidget *parent)
    : QWidget(parent), database(db), currentStudent(nullptr)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* title = new QLabel("Student Dashboard");
    title->setStyleSheet("font-size: 18pt; font-weight: bold;");
    QPushButton* logoutBtn = new QPushButton("Logout");
    connect(logoutBtn, &QPushButton::clicked, this, &StudentDashboard::logoutRequested);
    
    header->addWidget(title);
    header->addStretch();
    header->addWidget(logoutBtn);
    mainLayout->addLayout(header);
    
    QHBoxLayout* courseLayout = new QHBoxLayout();
    courseLayout->addWidget(new QLabel("Select Course:"));
    courseSelector = new QComboBox();
    connect(courseSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StudentDashboard::onCourseSelected);
    courseLayout->addWidget(courseSelector);
    
    QPushButton* enrollBtn = new QPushButton("Enroll New Course");
    QPushButton* dropBtn = new QPushButton("Drop Current Course");
    connect(enrollBtn, &QPushButton::clicked, this, &StudentDashboard::onEnrollClicked);
    connect(dropBtn, &QPushButton::clicked, this, &StudentDashboard::onDropClicked);
    
    courseLayout->addWidget(enrollBtn);
    courseLayout->addWidget(dropBtn);
    
    courseLayout->addStretch();
    mainLayout->addLayout(courseLayout);

    // Tabs
    mainTabs = new QTabWidget();
    mainLayout->addWidget(mainTabs);
    
    setupAssignmentTab();
    setupGradeTab();
}

void StudentDashboard::setStudent(Student* s) {
    currentStudent = s;
    refreshData();
}

void StudentDashboard::refreshData() {
    loadCourses();
    if(courseSelector->count() > 0) courseSelector->setCurrentIndex(0);
    loadAssignments();
    loadGrades();
}

void StudentDashboard::loadCourses() {
    courseSelector->clear();
   
    Course** courses = database->getCourses();
    for(int i=0; i<database->getCourseCount(); ++i) {
        if(courses[i]) {
           
            courseSelector->addItem(QString::fromStdString(courses[i]->getCourseName()), courses[i]->getCourseID());
        }
    }
}

void StudentDashboard::onCourseSelected(int index) {
    loadAssignments();
}

void StudentDashboard::setupAssignmentTab() {
    assignTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(assignTab);
    
    QHBoxLayout* actions = new QHBoxLayout();
    submitBtn = new QPushButton("Submit Selected Assignment");
    actions->addWidget(submitBtn);
    actions->addStretch();
    
    connect(submitBtn, &QPushButton::clicked, this, &StudentDashboard::onSubmitAssignmentClicked);
    
    assignTable = new QTableWidget();
    assignTable->setColumnCount(3);
    assignTable->setHorizontalHeaderLabels({"ID", "Title", "Marks"});
    assignTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    assignTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    layout->addLayout(actions);
    layout->addWidget(assignTable);
    
    mainTabs->addTab(assignTab, "Assignments");
}

void StudentDashboard::setupGradeTab() {
    gradeTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(gradeTab);
    
    gradeTable = new QTableWidget();
    gradeTable->setColumnCount(4);
    gradeTable->setHorizontalHeaderLabels({"AssignID", "Assignment", "Marks Obtained", "Total"});
    gradeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    gradeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    layout->addWidget(gradeTable);
    
    mainTabs->addTab(gradeTab, "My Grades");
}

void StudentDashboard::loadAssignments() {
    assignTable->setRowCount(0);
    int currentCourseId = courseSelector->currentData().toInt();
    
    Assignment** assigns = database->getAssignments();
    for(int i=0; i<database->getAssignmentCount(); ++i) {
        if(assigns[i] && assigns[i]->getCourseID() == currentCourseId) {
            int row = assignTable->rowCount();
            assignTable->insertRow(row);
            assignTable->setItem(row, 0, new QTableWidgetItem(QString::number(assigns[i]->getAssignID())));
            assignTable->setItem(row, 1, new QTableWidgetItem(assigns[i]->getTitle()));
            assignTable->setItem(row, 2, new QTableWidgetItem(QString::number(assigns[i]->getTotalMarks())));
        }
    }
}

void StudentDashboard::loadGrades() {
    gradeTable->setRowCount(0);
    if(!currentStudent) return;
    
    int myId = currentStudent->getID();
    Submission** subs = database->getSubmissions();
    Assignment** assigns = database->getAssignments();
    
    for(int i=0; i<database->getSubmissionCount(); ++i) {
        if(subs[i] && subs[i]->getStudentID() == myId) {
            int aId = subs[i]->getAssignID();
            
            QString title = "Unknown";
            int total = 0;
            for(int j=0; j<database->getAssignmentCount(); ++j) {
                if(assigns[j] && assigns[j]->getAssignID() == aId) {
                    title = QString::fromStdString(assigns[j]->getTitle());
                    total = assigns[j]->getTotalMarks();
                    break;
                }
            }
            
            int row = gradeTable->rowCount();
            gradeTable->insertRow(row);
            gradeTable->setItem(row, 0, new QTableWidgetItem(QString::number(aId)));
            gradeTable->setItem(row, 1, new QTableWidgetItem(title));
            gradeTable->setItem(row, 2, new QTableWidgetItem(QString::number(subs[i]->getMarks())));
            gradeTable->setItem(row, 3, new QTableWidgetItem(QString::number(total)));
        }
    }
}

void StudentDashboard::onSubmitAssignmentClicked() {
    int row = assignTable->currentRow();
    if(row < 0) return;
    
    int assignId = assignTable->item(row, 0)->text().toInt();
    
    bool ok;
    QString content = QInputDialog::getText(this, "Submit Assignment", "Enter your solution/content:", QLineEdit::Normal, "", &ok);
    
    if(ok && !content.isEmpty()) {
        if(!currentStudent) return;
        
        int subId = database->getSubmissionCount(); 
        Submission* sub = new Submission(subId, assignId, currentStudent->getID(), content.toStdString().c_str());
        
        database->addSubmission(sub);
        database->saveAll();
        
        QMessageBox::information(this, "Success", "Assignment Submitted!");
        refreshData();
    }
}

void StudentDashboard::onEnrollClicked() {
    bool ok;
    int id = QInputDialog::getInt(this, "Enroll Course", "Enter Course ID to Enroll:", 0, 0, 10000, 1, &ok);
    if(ok) {
        bool exists = false;
        Course** courses = database->getCourses();
        for(int i=0; i<database->getCourseCount(); ++i) {
            if(courses[i] && courses[i]->getCourseID() == id) {
                exists = true; 
                break;
            }
        }
        
        if(exists && currentStudent) {
            currentStudent->enrollCourse(id);
            database->saveAll();
            refreshData();
            QMessageBox::information(this, "Success", "Enrolled in course.");
        } else {
            QMessageBox::warning(this, "Error", "Course ID not found.");
        }
    }
}

void StudentDashboard::onDropClicked() {
    if(courseSelector->count() == 0) return;
    int id = courseSelector->currentData().toInt();
    
    if(QMessageBox::question(this, "Confirm", "Drop this course?") == QMessageBox::Yes) {
        if(currentStudent) {
            currentStudent->dropCourse(id);
            database->saveAll();
            refreshData();
            QMessageBox::information(this, "Success", "Dropped course.");
        }
    }
}
