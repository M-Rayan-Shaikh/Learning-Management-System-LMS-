#ifndef TEACHERDASHBOARD_H
#define TEACHERDASHBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include <QComboBox>
#include "LMSDatabase.h"
#include "Teacher.h"

class TeacherDashboard : public QWidget {
    Q_OBJECT

public:
    explicit TeacherDashboard(LMSDatabase* db, QWidget *parent = nullptr);
    void setTeacher(Teacher* t);
    void refreshData();

signals:
    void logoutRequested();

private slots:
    void onCreateAssignmentClicked();
    void onDeleteAssignmentClicked();
    void onAddCourseClicked();
    void onGradeSubmissionClicked();
    void onCourseSelected(int index);

private:
    LMSDatabase* database;
    Teacher* currentTeacher;
    
    QComboBox* courseSelector;
    QTabWidget* mainTabs;
    
    QWidget* assignTab;
    QTableWidget* assignTable;
    QPushButton* createAssignBtn;
    QPushButton* deleteAssignBtn;
    
    QWidget* gradingTab;
    QTableWidget* submissionTable;
    QPushButton* gradeBtn;

    void setupAssignmentTab();
    void setupGradingTab();
    
    void loadCourses();
    void loadAssignments();
    void loadSubmissions(); 
};

#endif // TEACHERDASHBOARD_H
