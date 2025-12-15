#include "LoginWidget.h"
#include <QFormLayout>
#include "Admin.h"
#include "Teacher.h"
#include "Student.h"

#include <QDir>
#include <QDebug>

LoginWidget::LoginWidget(LMSDatabase* db, QWidget *parent) 
    : QWidget(parent), database(db) 
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QFormLayout* form = new QFormLayout();

    usernameInput = new QLineEdit();
    passwordInput = new QLineEdit();
    passwordInput->setEchoMode(QLineEdit::Password);
    
    roleInput = new QComboBox();
    roleInput->addItems({"Admin", "Teacher", "Student"});

    form->addRow("Role:", roleInput);
    form->addRow("Username:", usernameInput);
    form->addRow("Password:", passwordInput);

    loginButton = new QPushButton("Login");
    
    // Debug info in status
    // Debug info removed as per request
    statusLabel = new QLabel("");
    statusLabel->setAlignment(Qt::AlignCenter);

    layout->addLayout(form);
    layout->addWidget(loginButton);
    layout->addWidget(statusLabel);

    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::onLoginClicked);
}

void LoginWidget::onLoginClicked() {
    QString user = usernameInput->text();
    QString pass = passwordInput->text();
    QString role = roleInput->currentText();
    
    User* loggedInUser = nullptr;

    if (role == "Admin") {
        Admin** admins = database->getAdmins();
        for(int i=0; i<database->getAdminCount(); ++i) {
            if(admins[i] && admins[i]->checkCredentials(user.toStdString().c_str(), pass.toStdString().c_str())) {
                loggedInUser = admins[i];
                break;
            }
        }
    } else if (role == "Teacher") {
        Teacher** teachers = database->getTeachers();
        for(int i=0; i<database->getTeacherCount(); ++i) {
            if(teachers[i] && teachers[i]->checkCredentials(user.toStdString().c_str(), pass.toStdString().c_str())) {
                loggedInUser = teachers[i];
                break;
            }
        }
    } else if (role == "Student") {
        Student** students = database->getStudents();
        for(int i=0; i<database->getStudentCount(); ++i) {
            if(students[i] && students[i]->checkCredentials(user.toStdString().c_str(), pass.toStdString().c_str())) {
                loggedInUser = students[i];
                break;
            }
        }
    }

    if(loggedInUser) {
        statusLabel->setText("Login Successful!");
        emit loginSuccessful(loggedInUser);
    } else {
        statusLabel->setText("Invalid Credentials");
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}
