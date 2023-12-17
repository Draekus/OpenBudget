#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include "linkbutton.h"
#include "registerdialog.h"
//#include "loginsuccessdialog.h"
#include "passwordresetdialog.h"
#include "user.h"

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent); // Constructor.

signals:
    void userLoggedInSignal(User *user); // Signal to indicate that the user has logged in.

private slots:
    void loginButtonClicked();          // When login button is clicked.
    void registerButtonClicked();       // When register button is clicked.
    void forgotPasswordButtonClicked(); // When forgot password button is clicked.
    void passwordRecoveryClosed();      // When password recovery dialog is closed.
    void enableLoginButton();    // Enable login button when username and password are entered.
    void userRegistered();       // When user has been registered.
    void registerDialogClosed(); // When register dialog is closed.

private:
    QLineEdit *usernameLineEdit = nullptr;                 // Username line edit.
    QLineEdit *passwordLineEdit = nullptr;                 // Password line edit.
    QPushButton *loginButton = nullptr;                    // Login button.
    QPushButton *registerButton = nullptr;                 // Register button.
    LinkButton *forgotPasswordButton = nullptr;            // Forgot password button.
    RegisterDialog *registerDialog = nullptr;              // Register dialog.
    PasswordResetDialog *passwordRecoveryDialog = nullptr; // Password recovery dialog.
};

#endif // LOGINDIALOG_H
