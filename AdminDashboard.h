#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include "LMSDatabase.h"

class AdminDashboard : public QWidget {
    Q_OBJECT

public:
    explicit AdminDashboard(LMSDatabase* db, QWidget *parent = nullptr);
    void refreshData();

signals:
    void logoutRequested();

private slots:
    void onCreateUserClicked();
    void onDeleteUserClicked();
    void onEditUserClicked(); // New slot
    void onCreateCourseClicked();
    void onDeleteCourseClicked();
    void onAssignTeacherClicked(); // New slot

private:
    LMSDatabase* database;
    QTabWidget* tabs;
    
    // User Management
    QWidget* userTab;
    QTableWidget* userTable;
    QPushButton* createUserBtn;
    QPushButton* deleteUserBtn;
    QPushButton* editUserBtn; // New button
    
    // Course Management
    QWidget* courseTab;
    QTableWidget* courseTable;
    QPushButton* createCourseBtn;
    QPushButton* deleteCourseBtn;
    QPushButton* assignTeacherBtn; // New button

    void setupUserTab();
    void setupCourseTab();
    void loadUsers();
    void loadCourses();
};

#endif // ADMINDASHBOARD_H
