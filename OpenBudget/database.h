#ifndef DATABASE_H
#define DATABASE_H

#include <QListWidget>
#include <QSqlDatabase>
#include "transaction.h"
#include "user.h"
#include "userlogin.h"

class Database
{
private:
    // Singleton instance of Database.
    static Database *INSTANCE;

public:
    // Singleton instance getter.
    static Database *getInstance();

private:
    Database();
    ~Database();

public:
    // Get user from database by userID.
    // Returns nullptr if user not found.
    User *getUser(int userID);

    // Get user from database by username.
    // Returns nullptr if user not found.
    User *getUser(const QString username);

    // Create user in database.
    // Returns nullptr if user failed to be created.
    User *createUser(const QString firstName, const QString lastName, Position position);

    // Get user login from database by username.
    // Returns nullptr if user login not found.
    UserLogin *getUserLogin(const QString username);

    // Get user login from database by email.
    // Returns nullptr if user login not found.
    UserLogin *getUserLoginEmail(const QString email);

    // Create user login in database.
    // Returns nullptr if user login failed to be created.
    UserLogin *createUserLogin(const QString username,
                               const QString password,
                               AccessLevel accessLevel,
                               const QString email,
                               int userID);

    // Update user login password.
    // Returns true if password was updated successfully.
    bool updatePassword(UserLogin *userLogin, const QString password);

    /* Retrieval Methods */

    // Get transactions from database by userID.
    // Returns nullptr if transactions not found.
    QVector<Transaction *> getTransactions(int userID);

    // Get transactions from database by userID and budgetID.
    // Returns nullptr if transactions not found.
    QVector<Transaction *> getTransactions(int userID, int budgetID);

    // Get transactions from database by userID and categoryID.
    // Returns nullptr if transactions not found.
    QVector<Transaction *> getTransactionsByCategory(int userID, int categoryID);

    // Get all category names from database by userID.
    // Returns nullptr if category names not found.
    QMap<int, QString> getCategoryNames(int userID);

    // Get a single category name from database by categoryID and userID.
    // Returns nullptr if category names not found.
    QMap<int, QString> getCategoryName(int userID, int categoryID);

    // Get all subcategory names from database by categoryID.
    // Returns nullptr if subcategory names not found.
    QMap<int, QString> getSubcategoryNames(int userID, int categoryID);

    // Get subcategory names from database by categoryID.
    // Returns nullptr if subcategory names not found.
    QString getSubcategoryName(int categoryID, int subcategoryID);

    /* Insertion Methods */

    // Inert transaction into database.
    // Returns nullptr if transaction failed to be created.
    Transaction *createTransaction(double amount,
                                   QString description,
                                   QString date,
                                   int categoryID,
                                   int subcategoryID,
                                   int userID,
                                   bool isDeposit = false);

    // Insert category into database.
    // Returns true if category was created successfully.
    bool createCategory(const QString &categoryName, int userID);

    // Insert subcategory into database.
    // Returns true if subcategory was created successfully.
    bool createSubcategory(const QString &subcategoryName, int userID, int categoryID);

    /* Deletion Methods */

    // Delete transaction from database.
    // Returns true if transaction was deleted successfully.
    bool deleteTransaction(int transactionID);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
