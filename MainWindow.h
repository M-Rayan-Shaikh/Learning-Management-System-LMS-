#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "LMSDatabase.h"
#include "LoginWidget.h"
#include "AdminDashboard.h"
#include "TeacherDashboard.h"
#include "StudentDashboard.h"
#include "User.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLogin(User* user);
    void handleLogout();

private:
    LMSDatabase database;
    QStackedWidget* stackedWidget;
    LoginWidget* loginWidget;
    AdminDashboard* adminDashboard;
    TeacherDashboard* teacherDashboard;
    StudentDashboard* studentDashboard;
    
   
    QWidget* dashboardWidget; 
    QLabel* welcomeLabel;
    QPushButton* logoutButton;
    
    void setupDashboard();
};

#endif // MAINWINDOW_H
