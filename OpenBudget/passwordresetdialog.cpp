// recoverpassworddialog.cpp
#include "passwordresetdialog.h"
#include "database.h"
#include "qdialogbuttonbox.h"

#include <QFormLayout>
#include <QMessageBox>
#include <QVBoxLayout>

#include <QSqlError>
#include <QSqlQuery>

/**
 * @brief Dialog that allows the user to recover their password
 * 
 * @param parent QWidget pointer to the parent widget.
 */
PasswordResetDialog::PasswordResetDialog(QWidget *parent)
    : QDialog{parent}
    , emailLineEdit{new QLineEdit(this)}
    , newPasswordLineEdit{new QLineEdit(this)}
    , confirmPasswordLineEdit{new QLineEdit(this)}
    , recoverPasswordButton{new QPushButton("Reset Password", this)}
    , cancelButton{new QPushButton("Close", this)}

{
    // Set window title.
    setWindowTitle("Recover Password");

    // Disable the recover password button until the email line edit is filled in.
    recoverPasswordButton->setEnabled(false);

    // Set the echo mode for password line edits to PasswordEchoOnEdit
    newPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

    // When the recover password button is clicked, call the recoverPasswordButtonClicked() function.
    connect(recoverPasswordButton,
            &QPushButton::clicked,
            this,
            &PasswordResetDialog::recoverPasswordButtonClicked);
    // When the cancel button is clicked, close the dialog.
    connect(cancelButton, &QPushButton::clicked, this, &PasswordResetDialog::cancelButtonClicked);
    // Only enable the reset password button when the email line edit is not empty.
    // And the new password & confim password line edits are not empty and match.
    connect(newPasswordLineEdit,
            &QLineEdit::textChanged,
            this,
            &PasswordResetDialog::enableResetPasswordButton);
    connect(confirmPasswordLineEdit,
            &QLineEdit::textChanged,
            this,
            &PasswordResetDialog::enableResetPasswordButton);

    // Create a form layout
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Email:", emailLineEdit);
    formLayout->addRow("New Password:", newPasswordLineEdit);
    formLayout->addRow("Confirm Password:", confirmPasswordLineEdit);

    // Create a button box to hold the buttons.
    auto buttonBox = new QDialogButtonBox;
    // Add the buttons
    buttonBox->addButton(recoverPasswordButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);

    // Create a vertical main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

/**
 * @brief Attempt to recover the user's password when the recover password button is clicked.
 */
void PasswordResetDialog::recoverPasswordButtonClicked()
{
    bool passwordReset = false;

    // Extract email from line edit.
    QString email = emailLineEdit->text();

    // Get a databse instance
    Database *db = Database::getInstance();
    // Get the user login from the database
    UserLogin *userLogin = db->getUserLoginEmail(email);

    if (userLogin == nullptr) {
        QMessageBox::critical(this,
                              "Error",
                              "Email not found.\n"
                              "Please try again.");
        return;
    }

    // Extract the new password from the line edit.
    QString newPassword = newPasswordLineEdit->text();

    // Update the user's password in the database.
    if (!db->updatePassword(userLogin, newPassword)) {
        // Create a message box to display the query error message.
        QMessageBox::critical(this,
                              "Error",
                              "Password reset failed.\n"
                              "Please try again.");
        return;
    }

    // Set passworReset flag to true.
    passwordReset = true;

    // If the password was reset, notify the user.
    if (passwordReset) {
        QMessageBox::information(this,
                                 "Password Reset",
                                 "Password reset successful.\n"
                                 "Please login with your new password.");
        emit passwordRecoveryClosed();
        close();
    }
}

/**
 * @brief Close the dialog when the close button is clicked.
 */
void PasswordResetDialog::cancelButtonClicked()
{
    emit passwordRecoveryClosed();
    close();
}

/**
 * @brief Enable the recover password button if the email line edit is not empty.
 */
void PasswordResetDialog::enableResetPasswordButton()
{
    QString email = emailLineEdit->text();
    QString newPassword = newPasswordLineEdit->text();
    QString confirmNewPassword = confirmPasswordLineEdit->text();

    // Enable the button only if the email, new password, and confirm new password are not empty
    // and the new password and confirm new password match
    recoverPasswordButton->setEnabled(!email.isEmpty() && !newPassword.isEmpty()
                                      && !confirmNewPassword.isEmpty()
                                      && newPassword == confirmNewPassword);
}
