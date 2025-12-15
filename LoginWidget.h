#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include "LMSDatabase.h"

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(LMSDatabase* db, QWidget *parent = nullptr);

signals:
    void loginSuccessful(User* user);

private slots:
    void onLoginClicked();

private:
    LMSDatabase* database;
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QComboBox* roleInput;
    QPushButton* loginButton;
    QLabel* statusLabel;
};

#endif // LOGINWIDGET_H
