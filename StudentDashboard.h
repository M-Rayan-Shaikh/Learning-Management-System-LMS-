#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include <QComboBox>
#include "LMSDatabase.h"
#include "Student.h"

class StudentDashboard : public QWidget {
    Q_OBJECT

public:
    explicit StudentDashboard(LMSDatabase* db, QWidget *parent = nullptr);
    void setStudent(Student* s);
    void refreshData();

signals:
    void logoutRequested();

private slots:
    void onSubmitAssignmentClicked();
    void onEnrollClicked();
    void onDropClicked();
    void onCourseSelected(int index);

private:
    LMSDatabase* database;
    Student* currentStudent;
    
    QComboBox* courseSelector;
    QTabWidget* mainTabs;
    
    QWidget* assignTab;
    QTableWidget* assignTable;
    QPushButton* submitBtn;
    
    QWidget* gradeTab;
    QTableWidget* gradeTable;

    void setupAssignmentTab();
    void setupGradeTab();
    
    void loadCourses();
    void loadAssignments();
    void loadGrades();
};

#endif // STUDENTDASHBOARD_H
