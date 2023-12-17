// registerdialog.h
#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    RegisterDialog(QWidget *parent = nullptr);

signals:
    void userRegistered();        // Signal to indicate that the user has been registered.
    void registrationCancelled(); // Signal to indicate that the registration has been cancelled.

private slots:
    void enableRegisterButton();  // Enable the register button when all fields are filled.
    void registerButtonClicked(); // Register the user when the register button is clicked.
    void cancelButtonClicked();   // Cancel registration when the cancel button is clicked.

private:
    QLineEdit *firstNameLineEdit = nullptr; // First name line edit.
    QLineEdit *lastNameLineEdit = nullptr;  // Last name line edit.
    QLineEdit *usernameLineEdit = nullptr;  // Username line edit.
    QLineEdit *passwordLineEdit = nullptr;  // Password line edit.
    QLineEdit *emailLineEdit = nullptr;     // Email line edit.
    QComboBox *positionComboBox = nullptr;  // Position combo box.
    QPushButton *registerButton = nullptr;  // Register button.
    QPushButton *cancelButton = nullptr;    // Cancel button.
};

#endif // REGISTERDIALOG_H
