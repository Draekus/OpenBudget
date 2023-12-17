#include "database.h"
#include "position.h"
#include "qstandardpaths.h"
#include "user.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

#include <QDir>

// Singleton instance of Database.
Database *Database::INSTANCE = nullptr;

/**
 * @brief Database singleton instance getter.
 * @return Database singleton instance.
 */
Database *Database::getInstance()
{
    if (!INSTANCE) {
        INSTANCE = new Database();
    }

    return INSTANCE;
}

/**
 * @brief Database middleware for facilitating database operations.
 */
Database::Database()
{
    // Get the user's home directory
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    // Specify the name of the SQLite database file
    QString databaseFileName = QDir(homeDir).filePath("openbudget.db");

    // Choose SQLite database driver.
    db = QSqlDatabase::addDatabase("QSQLITE");

    // Set name of database.
    db.setDatabaseName(databaseFileName);

    // Open database connection.
    if (!db.open()) {
        // If connection fails, display error message.
        QMessageBox::critical(nullptr,
                              QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connection.\n"
                                          "This example needs SQLite support. Please read "
                                          "the Qt SQL driver documentation for information how "
                                          "to build it.\n\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }

    // Create a query.
    QSqlQuery query;

    // Create User table.
    query.exec("CREATE TABLE IF NOT EXISTS User ("
               "userID INTEGER PRIMARY KEY AUTOINCREMENT, "
               "firstname TEXT NOT NULL, "
               "lastname TEXT NOT NULL, "
               "position INTEGER NOT NULL CHECK (position IN (0, 1, 2)))");
    if (!query.isActive()) {
        qDebug() << "Error creating User table: " << query.lastError().text();
    }

    // Create UserLogin table.
    query.exec("CREATE TABLE IF NOT EXISTS UserLogin ("
               "loginID INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT NOT NULL UNIQUE, "
               "password TEXT NOT NULL, "
               "accessLevel INTEGER NOT NULL CHECK (accessLevel IN (0, 1, 2)), "
               "email TEXT NOT NULL UNIQUE, "
               "userID INTEGER NOT NULL, "
               "FOREIGN KEY(userID) REFERENCES User(userID))");
    if (!query.isActive()) {
        qDebug() << "Error creating UserLogin table: " << query.lastError().text();
    }

    // Create Category table.
    query.exec("CREATE TABLE IF NOT EXISTS Category ("
               "categoryID INTEGER PRIMARY KEY AUTOINCREMENT, "
               "categoryName TEXT NOT NULL, "
               "userID INTEGER NOT NULL, "
               "FOREIGN KEY(userID) REFERENCES User(userID))");

    if (!query.isActive()) {
        qDebug() << "Error creating Category table: " << query.lastError().text();
    }

    // Create Subategory table.
    query.exec("CREATE TABLE IF NOT EXISTS Subcategory ("
               "subcategoryID INTEGER PRIMARY KEY AUTOINCREMENT, "
               "subcategoryName TEXT NOT NULL, "
               "categoryID INTEGER NOT NULL, "
               "userID INTEGER NOT NULL, "
               "FOREIGN KEY(userID) REFERENCES User(userID),"
               "FOREIGN KEY(categoryID) REFERENCES Category(categoryID))");

    if (!query.isActive()) {
        qDebug() << "Error creating Subcategory table: " << query.lastError().text();
    }

    // Create Transactions table.
    query.exec("CREATE TABLE IF NOT EXISTS Transactions ("
               "transactionID INTEGER PRIMARY KEY AUTOINCREMENT, "
               "amount DECIMAL(10,2) NOT NULL, "
               "description TEXT, "
               "transactionDate TEXT NOT NULL, "
               "categoryID INTEGER NOT NULL, "
               "subcategoryID INTEGER, "
               "userID INTEGER NOT NULL, "
               "isDeposit BOOLEAN NOT NULL,"
               "FOREIGN KEY(categoryID) REFERENCES Category(categoryID), "
               "FOREIGN KEY(subcategoryID) REFERENCES Subcategory(subcategoryID), "
               "FOREIGN KEY(userID) REFERENCES User(userID))");
    if (!query.isActive()) {
        qDebug() << "Error creating Transaction table: " << query.lastError().text();
    }

    // Create View to dynamically calculate balance
    query.exec(
        " CREATE VIEW IF NOT EXISTS TransactionsView AS "
        " SELECT t.*, "
        " (SELECT SUM(amount) FROM Transactions WHERE transactionID <= t.transactionID) AS balance "
        " FROM Transactions AS t");
    if (!query.isActive()) {
        qDebug() << "Error creating balance view: " << query.lastError().text();
    }
}

/**
 * @brief Database destructor.
 */
Database::~Database()
{
    // Close database connection.
    db.close();
    // Delete database instance.
    delete INSTANCE;
}

/**
 * @brief Retrieves a user from the database.
 * 
 * @param userID The ID of the user.
 * @returns A pointer to a User; nullptr if the user does not exist.
 */
User *Database::getUser(int userID)
{
    // Initialize a user pointer.
    User *user = nullptr;

    // Create a query to retrieve the user.
    QSqlQuery query;
    query.prepare("SELECT * FROM User WHERE userID = ?");
    query.addBindValue(userID);

    // Query the database for the user.
    if (query.exec()) {
        // If the user exists, create a new user.
        if (query.next()) {
            // Extract the user information.
            userID = query.value("userID").toInt();
            QString firstName = query.value("firstname").toString();
            QString lastName = query.value("lastname").toString();
            Position position = static_cast<Position>(query.value("position").toInt());
            // Create a new user.
            user = new User(firstName, lastName, position, userID);
        }
    }

    // Return the queried user.
    return user;
}

/**
 * @brief Inserts a user to the database.
 * 
 * @param firstName The first name of the user.
 * @param lastName The last name of the user.
 * @param position The position of the user.
 * @returns A pointer to a User; nullptr if the user could not be created.
 */
User *Database::createUser(const QString firstName, const QString lastName, Position position)
{
    // Initialize a user pointer.
    User *user = nullptr;

    // Create a query to insert the user into the database.
    QSqlQuery query;
    query.prepare("INSERT INTO User (firstname, lastname, position) "
                  "VALUES (?, ?, ?)");
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(static_cast<int>(position));

    // Insert the user into the database.
    if (query.exec()) {
        // Retrieve the user ID.
        int userID = query.lastInsertId().toInt();
        // Create a new user.
        user = new User(firstName, lastName, position, userID);
    }

    return user;
}

/**
 * @brief Retrieves a user login from the database.
 * 
 * @param username The username of the user.
 * @return A user login pointer; nullptr if the user login does not exist.
 */
UserLogin *Database::getUserLogin(const QString username)
{
    // Initialize a user login pointer.
    UserLogin *userLogin = nullptr;

    // Create a query to retrieve the user login.
    QSqlQuery query;
    query.prepare("SELECT * FROM UserLogin WHERE username = :username");
    query.bindValue(":username", username);

    // Query the database for the user login.
    if (query.exec()) {
        // If the user login exists, create a new user login.
        if (query.next()) {
            // Extract the user login information.
            QString username = query.value("username").toString();
            QString password = query.value("password").toString();
            AccessLevel accessLevel = static_cast<AccessLevel>(query.value("accessLevel").toInt());
            QString email = query.value("email").toString();
            int userID = query.value("userID").toInt();

            // Create a new user login.
            userLogin = new UserLogin(username, password, accessLevel, email, userID);
        }
    }
    return userLogin;
}

/**
 * @brief Retrieve a user login from the database
 * 
 * @param email The email of the user
 * @return A user login pointer; nullptr if the user login does not exist.
 */
UserLogin *Database::getUserLoginEmail(const QString email)
{
    // Initialize a user login pointer.
    UserLogin *userLogin = nullptr;

    // Create a query to retrieve the user login.
    QSqlQuery query;
    query.prepare("SELECT * FROM UserLogin WHERE email = :email");
    query.bindValue(":email", email);

    // Query the database for the user login.
    if (query.exec()) {
        // If the user login exists, create a new user login.
        if (query.next()) {
            // Extract the user login information.
            QString username = query.value("username").toString();
            QString password = query.value("password").toString();
            AccessLevel accessLevel = static_cast<AccessLevel>(query.value("accessLevel").toInt());
            QString email = query.value("email").toString();
            int userID = query.value("userID").toInt();
            // Create a new user login.
            userLogin = new UserLogin(username, password, accessLevel, email, userID);
        }
    }
    return userLogin;
}

/**
 * @brief Creates a user login.
 * 
 * @param username The username.
 * @param password The password.
 * @param accessLevel The access level.
 * @param userId The ID of the user.
 * @return A UserLogin pointer; nullptr if it fails to create.
 */
UserLogin *Database::createUserLogin(const QString username,
                                     const QString password,
                                     AccessLevel accessLevel,
                                     const QString email,
                                     int userID)
{
    // Initialize a user login pointer.
    UserLogin *userLogin = new UserLogin(username, password, accessLevel, email, userID, true);

    // Create a query to insert the user login into the database.
    QSqlQuery query;
    query.prepare("INSERT INTO UserLogin(username, password, accessLevel, email, userID) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(userLogin->username());
    query.addBindValue(userLogin->password());
    query.addBindValue(static_cast<int>(userLogin->accessLevel()));
    query.addBindValue(userLogin->email());
    query.addBindValue(userLogin->userID());

    // Insert the user login into the database.
    if (query.exec()) {
        // If insert succeeds return the user login.
        return userLogin;
    } else {
        // If insert fails delete the user login.
        delete userLogin;
        return nullptr;
    }
}

/**
 * @brief Updates a user's password.
 * 
 * @param username The username of the passowrd.
 * @param password The new password.
 * @return True if the password was updated; false otherwise.
 */
bool Database::updatePassword(UserLogin *userLogin, QString password)
{
    // Extract the user login information.
    QString username = userLogin->username();
    QString currentPassword = userLogin->password();

    // Change the user's password
    userLogin->setPassword(password);
    QString newPassword = userLogin->password();

    // Create a query to update the user's password in the database.
    QSqlQuery query;
    query.prepare("UPDATE UserLogin SET password = :password WHERE username = :username");
    query.bindValue(":password", newPassword);
    query.bindValue(":username", username);

    // Update the user's password in the database.
    if (query.exec()) {
        // If update succeeds, return true.
        return true;
    } else {
        // If update fails, revert the user login's password.
        userLogin->resetPassword(currentPassword);
        // Return false.
        return false;
    }
}

/**
 * @brief Retrieves a user's transactions.
 * 
 * @param userID The ID of the user.
 * @return QVector of transactions; empty if the user does not exist.
 */
QVector<Transaction *> Database::getTransactions(int userID)
{
    // Create a query to retrieve the user's transactions.
    QSqlQuery query;
    query.prepare("SELECT * FROM TransactionsView WHERE userID = :userID");
    query.bindValue(":userID", userID);

    // Initialize a vector of transactions.
    QVector<Transaction *> transactions;

    // Query the database for the user's transactions.
    if (query.exec()) {
        // If the query is successful, iterate through the results.
        while (query.next()) {
            // Extract the transaction information.
            int transactionID = query.value("transactionID").toInt();
            double amount = query.value("amount").toDouble();
            QString description = query.value("description").toString();
            QString date = query.value("transactionDate").toString();
            int categoryID = query.value("categoryID").toInt();
            int subcategoryID = query.value("subcategoryID").toInt();
            double balance = query.value("balance").toDouble();
            int userID = query.value("userID").toInt();
            bool isDeposit = query.value("isDeposit").toBool();

            // Create a new transaction.
            Transaction *transaction = new Transaction(transactionID,
                                                       amount,
                                                       description,
                                                       date,
                                                       categoryID,
                                                       subcategoryID,
                                                       balance,
                                                       userID,
                                                       isDeposit);

            // Add the transaction to the vector.
            transactions.push_back(transaction);
        }
    }

    // Return the vector of transactions, empty if query failed.
    return transactions;
}

/**
 * @brief Retrieves a user's transactions by category.
 * 
 * @param userID The ID of the user.
 * @param categoryID The ID of the category.
 * @return QVector of transactions; empty if the user does not exist.
 */
QVector<Transaction *> Database::getTransactionsByCategory(int userID, int categoryID)
{
    // Create a query to retrieve the user's transactions.
    QSqlQuery query;
    query.prepare(
        "SELECT * FROM TransactionsView WHERE userID = :userID AND categoryID = :categoryID");
    query.bindValue(":userID", userID);
    query.bindValue(":categoryID", categoryID);

    // Initialize a vector of transactions.
    QVector<Transaction *> transactions;

    // Query the database for the user's transactions.
    if (query.exec()) {
        // If the query is successful, iterate through the results.
        while (query.next()) {
            // Extract the transaction information.
            int transactionID = query.value("transactionID").toInt();
            double amount = query.value("amount").toDouble();
            QString description = query.value("description").toString();
            QString date = query.value("transactionDate").toString();
            int categoryID = query.value("categoryID").toInt();
            int subcategoryID = query.value("subcategoryID").toInt();
            double balance = query.value("balance").toDouble();
            int userID = query.value("userID").toInt();
            bool isDeposit = query.value("isDeposit").toBool();

            // Create a new transaction.
            Transaction *transaction = new Transaction(transactionID,
                                                       amount,
                                                       description,
                                                       date,
                                                       categoryID,
                                                       subcategoryID,
                                                       balance,
                                                       userID,
                                                       isDeposit);

            // Add the transaction to the vector.
            transactions.push_back(transaction);
        }
    }

    // Return the vector of transactions, empty if query failed.
    return transactions;
}

/**
 * @brief Retrieves a user's categories.
 * 
 * @param userID The ID of the user.
 * @return QMap of category ID's to names; empty if the user does not exist.
 */
QMap<int, QString> Database::getCategoryNames(int userID)
{
    // Create a query to retrieve the user's category.
    QSqlQuery query;
    query.prepare("SELECT * FROM Category WHERE userID = :userID OR userID = 0");
    query.bindValue(":userID", userID);

    // Initialize a map of category ID's to names.
    QMap<int, QString> categories;

    // Query the database for the user's categories.
    if (query.exec()) {
        // If the query is successful, iterate through the results.
        while (query.next()) {
            // Extract the category information.
            int categoryID = query.value("categoryID").toInt();
            QString categoryName = query.value("categoryName").toString();

            // Insert the ID and name into the categories map.
            categories.insert(categoryID, categoryName);
        }
    }

    // Return the map of category ID's to names, empty if query failed.
    return categories;
}

/**
 * @brief Retrieves a specific category from user's categories.
 * 
 * @param userID The ID of the user.
 * @param categoryID The ID of the category.
 * @return QMap of subcategory ID's to names; empty if the user does not exist.
 */
QMap<int, QString> Database::getCategoryName(int userID, int categoryID)
{
    // Create a query to retrieve the user's category.
    QSqlQuery query;
    query.prepare("SELECT * FROM Category WHERE userID = :userID AND categoryID = :categoryID");
    query.bindValue(":userID", userID);
    query.bindValue(":categoryID", categoryID);

    // Initialize a map of category ID's to names.
    QMap<int, QString> categories;

    // Query the database for the user's category.
    if (query.exec()) {
        // If the query is successful, iterate through the results.
        while (query.next()) {
            // Extract the category information.
            int categoryID = query.value("categoryID").toInt();
            QString categoryName = query.value("categoryName").toString();

            // Insert the ID and name into the categories map.
            categories.insert(categoryID, categoryName);
        }
    }

    // Return the map of category ID's to names, empty if query failed.
    return categories;
}

/**
 * @brief Retrieves a user's subcategories.
 * 
 * @param userID The ID of the user.
 * @param categoryID The ID of the parent category.
 * @return QMap of subcategory ID's to names; empty if the user does not exist.
 */
QMap<int, QString> Database::getSubcategoryNames(int userID, int categoryID)
{
    // Create a query to retrieve the user's subcategories.
    QSqlQuery query;
    query.prepare("SELECT * FROM Subcategory WHERE userID = :userID AND categoryID = :categoryID");
    query.bindValue(":userID", userID);
    query.bindValue(":categoryID", categoryID);

    // Initialize a map of subcategory ID's to names.
    QMap<int, QString> subcategories;

    // Query the database for the user's subcategories.
    if (query.exec()) {
        // If the query is successful, iterate through the results.
        while (query.next()) {
            // Extract the subcategory information.
            int subcategoryID = query.value("subcategoryID").toInt();
            QString subcategoryName = query.value("subcategoryName").toString();

            // Insert the ID and name into the subcategories map.
            subcategories.insert(subcategoryID, subcategoryName);
        }
    }

    return subcategories;
}

/**
 * @brief Retrieves the name of a specific subcategory.
 * 
 * @param categoryID The ID of the parent category.
 * @param subcategoryID The ID of the subcategory.
 * @return QMap of subcategory ID's to names; empty if the user does not exist.
 */
QString Database::getSubcategoryName(int categoryID, int subcategoryID)
{
    // Initialize a string for the subcategory name.
    QString subcategoryName = "";
    // Create a query to retrieve the user's subcategories.
    QSqlQuery query;
    query.prepare("SELECT subcategoryName FROM Subcategory WHERE categoryID = :categoryID AND "
                  "subcategoryID = :subcategoryID");
    query.bindValue(":categoryID", categoryID);
    query.bindValue(":subcategoryID", subcategoryID);

    // Query the database for the user's subcategories.
    if (query.exec()) {
        // Set the query to the first result.
        query.next();
        // Extract the subcategory information.
        subcategoryName = query.value("subcategoryName").toString();
    }

    // Return the name of the subcategory, empty if query failed.
    return subcategoryName;
}

/**
 * @brief Create a transaction and insert it into the database.
 * 
 * @param amount The amount of the transaction.
 * @param description The description of the transaction.
 * @param date The date of the transaction.
 * @param categoryID The ID of the category.
 * @param subcategoryID The ID of the subcategory.
 * @param userID The ID of the user.
 * @param isDeposit Whether the transaction is a deposit.
 * @return The created transaction; NULL if the transaction could not be created.
 */
Transaction *Database::createTransaction(double amount,
                                         QString description,
                                         QString date,
                                         int categoryID,
                                         int subcategoryID,
                                         int userID,
                                         bool isDeposit)
{
    // If the transaction is a deposit.
    if (isDeposit) {
        // Make the amount positive.
        amount = abs(amount);
        // Set category ID to 0 (Deposit).
        categoryID = 0;
        // Set subcategory ID to NULL.
        subcategoryID = 0;

    } else {
        // If the transaction is a withdrawal.
        // Make the amount negative.
        amount = (abs(amount) * -1);
    }

    // Create a new transaction.
    Transaction *transaction
        = new Transaction(amount, description, date, categoryID, subcategoryID, userID, isDeposit);
    qDebug() << "Database::createTransaction()";
    qDebug() << "Transaction: " << transaction->toString();
    // Create a query to insert the transaction into the database.
    QSqlQuery query;
    query.prepare("INSERT INTO Transactions (amount, description, transactionDate, categoryID, "
                  "subcategoryID, userID, isDeposit) VALUES (:amount, :description, :date, "
                  ":categoryID, :subcategoryID, :userID, :isDeposit)");
    query.bindValue(":amount", transaction->amount());
    query.bindValue(":description", transaction->description());
    query.bindValue(":date", transaction->date());
    query.bindValue(":categoryID", transaction->categoryID());
    query.bindValue(":subcategoryID", transaction->subcategoryID());
    query.bindValue(":userID", transaction->userID());
    query.bindValue(":isDeposit", transaction->isDeposit());

    // Execute the query.
    if (query.exec()) {
        // If the query is successful, return the transaction.
        return transaction;

    } else {
        // If the query fails, print an error message.
        return nullptr;
    }
}

/**
 * @brief Insert a new category into the database.
 * @param categoryName The name of the category.
 * @param userID The ID of the user.
 * @return Returns true if the category was successfully created, false otherwise.
 */
bool Database::createCategory(const QString &categoryName, int userID)
{
    // Create a query to insert the category into the database.
    QSqlQuery query;
    query.prepare("INSERT INTO Category (categoryName, userID) VALUES (:categoryName, :userID)");
    query.bindValue(":categoryName", categoryName);
    query.bindValue(":userID", userID);

    // Execute the query.
    if (query.exec()) {
        // If the query is successful, return true.
        return true;

    } else {
        // If the query fails, print an error message.
        qDebug() << query.lastError().text();
        return false;
    }
}

/**
 * @brief Insert a new subcategory into the database.
 * @param subcategoryName The name of the subcategory.
 * @param userID The ID of the user.
 * @param categoryID The ID of the parent category.
 * @return Returns true if the subcategory was successfully created, false otherwise.
 */
bool Database::createSubcategory(const QString &subcategoryName, int userID, int categoryID)
{
    // Create a query to insert the subcategory into the database.
    QSqlQuery query;
    query.prepare("INSERT INTO Subcategory (subcategoryName, userID, categoryID) VALUES "
                  "(:subcategoryName, :userID, :categoryID)");
    query.bindValue(":subcategoryName", subcategoryName);
    query.bindValue(":userID", userID);
    query.bindValue(":categoryID", categoryID);

    // Execute the query.
    if (query.exec()) {
        // If the query is successful, return true.
        return true;

    } else {
        // If the query fails, print an error message.
        qDebug() << query.lastError().text();
        return false;
    }
}

/**
 * @brief Delete a transaction from the database.
 * @param transactionID The ID of the transaction.
 * @return Returns true if the transaction was successfully deleted, false otherwise.
 */
bool Database::deleteTransaction(int transactionID)
{
    // Create a query to delete the transaction from the database.
    QSqlQuery query;
    query.prepare("DELETE FROM Transactions WHERE transactionID = :transactionID");
    query.bindValue(":transactionID", transactionID);

    // Execute the query.
    if (query.exec()) {
        // If the query is successful, return true.
        return true;

    } else {
        // If the query fails, print an error message.
        qDebug() << query.lastError().text();
        return false;
    }
}
