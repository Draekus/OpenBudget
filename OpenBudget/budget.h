#ifndef BUDGET_H
#define BUDGET_H

#include <QString>

class Budget
{
public:
    Budget();
    Budget(int categoryID, int subcategoryID, double amount, int userID);

    // Retrieve budget details.
    int categoryID() const;
    int subcategoryID() const;
    double amount() const;
    int userID() const;

    // Set budget details.
    void setCategoryID(const int &categoryID);
    void setSubcategoryID(const int &subcategoryID);
    void setAmount(const double &amount);
    void setUserID(const int &userID);

    // To string.
    QString toString() const;

private:
    int m_categoryID;
    int m_subcategoryID;
    double m_amount;
    int m_userID;
};

#endif // BUDGET_H
