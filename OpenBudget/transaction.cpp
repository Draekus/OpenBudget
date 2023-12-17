#include "transaction.h"

/**
 * @brief Default constructor creates empty Transaction object.
 * 
 * @return Transaction object.
 */
Transaction::Transaction()
    : m_amount{0.0}
    , m_description{""}
    , m_date{""}
    , m_categoryID{0}
    , m_subcategoryID{0}
    , m_balance{0.0}
    , m_userID{0}
    , m_isDeposit{false}
{}

/**
 * @brief Stores transaction information for existing transactions.
 * 
 * @param transactionID Transaction ID.
 * @param amount Transaction amount.
 * @param description Transaction description.
 * @param date Transaction date.
 * @param categoryID Transaction category ID.
 * @param subcategoryID Transaction subcategory ID.
 * @param userID Transaction user ID.
 * @param isDeposit Transaction is a deposit.
 * @return Transaction object.
 */
Transaction::Transaction(int transactionID,
                         double amount,
                         QString description,
                         QString date,
                         int categoryID,
                         int subcategoryID,
                         double balance,
                         int userID,
                         bool isDeposit)
    : m_transactionID{transactionID}
    , m_amount{amount}
    , m_description{description}
    , m_date{date}
    , m_categoryID{categoryID}
    , m_subcategoryID{subcategoryID}
    , m_balance{balance}
    , m_userID{userID}
    , m_isDeposit{isDeposit}
{}

/**
 * @brief Stores transaction information for new transactions.
 * 
 * @param amount Transaction amount.
 * @param description Transaction description.
 * @param date Transaction date.
 * @param categoryID Transaction category ID.
 * @param subcategoryID Transaction subcategory ID.
 * @param userID Transaction user ID.
 * @param isDeposit Transaction is a deposit.
 * @return Transaction object.
 */
Transaction::Transaction(double amount,
                         QString description,
                         QString date,
                         int categoryID,
                         int subcategoryID,
                         int userID,
                         bool isDeposit)
    : m_amount{amount}
    , m_description{description}
    , m_date{date}
    , m_categoryID{categoryID}
    , m_subcategoryID{subcategoryID}
    , m_userID{userID}
    , m_isDeposit{isDeposit}
{}

/**
 * @brief Transaction destructor.
 */
Transaction::~Transaction()
{
    delete this;
}

/**
 * @brief Getter for transactionID.
 * 
 * @return Transaction ID.
 */
int Transaction::transactionID() const
{
    return m_transactionID;
}

/**
 * @brief Getter for amount.
 * 
 * @return Transaction amount.
 */
double Transaction::amount() const
{
    return m_amount;
}

/**
 * @brief Getter for description.
 * 
 * @return Transaction description.
 */
QString Transaction::description() const
{
    return m_description;
}

/**
 * @brief Getter for date.
 * 
 * @return Transaction date.
 */
QString Transaction::date() const
{
    return m_date;
}

/**
 * @brief Getter for categoryID.
 * 
 * @return Transaction category ID.
 */
int Transaction::categoryID() const
{
    return m_categoryID;
}

/**
 * @brief Getter for subcategoryID.
 * 
 * @return Transaction subcategory ID.
 */
int Transaction::subcategoryID() const
{
    return m_subcategoryID;
}

/**
 * @brief Getter for balance.
 * 
 * @return Transaction balance.
 */
double Transaction::balance() const
{
    return m_balance;
}

/**
 * @brief Getter for userID.
 * 
 * @return Transaction user ID.
 */
int Transaction::userID() const
{
    return m_userID;
}

/**
 * @brief Getter for isDeposit.
 * 
 * @return Transaction is a deposit.
 */
bool Transaction::isDeposit() const
{
    return m_isDeposit;
}

/**
 * @brief Setter for transactionID.
 * 
 * @param transactionID Transaction ID.
 */
void Transaction::setTransactionID(const int &transactionID)
{
    m_transactionID = transactionID;
}

/**
 * @brief Setter for amount.
 * 
 * @param amount Transaction amount.
 */
void Transaction::setAmount(const double &amount)
{
    m_amount = amount;
}

/**
 * @brief Setter for description.
 * 
 * @param description Transaction description.
 */
void Transaction::setDescription(const QString &description)
{
    m_description = description;
}

/**
 * @brief Setter for date.
 * 
 * @param date Transaction date.
 */
void Transaction::setDate(const QString &date)
{
    m_date = date;
}

/**
 * @brief Setter for categoryID.
 * 
 * @param categoryID Transaction category ID.
 */
void Transaction::setCategoryID(const int &categoryID)
{
    m_categoryID = categoryID;
}

/**
 * @brief Setter for subcategoryID.
 * 
 * @param subcategoryID Transaction subcategory ID.
 */
void Transaction::setSubcategoryID(const int &subcategoryID)
{
    m_subcategoryID = subcategoryID;
}

/**
 * @brief Setter for balance.
 * 
 * @param balance Transaction balance.
 */
void Transaction::setBalance(const double &balance)
{
    m_balance = balance;
}

/**
 * @brief Setter for userID.
 * 
 * @param userID Transaction user ID.
 */
void Transaction::setUserID(const int &userID)
{
    m_userID = userID;
}

/**
 * @brief Returns a string representation of the Transaction object.
 * 
 * @return Transaction as string.
 */
QString Transaction::toString() const
{
    QString transactionString;
    transactionString += "TransactionID: " + QString::number(m_transactionID) + " ";
    transactionString += "Amount: " + QString::number(m_amount) + " ";
    transactionString += "Desc: " + m_description + " ";
    transactionString += "Date: " + m_date + " ";
    transactionString += "CategoryID: " + QString::number(m_categoryID) + " ";
    transactionString += "SubcategoryID: " + QString::number(m_subcategoryID) + " ";
    transactionString += "Balance: " + QString::number(m_balance) + " ";
    transactionString += "UserID: " + QString::number(m_userID) + " ";
    transactionString += "isDeposit: " + QString::number(m_isDeposit) + " ";
    return transactionString;
}
