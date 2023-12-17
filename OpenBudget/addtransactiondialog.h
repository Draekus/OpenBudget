#ifndef ADDTRANSACTIONDIALOG_H
#define ADDTRANSACTIONDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

class AddTransactionDialog : public QDialog
{
    Q_OBJECT

public:
    AddTransactionDialog(QWidget *parent, int userID);

signals:
    void transactionAdded(); // Signal to indicate that a transaction has been added.

private slots:
    void addTransaction();  // Add the transaction when the add button is clicked.
    void categoryChanged(); // Enable subcategoryCombo when category is valid.
    void enableAddButton(); // Enable the add button when the amount and description are not empty and category is valid.
    void cancelAddTransaction(); // Close the dialog when the cancel button is clicked.

private:
    QLineEdit *amountLineEdit = nullptr;
    QTextEdit *descriptionTextEdit = nullptr;
    QDateEdit *dateEdit = nullptr;
    QComboBox *categoryCombo = nullptr;
    QComboBox *subcategoryComboBox = nullptr;
    QPushButton *addButton = nullptr;

private:
    void loadCategories();                  // Load categories from the database.
    void loadSubcategories(int categoryID); // Load subcategories from the database.

private:
    int m_userID;
};

#endif // ADDTRANSACTIONDIALOG_H
