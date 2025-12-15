#include "MainWindow.h"
#include <QWidget>
#include "Admin.h"
#include "Teacher.h"
#include "Student.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    
    database.loadAll(); 
    
    
    QWidget* central = new QWidget;
    setCentralWidget(central);
    QVBoxLayout* layout = new QVBoxLayout(central);
    
    stackedWidget = new QStackedWidget();
    layout->addWidget(stackedWidget);
    
    loginWidget = new LoginWidget(&database);
    connect(loginWidget, &LoginWidget::loginSuccessful, this, &MainWindow::handleLogin);
    stackedWidget->addWidget(loginWidget);
 
    setupDashboard();
    
    dashboardWidget = new QWidget();
    QVBoxLayout* dLayout = new QVBoxLayout(dashboardWidget);
    welcomeLabel = new QLabel("Welcome!");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    logoutButton = new QPushButton("Logout");
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);
    dLayout->addWidget(welcomeLabel);
    dLayout->addWidget(logoutButton);
    stackedWidget->addWidget(dashboardWidget);
    
    stackedWidget->setCurrentIndex(0);
    
    resize(800, 600);
    setWindowTitle("LMS Application");
}

MainWindow::~MainWindow() {
    database.saveAll(); 
}

void MainWindow::setupDashboard() {
    adminDashboard = new AdminDashboard(&database);
    connect(adminDashboard, &AdminDashboard::logoutRequested, this, &MainWindow::handleLogout);
    
    teacherDashboard = new TeacherDashboard(&database);
    connect(teacherDashboard, &TeacherDashboard::logoutRequested, this, &MainWindow::handleLogout);

    studentDashboard = new StudentDashboard(&database);
    connect(studentDashboard, &StudentDashboard::logoutRequested, this, &MainWindow::handleLogout);
    
    stackedWidget->addWidget(adminDashboard); 
    stackedWidget->addWidget(teacherDashboard);
    stackedWidget->addWidget(studentDashboard); 
}

void MainWindow::handleLogin(User* user) {
    QString role;
    
    if (dynamic_cast<Admin*>(user)) {
        adminDashboard->refreshData();
        stackedWidget->setCurrentWidget(adminDashboard);
    } 
    else if (Teacher* t = dynamic_cast<Teacher*>(user)) {
        teacherDashboard->setTeacher(t);
        stackedWidget->setCurrentWidget(teacherDashboard);
    }
    else if (Student* s = dynamic_cast<Student*>(user)) {
        studentDashboard->setStudent(s);
        stackedWidget->setCurrentWidget(studentDashboard);
    }
    else {
        if(dashboardWidget) {
             welcomeLabel->setText(QString("Welcome, %1!").arg(user->getName()));
             stackedWidget->setCurrentWidget(dashboardWidget);
        }
    }
}

void MainWindow::handleLogout() {
    stackedWidget->setCurrentIndex(0);
    welcomeLabel->setText("Welcome!");
}
