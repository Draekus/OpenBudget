#ifndef USERLOGIN_H
#define USERLOGIN_H

#include "accesslevel.h"

#include <QString>
#include <map>

/**
 * @brief The UserLogin class contains the account information of a User.
 */
class UserLogin
{
public:
    UserLogin();
    UserLogin(QString username,
              QString password,
              AccessLevel accessLevel,
              QString email,
              int userID,
              bool newUser = false);
    ~UserLogin();

    // Retrieve user's login details.
    QString username() const;
    QString password() const;
    AccessLevel accessLevel() const;
    QString accessLevelName() const;
    QString email() const;
    int userID() const;

    // Set user's login details.
    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void resetPassword(const QString &password);
    void setAccessLevel(const AccessLevel &accessLevel);
    void setEmail(const QString &email);
    void setUserID(const int &userID);

    // Check if the given password matches the user's password.
    bool checkPassword(const QString &password) const;

    // Overloaded operators compare users based on their access level.
    // Sorts in order READ < READ_WRITE < READ_WRITE_DELETE.
    bool operator==(const UserLogin &other) const;
    bool operator!=(const UserLogin &other) const;
    bool operator<(const UserLogin &other) const;
    bool operator>(const UserLogin &other) const;
    bool operator<=(const UserLogin &other) const;
    bool operator>=(const UserLogin &other) const;

    QString toString() const;

private:
    QString m_username;
    QString m_password;
    AccessLevel m_accessLevel;
    QString m_accessLevelName;
    // Maps AccessLevel to QString representation.
    std::map<AccessLevel, QString> m_accessLevelNames = {{AccessLevel::READ, "Read"},
                                                         {AccessLevel::READ_WRITE, "Read/Write"},
                                                         {AccessLevel::READ_WRITE_DELETE,
                                                          "Read/Write/Delete"}};
    QString m_email;
    int m_userID;

    // Hashes the given password.
    QString hashPassword(const QString &password) const;
    // Generates a random salt.
    QString generateSalt() const;
    // Hashes the given password with the given salt.
    QString hashPassword(const QString &password, const QString &salt) const;
};

#endif // USERLOGIN_H
