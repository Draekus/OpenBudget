#ifndef RECOVERPASSWORDDIALOG_H
#define RECOVERPASSWORDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class PasswordResetDialog : public QDialog
{
    Q_OBJECT

public:
    PasswordResetDialog(QWidget *parent = nullptr);

signals:
    void passwordRecoveryClosed(); // Signal to indicate that the password recovery dialog has been closed.

private slots:
    void recoverPasswordButtonClicked(); // Attempt to recover the user's password when the recover password button is clicked.
    void cancelButtonClicked(); // Close the dialog when the cancel button is clicked.
    void enableResetPasswordButton(); // Enable the recover password button when the email line edit is not empty.

private:
    QLineEdit *emailLineEdit;           // Email line edit.
    QLineEdit *newPasswordLineEdit;     // New password line edit.
    QLineEdit *confirmPasswordLineEdit; // Confirm new password line edit.
    QPushButton *recoverPasswordButton; // Recover password button.
    QPushButton *cancelButton; // Return to the login dialog when the cancel button is clicked.
    QLabel *usernameLabel;     // Username label.
    QLabel *passwordLabel;     // Password label.
};

#endif // RECOVERPASSWORDDIALOG_H
