#include "AdminDashboard.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include "Admin.h"
#include "Teacher.h"
#include "Student.h"
#include "Course.h"
#include "OnlineCourse.h"
#include "OnsiteCourse.h"

AdminDashboard::AdminDashboard(LMSDatabase* db, QWidget *parent)
    : QWidget(parent), database(db)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Header
    QHBoxLayout* header = new QHBoxLayout();
    QLabel* title = new QLabel("Admin Dashboard");
    title->setStyleSheet("font-size: 18pt; font-weight: bold;");
    QPushButton* logoutBtn = new QPushButton("Logout");
    connect(logoutBtn, &QPushButton::clicked, this, &AdminDashboard::logoutRequested);
    
    header->addWidget(title);
    header->addStretch();
    header->addWidget(logoutBtn);
    mainLayout->addLayout(header);
    
    // Tabs
    tabs = new QTabWidget();
    mainLayout->addWidget(tabs);
    
    setupUserTab();
    setupCourseTab();
    
    refreshData();
}

void AdminDashboard::setupUserTab() {
    userTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(userTab);
    
    QHBoxLayout* actions = new QHBoxLayout();
    createUserBtn = new QPushButton("Create User");
    deleteUserBtn = new QPushButton("Delete Selected User");
    actions->addWidget(createUserBtn);
    actions->addWidget(deleteUserBtn);
    editUserBtn = new QPushButton("Edit User"); 
    actions->addWidget(editUserBtn);
    actions->addStretch();
    
    connect(createUserBtn, &QPushButton::clicked, this, &AdminDashboard::onCreateUserClicked);
    connect(deleteUserBtn, &QPushButton::clicked, this, &AdminDashboard::onDeleteUserClicked);
    connect(editUserBtn, &QPushButton::clicked, this, &AdminDashboard::onEditUserClicked);
    
    userTable = new QTableWidget();
    userTable->setColumnCount(4);
    userTable->setHorizontalHeaderLabels({"ID", "Role", "Name", "Username"});
    userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    layout->addLayout(actions);
    layout->addWidget(userTable);
    
    tabs->addTab(userTab, "Manage Users");
}

void AdminDashboard::onEditUserClicked() {
    int row = userTable->currentRow();
    if (row < 0) return;

    int id = userTable->item(row, 0)->text().toInt();
    QString role = userTable->item(row, 1)->text();
    
    User* targetUser = nullptr;
    if (role == "Student") {
        Student** students = database->getStudents();
        for(int i=0; i<database->getStudentCount(); ++i) {
            if(students[i] && students[i]->getID() == id) {
                targetUser = students[i];
                break;
            }
        }
    } else if (role == "Teacher") {
        Teacher** teachers = database->getTeachers();
        for(int i=0; i<database->getTeacherCount(); ++i) {
            if(teachers[i] && teachers[i]->getID() == id) {
                targetUser = teachers[i];
                break;
            }
        }
    }

    if (!targetUser) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Edit User");
    QFormLayout form(&dialog);

    QLineEdit* nameEdit = new QLineEdit(targetUser->getName());
    QLineEdit* userEdit = new QLineEdit(targetUser->getUsername());
    QLineEdit* passEdit = new QLineEdit(targetUser->getPassword());
    
    form.addRow("Name:", nameEdit);
    form.addRow("Username:", userEdit);
    form.addRow("Password:", passEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        targetUser->setName(nameEdit->text().toStdString().c_str());
        targetUser->setUsername(userEdit->text().toStdString().c_str());
        targetUser->setPassword(passEdit->text().toStdString().c_str());
        
        database->saveAll(); // Persist
        refreshData();
        QMessageBox::information(this, "Success", "User Updated.");
    }
}




