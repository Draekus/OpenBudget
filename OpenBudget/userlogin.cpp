#include "userlogin.h"
#include <QCryptographicHash>

/**
 * @brief Defaul constructor creates empty UserLogin object.
 * 
 * @return UserLogin object.
 */
UserLogin::UserLogin()
    : m_username{""}
    , m_password{""}
    , m_accessLevel{0}
    , m_accessLevelName{""}
    , m_email{""}
    , m_userID{0}

{}

/**
 * @brief Stores the user's account login information.
 *
 * @param username The user's username.
 * @param password The user's password.
 * @param accessLevel The user's access level.
 * @param email The user's email address.
 * @param userID The user's userID.
 * @param newUser Whether or not the user is a new user.
 * @return UserLogin object.
 */
UserLogin::UserLogin(QString username,
                     QString password,
                     AccessLevel accessLevel,
                     QString email,
                     int userID,
                     bool newUser)
    : m_username{username}
    , m_accessLevel{accessLevel}
    , m_email{email}
    , m_userID{userID}
{
    // If a new user is being created, hash their password.
    m_password = newUser ? hashPassword(password) : password;
    // Convert access level to string.
    m_accessLevelName = m_accessLevelNames[accessLevel];
}

/**
 * @brief UserLogin destructor.
 */
UserLogin::~UserLogin()
{
    delete this;
}

/**
 * @brief Getter for username.
 *
 * @return The user's username.
 */
QString UserLogin::username() const
{
    return m_username;
}

/**
 * @brief Getter for password.
 *
 * @return The user's password.
 */
QString UserLogin::password() const
{
    return m_password;
}

/**
 * @brief Getter for accessLevel.
 *
 * @return The user's access level.
 */
AccessLevel UserLogin::accessLevel() const
{
    return m_accessLevel;
}

/**
 * @brief Getter for accessLevelName.
 *
 * @return The user's access level name.
 */
QString UserLogin::accessLevelName() const
{
    return m_accessLevelName;
}

/**
 * @brief Getter for email.
 *
 * @return The user's email.
 */
QString UserLogin::email() const
{
    return m_email;
}

/**
 * @brief Getter for userID.
 *
 * @return The user's userID.
 */
int UserLogin::userID() const
{
    return m_userID;
}

/**
 * @brief Setter for username.
 *
 * @param username The user's username.
 */
void UserLogin::setUsername(const QString &username)
{
    m_username = username;
}

/**
 * @brief Setter for password.
 *
 * @param password The user's password.
 */
void UserLogin::setPassword(const QString &password)
{
    m_password = hashPassword(password);
}

/**
 * @brief Resets the user's password.
 *
 * @param password The user's new password.
 */
void UserLogin::resetPassword(const QString &password)
{
    m_password = password;
}

/**
 * @brief Setter for accessLevel.
 *
 * @param accessLevel The user's access level.
 */
void UserLogin::setAccessLevel(const AccessLevel &accessLevel)
{
    m_accessLevel = accessLevel;
}

/**
 * @brief Setter for email.
 *
 * @param email The user's email.
 */
void UserLogin::setEmail(const QString &email)
{
    m_email = email;
}

/**
 * @brief Setter for userID.
 *
 * @param userID The user's userID.
 */
void UserLogin::setUserID(const int &userID)
{
    m_userID = userID;
}

/**
 * @brief Checks if given password matches password of UserLogin object.
 *
 * @param password The password to verify.
 * @return true if password matches, false otherwise.
 */
bool UserLogin::checkPassword(const QString &password) const
{
    return (m_password == hashPassword(password));
}

/**
 * @brief Overloaded == operator for UserLogin objects.
 *
 * @param other UserLogin object to compare to.
 * @return true if UserLogin objects are equal, false otherwise.
 */
bool UserLogin::operator==(const UserLogin &other) const
{
    return (m_accessLevel == other.m_accessLevel);
}

/**
 * @brief Overloaded != operator for UserLogin objects.
 *
 * @param other UserLogin object to compare to.
 * @return true if UserLogin objects are not equal, false otherwise.
 */
bool UserLogin::operator!=(const UserLogin &other) const
{
    return !(m_accessLevel == other.m_accessLevel);
}

/**
 * @brief Overloaded < operator for UserLogin objects.
 *
 * @param other UserLogin object to compare to.
 * @return true if this UserLogin object is less than other UserLogin object, false otherwise.
 */
bool UserLogin::operator<(const UserLogin &other) const
{
    return (m_accessLevel < other.m_accessLevel);
}

/**
 * @brief Overloaded > operator for UserLogin objects.
 *
 * @param other UserLogin object to compare to.
 * @return true if this UserLogin object is greater than other UserLogin object, false otherwise.
 */
bool UserLogin::operator>(const UserLogin &other) const
{
    return (m_accessLevel > other.m_accessLevel);
}

/**
 * @brief Overloaded <= operator for UserLogin objects.
 *
 * @param other UserLogin object to compare to.
 * @return true if this UserLogin object is less than or equal to other UserLogin object, false otherwise.
 */
bool UserLogin::operator<=(const UserLogin &other) const
{
    return (m_accessLevel <= other.m_accessLevel);
}

/**
 * @brief Overloaded >= operator for UserLogin objects.
 *
 * @param other UserLogin object to compare to.
 * @return true if this UserLogin object is greater than or equal to other UserLogin object, false otherwise.
 */
bool UserLogin::operator>=(const UserLogin &other) const
{
    return (m_accessLevel >= other.m_accessLevel);
}

/**
 * @brief Converts UserLogin object to QString.
 *
 * @return The user's login information.
 */
QString UserLogin::toString() const
{
    return QString("UserLogin: %1 %2 %3 %4 %5")
        .arg(m_username, m_password, m_accessLevelName, m_email)
        .arg(m_userID);
}

/**
 * @brief Hashes and salts password using MD5 hashing algorithm.
 * 
 * @param password The password to hash.
 * @return The hashed password.
 */
QString UserLogin::hashPassword(const QString &password) const
{
    // Generate salt and append to password.
    QString salt = generateSalt();
    QString saltedPassword = password + salt;

    // Hash salted password using MD5 hashing algorithm.
    QString hashedPassword
        = QCryptographicHash::hash(saltedPassword.toUtf8(), QCryptographicHash::Md5).toHex();

    return hashedPassword;
}

/**
 * @brief Generates salt for password hashing.
 *
 * @return The generated salt.
 */
QString UserLogin::generateSalt() const
{
    // Generate salt by adding 32 to userID.
    QString salt = QString::number(m_userID + 32);

    return salt;
}
