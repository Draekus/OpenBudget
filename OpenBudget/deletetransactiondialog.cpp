#include "deletetransactiondialog.h"
#include <QMessageBox>
#include "database.h"

/**
 * @brief Allows the user to delete transactions from the database.
 * 
 * @param transactionID ID of the transaction to delete.
 * @param date Date of the transaction to delete.
 * @param description Description of the transaction to delete.
 * @param amount Amount of the transaction to delete.
 * @param parent Pointer to the parent widget.
 */
DeleteTransactionDialog::DeleteTransactionDialog(int transactionID,
                                                 const QString &date,
                                                 const QString &description,
                                                 const QString &amount,
                                                 QWidget *parent)
    : QDialog{parent}
    , m_transactionID{transactionID}
    , m_transactionAmount{amount}
    , m_transactionDescription{description}
    , m_transactionDate{date}
{
    setWindowTitle("Delete Transaction");

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Transaction Details Group Box
    QGroupBox *groupBox = new QGroupBox("Transaction Details");
    QFormLayout *formLayout = new QFormLayout();

    // Labels for transaction details
    QLabel *dateLabel = new QLabel("Date: " + date);
    QLabel *descriptionLabel = new QLabel("Description: " + description);
    QLabel *amountLabel = new QLabel("Amount: $" + amount);

    // Add labels to the form layout
    formLayout->addRow(dateLabel);
    formLayout->addRow(descriptionLabel);
    formLayout->addRow(amountLabel);

    // Set layout for the group box
    groupBox->setLayout(formLayout);

    // Button Box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::No);
    QPushButton *deleteButton = buttonBox->button(QDialogButtonBox::Yes);

    // Delete transaction when the delete button is clicked
    connect(deleteButton,
            &QPushButton::clicked,
            this,
            &DeleteTransactionDialog::deleteButtonClicked);
    // Close the dialog when the no button is clicked
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Add the group box and button box to the main layout
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);

    // Set the main layout for the dialog
    setLayout(mainLayout);
}

/**
 * @brief Attempts to delete the transaction from the database.
 */
void DeleteTransactionDialog::deleteButtonClicked()
{
    // Create a database instance
    Database *db;
    db = Database::getInstance();

    // Delete the transaction from the database
    if (db->deleteTransaction(m_transactionID)) {
        // If the transaction was deleted successfully, notify the user
        QMessageBox::information(this, "Success", "Transaction deleted.", QMessageBox::Ok);
        // Emit the transactionDeleted() signal
        emit transactionDeleted();
        // Close the dialog
        accept();
    } else {
        // Display an error message
        QMessageBox::critical(this, "Error", "Failed to delete transaction.", QMessageBox::Ok);
    }
}