void AdminDashboard::setupCourseTab() {
    courseTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(courseTab);
    
    QHBoxLayout* actions = new QHBoxLayout();
    createCourseBtn = new QPushButton("Create Course");
    deleteCourseBtn = new QPushButton("Delete Selected Course");
    actions->addWidget(createCourseBtn);
    actions->addWidget(deleteCourseBtn);
    assignTeacherBtn = new QPushButton("Assign Teacher");
    actions->addWidget(assignTeacherBtn);
    actions->addStretch();
    
    connect(createCourseBtn, &QPushButton::clicked, this, &AdminDashboard::onCreateCourseClicked);
    connect(deleteCourseBtn, &QPushButton::clicked, this, &AdminDashboard::onDeleteCourseClicked);
    connect(assignTeacherBtn, &QPushButton::clicked, this, &AdminDashboard::onAssignTeacherClicked);
    
    courseTable = new QTableWidget();
    courseTable->setColumnCount(4);
    courseTable->setHorizontalHeaderLabels({"ID", "Type", "Name", "Extra"});
    courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    courseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    layout->addLayout(actions);
    layout->addWidget(courseTable);
    
    tabs->addTab(courseTab, "Manage Courses");
}

void AdminDashboard::onAssignTeacherClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Assign Teacher to Course");
    QFormLayout form(&dialog);

    QComboBox* teacherCombo = new QComboBox();
    Teacher** teachers = database->getTeachers();
    int tCount = database->getTeacherCount();
    for (int i = 0; i < tCount; ++i) {
        if (teachers[i]) {
            teacherCombo->addItem(QString("%1 (ID: %2)").arg(teachers[i]->getName()).arg(teachers[i]->getID()), teachers[i]->getID());
        }
    }

    QComboBox* courseCombo = new QComboBox();
    Course** courses = database->getCourses();
    int cCount = database->getCourseCount();
    for (int i = 0; i < cCount; ++i) {
        if (courses[i]) {
            courseCombo->addItem(QString("%1 (ID: %2)").arg(QString::fromStdString(courses[i]->getCourseName())).arg(courses[i]->getCourseID()), courses[i]->getCourseID());
        }
    }

    form.addRow("Teacher:", teacherCombo);
    form.addRow("Course:", courseCombo);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (teacherCombo->currentIndex() == -1 || courseCombo->currentIndex() == -1) return;

        int teacherId = teacherCombo->currentData().toInt();
        int courseId = courseCombo->currentData().toInt();

        Teacher** allTeachers = database->getTeachers();
        for (int i = 0; i < tCount; ++i) {
            if (allTeachers[i] && allTeachers[i]->getID() == teacherId) {
                allTeachers[i]->addCourse(courseId);
                break;
            }
        }
        database->saveAll(); // Persist changes
        QMessageBox::information(this, "Success", "Teacher assigned to course.");
    }
}


void AdminDashboard::refreshData() {
    loadUsers();
    loadCourses();
}

void AdminDashboard::loadUsers() {
    userTable->setRowCount(0);
    
    // Load Admins
    Admin** admins = database->getAdmins();
    for(int i=0; i<database->getAdminCount(); ++i) {
        if(admins[i]) {
            int row = userTable->rowCount();
            userTable->insertRow(row);
            userTable->setItem(row, 0, new QTableWidgetItem(QString::number(admins[i]->getID())));
            userTable->setItem(row, 1, new QTableWidgetItem("Admin"));
            userTable->setItem(row, 2, new QTableWidgetItem(admins[i]->getName()));
            userTable->setItem(row, 3, new QTableWidgetItem(admins[i]->getUsername()));
        }
    }
    
    // Load Teachers
    Teacher** teachers = database->getTeachers();
    for(int i=0; i<database->getTeacherCount(); ++i) {
        if(teachers[i]) {
            int row = userTable->rowCount();
            userTable->insertRow(row);
            userTable->setItem(row, 0, new QTableWidgetItem(QString::number(teachers[i]->getID())));
            userTable->setItem(row, 1, new QTableWidgetItem("Teacher"));
            userTable->setItem(row, 2, new QTableWidgetItem(teachers[i]->getName()));
            userTable->setItem(row, 3, new QTableWidgetItem(teachers[i]->getUsername()));
        }
    }
    
    // Load Students
    Student** students = database->getStudents();
    for(int i=0; i<database->getStudentCount(); ++i) {
        if(students[i]) {
            int row = userTable->rowCount();
            userTable->insertRow(row);
            userTable->setItem(row, 0, new QTableWidgetItem(QString::number(students[i]->getID())));
            userTable->setItem(row, 1, new QTableWidgetItem("Student"));
            userTable->setItem(row, 2, new QTableWidgetItem(students[i]->getName()));
            userTable->setItem(row, 3, new QTableWidgetItem(students[i]->getUsername()));
        }
    }
}

