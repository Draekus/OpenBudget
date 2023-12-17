// registerdialog.cpp
#include "registerdialog.h"
#include "database.h"
#include "qdialogbuttonbox.h"

#include <QFormLayout>
#include <QMessageBox>
#include <QVBoxLayout>

#include <QSqlError>
#include <QSqlQuery>

/**
 * @brief Dialog that allows users to register a new account.
 * 
 * @param parent QWidget pointer to parent widget.
 */
RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog{parent}
    , firstNameLineEdit{new QLineEdit(this)}
    , lastNameLineEdit{new QLineEdit(this)}
    , usernameLineEdit{new QLineEdit(this)}
    , passwordLineEdit{new QLineEdit(this)}
    , emailLineEdit{new QLineEdit(this)}
    , positionComboBox{new QComboBox(this)}
    , registerButton{new QPushButton("Register", this)}
    , cancelButton{new QPushButton("Cancel", this)}
{
    // Set window title.
    setWindowTitle("Register");

    // Set password line edit to hide characters.
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Add items to the position combo box.
    positionComboBox->addItem("User", 0);
    positionComboBox->addItem("Developer", 1);
    positionComboBox->addItem("Admin", 2);

    // Disable the register button until all fields are filled in.
    registerButton->setEnabled(false);

    /* Connect signals and slots */
    // When register button is clicked, run registerButtonClicked function.
    connect(registerButton, &QPushButton::clicked, this, &RegisterDialog::registerButtonClicked);
    // When cancel button is clicked, close the dialog.
    connect(cancelButton, &QPushButton::clicked, this, &RegisterDialog::cancelButtonClicked);
    // Only enable the register button when all fields are filled in.
    connect(firstNameLineEdit, &QLineEdit::textChanged, this, &RegisterDialog::enableRegisterButton);
    connect(lastNameLineEdit, &QLineEdit::textChanged, this, &RegisterDialog::enableRegisterButton);
    connect(usernameLineEdit, &QLineEdit::textChanged, this, &RegisterDialog::enableRegisterButton);
    connect(passwordLineEdit, &QLineEdit::textChanged, this, &RegisterDialog::enableRegisterButton);
    connect(emailLineEdit, &QLineEdit::textChanged, this, &RegisterDialog::enableRegisterButton);

    // Create a form layout to hold the widgets.
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("First Name:", firstNameLineEdit);
    formLayout->addRow("Last Name:", lastNameLineEdit);
    formLayout->addRow("Username:", usernameLineEdit);
    formLayout->addRow("Password:", passwordLineEdit);
    formLayout->addRow("Email:", emailLineEdit);
    formLayout->addRow("Position:", positionComboBox);

    // Create a button box to hold the buttons.
    auto buttonBox = new QDialogButtonBox;
    // Add the buttons
    buttonBox->addButton(registerButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);

    // Create a vertical layout to hold the form layout and button.
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    // Set the main layout for the window.
    setLayout(mainLayout);
}

/**
 * @brief Enable the register button if all fields are filled in.
 */
void RegisterDialog::enableRegisterButton()
{
    // Enable the register button if all fields are filled in.
    registerButton->setEnabled(
        !firstNameLineEdit->text().isEmpty() && !lastNameLineEdit->text().isEmpty()
        && !usernameLineEdit->text().isEmpty() && !passwordLineEdit->text().isEmpty()
        && !emailLineEdit->text().isEmpty());
}

/**
 * @brief Attempt to register the user in the database.
 */
void RegisterDialog::registerButtonClicked()
{
    // Flag to indicate if the user was registered successfully.
    bool registered = false;

    // Retrieve user input.
    QString firstName = firstNameLineEdit->text();
    QString lastName = lastNameLineEdit->text();
    QString userName = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QString email = emailLineEdit->text();
    Position position = static_cast<Position>(positionComboBox->currentData().toInt());
    AccessLevel accessLevel = static_cast<AccessLevel>(positionComboBox->currentData().toInt());

    // Get a database instance
    Database *db = Database::getInstance();
    // Query the database for the user's chosen username.
    UserLogin *userLogin = db->getUserLogin(userName);
    // If the user login exists, display an error message.
    if (userLogin != nullptr) {
        // Create a message box to display the query error message.
        QMessageBox::critical(this,
                              tr("Registration Failed"),
                              tr("Username already exists in database."));
        return;
    }

    // Insert the new user into the database.
    User *newUser = db->createUser(firstName, lastName, position);
    // If the user was not created, display an error message.
    if (newUser == nullptr) {
        // Create a message box to display the query error message.
        QMessageBox::critical(this, tr("Registration Failed"), tr("Failed to create user."));
        return;
    }

    // Insert the new user login into the database.
    UserLogin *newUserLogin = db->createUserLogin(userName,
                                                  password,
                                                  accessLevel,
                                                  email,
                                                  newUser->userID());
    // If the user login was not created, display an error message.
    if (newUserLogin == nullptr) {
        // Create a message box to display the query error message.
        QMessageBox::critical(this, tr("Registration Failed"), tr("Failed to create user login."));
        return;
    }

    // Create a message box notifying the user of successful registration.
    QMessageBox::information(this,
                             tr("Registration Successful"),
                             tr("User Account Registered \n"
                                "Please login to continue."));

    // Set the registered flag to true.
    registered = true;

    // If the user was registered successfully, emit the signal.
    if (registered) {
        // Emit the signal
        emit userRegistered();
        // Close the dialog
        accept();
    }
}

/**
 * @brief Cancel user registration and return to the login dialog.
 *
 */
void RegisterDialog::cancelButtonClicked()
{
    // Close the dialog
    emit registrationCancelled();
    close();
}
