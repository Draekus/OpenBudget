#ifndef USER_H
#define USER_H

#include "position.h"
#include <QString>
#include <map>

/**
 * @brief The User class contains the personal information of a User.
 */
class User
{
public:
    User();
    User(QString firstName, QString lastName, Position position, int userID);
    ~User();

    // Retrieve user's attributes.
    QString firstName() const;
    QString lastName() const;
    Position position() const;
    QString positionName() const;
    int userID() const;

    // Set user's attributes.
    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setPosition(const Position &position);
    void setUserID(const int &userID);

    // Overloaded operators compare users based on their position.
    // Sorts in order User < Developer < Admin.
    bool operator==(const User &other) const;
    bool operator!=(const User &other) const;
    bool operator<(const User &other) const;
    bool operator>(const User &other) const;
    bool operator<=(const User &other) const;
    bool operator>=(const User &other) const;

    QString toString() const;

private:
    QString m_firstName;
    QString m_lastName;
    Position m_position;
    QString m_positionName;
    // Maps Position to QString representation.
    std::map<Position, QString> m_positionNames = {{Position::User, "User"},
                                                   {Position::Developer, "Developer"},
                                                   {Position::Admin, "Admin"}};
    int m_userID;
};

#endif // USER_H