void AdminDashboard::loadCourses() {
    courseTable->setRowCount(0);
    Course** courses = database->getCourses();
    for(int i=0; i<database->getCourseCount(); ++i) {
        if(courses[i]) {
            int row = courseTable->rowCount();
            courseTable->insertRow(row);
            courseTable->setItem(row, 0, new QTableWidgetItem(QString::number(courses[i]->getCourseID())));
            courseTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(courses[i]->getCourseType())));
            courseTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(courses[i]->getCourseName())));
            courseTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(courses[i]->getExtraDetails())));
        }
    }
}

// Actions
void AdminDashboard::onCreateUserClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Create User");
    QFormLayout form(&dialog);
    
    QComboBox* typeCombo = new QComboBox();
    typeCombo->addItems({"Student", "Teacher"});
    
    QLineEdit* idEdit = new QLineEdit();
    QLineEdit* nameEdit = new QLineEdit();
    QLineEdit* userEdit = new QLineEdit();
    QLineEdit* passEdit = new QLineEdit();
    passEdit->setEchoMode(QLineEdit::Password);
    
    form.addRow("Type:", typeCombo);
    form.addRow("ID:", idEdit);
    form.addRow("Name:", nameEdit);
    form.addRow("Username:", userEdit);
    form.addRow("Password:", passEdit);
    
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        int id = idEdit->text().toInt();
        QString name = nameEdit->text();
        QString user = userEdit->text();
        QString pass = passEdit->text();
        QString type = typeCombo->currentText();
        
        if (type == "Student") {
             database->addStudent(new Student(id, name.toStdString().c_str(), user.toStdString().c_str(), pass.toStdString().c_str()));
        } else if (type == "Teacher") {
             database->addTeacher(new Teacher(id, name.toStdString().c_str(), user.toStdString().c_str(), pass.toStdString().c_str()));
        }
        database->saveAll();
        refreshData();
        QMessageBox::information(this, "Success", "User Created.");
    }
}

void AdminDashboard::onDeleteUserClicked() {
    int row = userTable->currentRow();
    if (row < 0) return;
    
    int id = userTable->item(row, 0)->text().toInt();
    QString role = userTable->item(row, 1)->text();
    
    if (QMessageBox::question(this, "Confirm", "Delete this user?") == QMessageBox::Yes) {
        if (role == "Student") database->removeStudent(id);
        else if (role == "Teacher") database->removeTeacher(id);
        else QMessageBox::warning(this, "Error", "Cannot delete Admin via GUI yet (safety).");
        
        database->saveAll(); // Persist
        refreshData();
    }
}

void AdminDashboard::onCreateCourseClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Create Course");
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
        
        if (type == "Online") {
            database->addCourse(new OnlineCourse(id, name.toStdString(), extra.toStdString()));
        } else {
            database->addCourse(new OnsiteCourse(id, name.toStdString().c_str(), extra.toStdString()));
        }
        
        database->saveAll();
        refreshData();
    }
}

void AdminDashboard::onDeleteCourseClicked() {
    int row = courseTable->currentRow();
    if (row < 0) return;
    
    int id = courseTable->item(row, 0)->text().toInt();
    
    if (QMessageBox::question(this, "Confirm", "Delete this course?") == QMessageBox::Yes) {
        database->removeCourse(id);
        database->saveAll();
        refreshData();
    }
}
