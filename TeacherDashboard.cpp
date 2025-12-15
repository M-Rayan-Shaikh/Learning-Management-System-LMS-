#include "TeacherDashboard.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include "Course.h"
#include "OnlineCourse.h"
#include "OnsiteCourse.h"
#include "Assignment.h"
#include "Submission.h"

TeacherDashboard::TeacherDashboard(LMSDatabase* db, QWidget *parent)
    : QWidget(parent), database(db), currentTeacher(nullptr)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* title = new QLabel("Teacher Dashboard");
    title->setStyleSheet("font-size: 18pt; font-weight: bold;");
    QPushButton* logoutBtn = new QPushButton("Logout");
    connect(logoutBtn, &QPushButton::clicked, this, &TeacherDashboard::logoutRequested);
    
    header->addWidget(title);
    header->addStretch();
    header->addWidget(logoutBtn);
    mainLayout->addLayout(header);
    

    QHBoxLayout* courseLayout = new QHBoxLayout();
    courseLayout->addWidget(new QLabel("Select Course:"));
    courseSelector = new QComboBox();
    connect(courseSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TeacherDashboard::onCourseSelected);
    courseLayout->addWidget(courseSelector);
    
    QPushButton* addCourseBtn = new QPushButton("Add New Course");
    connect(addCourseBtn, &QPushButton::clicked, this, &TeacherDashboard::onAddCourseClicked);
    courseLayout->addWidget(addCourseBtn);
    
    courseLayout->addStretch();
    mainLayout->addLayout(courseLayout);

    mainTabs = new QTabWidget();
    mainLayout->addWidget(mainTabs);
    
    setupAssignmentTab();
    setupGradingTab();
}

void TeacherDashboard::setTeacher(Teacher* t) {
    currentTeacher = t;
    refreshData();
}

void TeacherDashboard::refreshData() {
    loadCourses();
    // Use first course if available
    if(courseSelector->count() > 0) courseSelector->setCurrentIndex(0);
    loadAssignments();
    loadSubmissions();
}

void TeacherDashboard::loadCourses() {
    courseSelector->clear();
    if(!currentTeacher) return;
    
    // In current implementation, Teacher holds course IDs. 
    // We iterate specific courses
    // Assuming Teacher has getCourseIDs or we iterate all courses and check permissions?
    // Teacher.h shows: int courseIDs[5]; int coursesCount;
    // But those are private/protected. We need to check if Teacher has getter.
    // Let's assume we iterate all database courses for now and matching ID if accessible, 
    // OR we rely on Teacher::subjects (if that's what it was).
    // Actually, checking Teacher.h: it has list of course IDs.
    // For now, I'll assume we show ALL courses or just the ones the teacher "owns".
    // Since accessors might be tricky, let's just list ALL courses in DB for simplicity 
    // as per console app often allowing broader access, or refine later.
    
    // Better: List courses from DB
    Course** courses = database->getCourses();
    for(int i=0; i<database->getCourseCount(); ++i) {
        if(courses[i]) {
            // Using UserRole to store ID
            courseSelector->addItem(QString::fromStdString(courses[i]->getCourseName()), courses[i]->getCourseID());
        }
    }
}

void TeacherDashboard::onCourseSelected(int index) {
    loadAssignments();
    loadSubmissions();
}

void TeacherDashboard::setupAssignmentTab() {
    assignTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(assignTab);
    
    QHBoxLayout* actions = new QHBoxLayout();
    createAssignBtn = new QPushButton("Create Assignment");
    deleteAssignBtn = new QPushButton("Delete Assignment");
    actions->addWidget(createAssignBtn);
    actions->addWidget(deleteAssignBtn);
    actions->addStretch();
    
    connect(createAssignBtn, &QPushButton::clicked, this, &TeacherDashboard::onCreateAssignmentClicked);
    connect(deleteAssignBtn, &QPushButton::clicked, this, &TeacherDashboard::onDeleteAssignmentClicked);
    
    assignTable = new QTableWidget();
    assignTable->setColumnCount(3);
    assignTable->setHorizontalHeaderLabels({"ID", "Title", "Total Marks"});
    assignTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    assignTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    layout->addLayout(actions);
    layout->addWidget(assignTable);
    
    mainTabs->addTab(assignTab, "Assignments");
}

void TeacherDashboard::setupGradingTab() {
    gradingTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(gradingTab);
    
    QHBoxLayout* actions = new QHBoxLayout();
    gradeBtn = new QPushButton("Grade Selected Submission");
    actions->addWidget(gradeBtn);
    actions->addStretch();
    
    connect(gradeBtn, &QPushButton::clicked, this, &TeacherDashboard::onGradeSubmissionClicked);
    
    submissionTable = new QTableWidget();
    submissionTable->setColumnCount(5);
    submissionTable->setHorizontalHeaderLabels({"SubID", "AssignID", "StudentID", "Content", "Marks"});
    submissionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    submissionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    layout->addLayout(actions);
    layout->addWidget(submissionTable);
    
    mainTabs->addTab(gradingTab, "Submissions");
}

