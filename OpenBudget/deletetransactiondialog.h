#ifndef DELETETRANSACTIONDIALOG_H
#define DELETETRANSACTIONDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class DeleteTransactionDialog : public QDialog
{
    Q_OBJECT

public:
    DeleteTransactionDialog(int transactionID,
                            const QString &date,
                            const QString &description,
                            const QString &amount,
                            QWidget *parent = nullptr);

signals:
    void transactionDeleted(); // Signal to indicate that a transaction has been deleted.

private slots:
    void deleteButtonClicked(); // Delete the transaction when the delete button is clicked.

private:
    int m_transactionID;
    QString m_transactionAmount;
    QString m_transactionDescription;
    QString m_transactionDate;
};

#endif // DELETETRANSACTIONDIALOG_H
