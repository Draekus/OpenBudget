#include "logindialog.h"
#include <QFormLayout>
#include <QSqlError>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QtWidgets>
#include "database.h"
#include "linkbutton.h"
#include "user.h"
#include "userlogin.h"

/**
 * @brief Dialog that allows users to login to their account.
 * 
 * @param parent QWidget pointer to parent widget.
 */
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog{parent}
    , usernameLineEdit{new QLineEdit(this)}
    , passwordLineEdit{new QLineEdit(this)}
    , loginButton{new QPushButton("Login", this)}
    , registerButton{new QPushButton("Register", this)}
    , forgotPasswordButton{new LinkButton("Forgot Password", this)}
{
    // Set window title.
    setWindowTitle("Login");

    // Set password line edit to hide characters.
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    /* Connect the signals and slots */
    // When login button is clicked, run loginButtonClicked function.
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::loginButtonClicked);
    // When register button is clicked, run registerButtonClicked function.
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::registerButtonClicked);
    // When forgot password button is clicked, run forgotPasswordButtonClicked function.
    connect(forgotPasswordButton,
            &QPushButton::clicked,
            this,
            &LoginDialog::forgotPasswordButtonClicked);
    // Only enable the login button when both username and password are filled in.
    connect(usernameLineEdit, &QLineEdit::textChanged, this, &LoginDialog::enableLoginButton);
    connect(passwordLineEdit, &QLineEdit::textChanged, this, &LoginDialog::enableLoginButton);

    // Create a label and line edit for username.
    auto usernameLabel = new QLabel("Username:");
    usernameLabel->setBuddy(usernameLineEdit);
    // Create a label and line edit for password.
    auto passwordLabel = new QLabel("Password:");
    passwordLabel->setBuddy(passwordLineEdit);

    // Set the login button to be the default button.
    loginButton->setDefault(true);

    // Create a button box to hold the buttons.
    auto buttonBox = new QDialogButtonBox;
    // Add the buttons
    buttonBox->addButton(loginButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(registerButton, QDialogButtonBox::ActionRole);

    // Disable the login button.
    loginButton->setEnabled(false);

    // Create a grid layout to hold the widgets.
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(usernameLabel, 0, 0);
    mainLayout->addWidget(usernameLineEdit, 0, 1);
    mainLayout->addWidget(passwordLabel, 1, 0);
    mainLayout->addWidget(passwordLineEdit, 1, 1);
    mainLayout->addWidget(buttonBox, 2, 0, 1, 2);
    mainLayout->addWidget(forgotPasswordButton, 3, 0, 1, 1);

    setLayout(mainLayout);
}

/**
 * @brief Attempt to log the user in when the login button is clicked.
 */
void LoginDialog::loginButtonClicked()
{
    // Retrieve the username and password from the line edits.
    QString enteredUsername = usernameLineEdit->text();
    QString enteredPassword = passwordLineEdit->text();

    // Get database instance.
    Database *db = Database::getInstance();
    UserLogin *userLogin = db->getUserLogin(enteredUsername);

    // If login exists and password matches.
    if (userLogin != nullptr) {
        if (userLogin->checkPassword(enteredPassword)) {
            // Get the user from the database.
            User *user = db->getUser(userLogin->userID());
            // If user exists, login.
            if (user) {
                // Hide the login window.
                hide();
                // Emit the userLoggedInSignal and show main window.
                emit userLoggedInSignal(user);
            }
        } else {
            // Create a message box to display the query error message.
            QMessageBox::information(this,
                                     tr("Login Failed"),
                                     tr("Username & Password combination not found. "
                                        "Please try again."));
            // Clear the line edits.
            usernameLineEdit->clear();
            passwordLineEdit->clear();
        }
    } else {
        // Create a message box to display the query error message.
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Username does not exist! "
                                 "Please try again."));
        // Clear the line edits.
        usernameLineEdit->clear();
        passwordLineEdit->clear();
    }
}

/**
 * @brief Open the register dialog when the register button is clicked.
 */
void LoginDialog::registerButtonClicked()
{
    // Hide the login window
    hide();
    // Create the register dialog.
    registerDialog = new RegisterDialog(this);
    // When user has been registered, run userRegistered function.
    connect(registerDialog, &RegisterDialog::userRegistered, this, &LoginDialog::userRegistered);
    // When user has been registered, run userRegistered function.
    connect(registerDialog,
            &RegisterDialog::registrationCancelled,
            this,
            &LoginDialog::registerDialogClosed);
    // Show the register dialog.
    registerDialog->show();
}

/**
 * @brief Show the login dialog when the user has been successfully registered.
 */
void LoginDialog::userRegistered()
{
    // Show the login window.
    show();
}

/**
 * @brief Show the login dialog if the register dialog is closed.
 */
void LoginDialog::registerDialogClosed()
{
    // Delete the register dialog.
    registerDialog = nullptr;
    // Show the login window.
    show();
}

/**
 * @brief Open the forgot password dialog when the forgot password button is clicked.
 */
void LoginDialog::forgotPasswordButtonClicked()
{
    // Hide the login window.
    hide();
    // Create the password recovery dialog.
    passwordRecoveryDialog = new PasswordResetDialog(this);
    // When password has been recovered, run passwordRecovered function.
    connect(passwordRecoveryDialog,
            &PasswordResetDialog::passwordRecoveryClosed,
            this,
            &LoginDialog::passwordRecoveryClosed);
    // Show the password recovery dialog.
    passwordRecoveryDialog->show();
}

/**
 * @brief Show the login dialog if the password recovery dialog is closed.
 */
void LoginDialog::passwordRecoveryClosed()
{
    // Show the login window.
    show();
    // Set the password recovery dialog to nullptr.
    passwordRecoveryDialog = nullptr;
}

/**
 * @brief Enable the login button when both username and password are filled in.
 */
void LoginDialog::enableLoginButton()
{
    loginButton->setEnabled(!usernameLineEdit->text().isEmpty()
                            && !passwordLineEdit->text().isEmpty());
}