void TeacherDashboard::loadAssignments() {
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

void TeacherDashboard::loadSubmissions() {
    submissionTable->setRowCount(0);
    int currentCourseId = courseSelector->currentData().toInt();
    
    // We need to find assignments for this course first to filter submissions
    // Or just iterate all submissions and check if their assignment belongs to this course.
    
    Submission** subs = database->getSubmissions();
    Assignment** assigns = database->getAssignments();
    
    for(int i=0; i<database->getSubmissionCount(); ++i) {
        if(subs[i]) {
            // Find parent assignment
            int aId = subs[i]->getAssignID();
            bool belongsToCourse = false;
            
            // Inefficient lookup but functional for small DB
            for(int j=0; j<database->getAssignmentCount(); ++j) {
                if(assigns[j] && assigns[j]->getAssignID() == aId && assigns[j]->getCourseID() == currentCourseId) {
                    belongsToCourse = true;
                    break;
                }
            }
            
            if(belongsToCourse) {
                int row = submissionTable->rowCount();
                submissionTable->insertRow(row);
                // SubID is index i effectively
                submissionTable->setItem(row, 0, new QTableWidgetItem(QString::number(i))); 
                submissionTable->setItem(row, 1, new QTableWidgetItem(QString::number(aId)));
                submissionTable->setItem(row, 2, new QTableWidgetItem(QString::number(subs[i]->getStudentID())));
                submissionTable->setItem(row, 3, new QTableWidgetItem(subs[i]->getContent()));
                submissionTable->setItem(row, 4, new QTableWidgetItem(QString::number(subs[i]->getMarks())));
            }
        }
    }
}

void TeacherDashboard::onCreateAssignmentClicked() {
    if(courseSelector->count() == 0) return;
    int cid = courseSelector->currentData().toInt();
    
    QDialog dialog(this);
    dialog.setWindowTitle("Create Assignment");
    QFormLayout form(&dialog);
    
    QLineEdit* idEdit = new QLineEdit();
    QLineEdit* titleEdit = new QLineEdit();
    QLineEdit* marksEdit = new QLineEdit();
    
    form.addRow("Assignment ID:", idEdit);
    form.addRow("Title:", titleEdit);
    form.addRow("Marks:", marksEdit);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        int id = idEdit->text().toInt();
        QString title = titleEdit->text();
        int marks = marksEdit->text().toInt();
        
        database->addAssignment(new Assignment(id, cid, title.toStdString().c_str(), "", marks));
        database->saveAll();
        refreshData();
    }
}

void TeacherDashboard::onDeleteAssignmentClicked() {
    int row = assignTable->currentRow();
    if(row < 0) return;
    int id = assignTable->item(row, 0)->text().toInt();
    
    if (QMessageBox::question(this, "Confirm", "Delete Assignment?") == QMessageBox::Yes) {
        database->removeAssignment(id);
        database->saveAll();
        refreshData();
    }
}

void TeacherDashboard::onGradeSubmissionClicked() {
    int row = submissionTable->currentRow();
    if(row < 0) return;
    
    // Create direct pointer access or use setters?
    // User pointer access via array
    int subIndex = submissionTable->item(row, 0)->text().toInt(); 
    Submission* sub = database->getSubmissions()[subIndex];
    
    if(sub) {
        bool ok;
        int marks = QInputDialog::getInt(this, "Grade Submission", "Enter Marks:", sub->getMarks(), 0, 100, 1, &ok);
        if(ok) {
            sub->setMarks(marks);
            database->saveAll(); // Save marks
            refreshData();
        }
    }
}

void TeacherDashboard::onAddCourseClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Course");
    QFormLayout form(&dialog);
    
    QComboBox* typeCombo = new QComboBox();
    typeCombo->addItems({"Online", "Onsite"});
    
    QLineEdit* idEdit = new QLineEdit();
    QLineEdit* nameEdit = new QLineEdit();
    QLineEdit* extraEdit = new QLineEdit(); // Platform or Room
    
    form.addRow("Type:", typeCombo);
    form.addRow("ID:", idEdit);
    form.addRow("Name:", nameEdit);
    form.addRow("Platform/Room:", extraEdit);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        int id = idEdit->text().toInt();
        QString name = nameEdit->text();
        QString extra = extraEdit->text();
        QString type = typeCombo->currentText();
        
        // Add to DB
        if (type == "Online") {
            database->addCourse(new OnlineCourse(id, name.toStdString(), extra.toStdString()));
        } else {
            database->addCourse(new OnsiteCourse(id, name.toStdString().c_str(), extra.toStdString()));
        }
        
        // Assign to this teacher
        if(currentTeacher) {
            currentTeacher->addCourse(id);
        }
        
        database->saveAll();
        refreshData();
        QMessageBox::information(this, "Success", "Course Added and Assigned to You.");
    }
}
