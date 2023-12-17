#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>

class Transaction
{
public:
    Transaction();
    // Constructor for transactions retrieved from database.
    Transaction(int transactionID,
                double amount,
                QString description,
                QString date,
                int categoryID,
                int subcategoryID,
                double balance,
                int userID,
                bool isDeposit = false);
    // Constructor for transactions created by the user.
    Transaction(double amount,
                QString description,
                QString date,
                int categoryID,
                int subcategoryID,
                int userID,
                bool isDeposit = false);
    ~Transaction();

    // Retrieve transaction details.
    int transactionID() const;
    double amount() const;
    QString description() const;
    QString date() const;
    int categoryID() const;
    int subcategoryID() const;
    double balance() const;
    int userID() const;
    bool isDeposit() const;

    // Set transaction details.
    void setTransactionID(const int &transactionID);
    void setAmount(const double &amount);
    void setDescription(const QString &description);
    void setDate(const QString &date);
    void setCategoryID(const int &categoryID);
    void setSubcategoryID(const int &subcategoryID);
    void setBalance(const double &balance);
    void setUserID(const int &userID);

    // To string.
    QString toString() const;

private:
    int m_transactionID;
    double m_amount;
    QString m_description;
    QString m_date;
    int m_categoryID;
    int m_subcategoryID;
    double m_balance;
    int m_userID;
    bool m_isDeposit;
};

#endif // TRANSACTION_H
