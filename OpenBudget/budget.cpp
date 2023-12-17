#include "budget.h"

/**
 * @brief Default constructor creates empty Budget object.
 * 
 * @return Budget object.
 */
Budget::Budget()
    : m_categoryID{0}
    , m_subcategoryID{0}
    , m_amount{0.0}
    , m_userID{0}
{}

/**
 * @brief Budget constructor for category and subcategory.
 *
 * @param categoryID The budget's category ID.
 * @param subcategoryID The budget's subcategory ID.
 * @param amount The budget's amount.
 * @param userID The budget's userID.
 * @return Budget object.
 */
Budget::Budget(int categoryID, int subcategoryID, double amount, int userID)
    : m_categoryID{categoryID}
    , m_subcategoryID{subcategoryID}
    , m_amount{amount}
    , m_userID{userID}
{}

/**
 * @brief Getter for categoryID.
 *
 * @return The budget's category ID.
 */
int Budget::categoryID() const
{
    return m_categoryID;
}

/**
 * @brief Getter for subcategoryID.
 *
 * @return The budget's subcategory ID.
 */
int Budget::subcategoryID() const
{
    return m_subcategoryID;
}

/**
 * @brief Getter for amount.
 *
 * @return The budget's amount.
 */
double Budget::amount() const
{
    return m_amount;
}

/**
 * @brief Getter for userID.
 *
 * @return The budget's userID.
 */
int Budget::userID() const
{
    return m_userID;
}

/**
 * @brief Setter for categoryID.
 *
 * @param categoryID The budget's category ID.
 */
void Budget::setCategoryID(const int &categoryID)
{
    m_categoryID = categoryID;
}

/**
 * @brief Setter for subcategoryID.
 *
 * @param subcategoryID The budget's subcategory ID.
 */
void Budget::setSubcategoryID(const int &subcategoryID)
{
    m_subcategoryID = subcategoryID;
}

/**
 * @brief Setter for amount.
 *
 * @param amount The budget's amount.
 */
void Budget::setAmount(const double &amount)
{
    m_amount = amount;
}

/**
 * @brief Setter for userID.
 *
 * @param userID The budget's userID.
 */
void Budget::setUserID(const int &userID)
{
    m_userID = userID;
}

/**
 * @brief Returns a string representation of the Budget object.
 *
 * @return QString representation of Budget object.
 */
QString Budget::toString() const
{
    QString budgetString;
    budgetString += "Category ID: " + QString::number(m_categoryID) + "\n";
    budgetString += "Subcategory ID: " + QString::number(m_subcategoryID) + "\n";
    budgetString += "Amount: " + QString::number(m_amount) + "\n";
    budgetString += "User ID: " + QString::number(m_userID) + "\n";
    return budgetString;
}
