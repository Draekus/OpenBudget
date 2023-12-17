#include "user.h"

/**
 * @brief Default constructor creates empty User object.
 * @return User object.
 */
User::User()
    : m_firstName{""}
    , m_lastName{""}
    , m_position{0}
    , m_userID{0}

{}

/**
 * @brief Stores the user's account information.
 *
 * @param firstName The user's first name.
 * @param lastName The user's last name.
 * @param position The user's position.
 * @param userID The user's userID.
 * @return User object.
 */
User::User(QString firstName, QString lastName, Position position, int userID)
    : m_firstName{firstName}
    , m_lastName{lastName}
    , m_position{position}
    , m_userID{userID}
{
    m_positionName = m_positionNames[position];
}

/**
 * @brief User destructor.
 */
User::~User()
{
    delete this;
}

/**
 * @brief Getter for firstName.
 *
 * @return The user's first name.
 */
QString User::firstName() const
{
    return m_firstName;
}

/**
 * @brief Getter for lastName.
 *
 * @return The user's last name.
 */
QString User::lastName() const
{
    return m_lastName;
}

/**
 * @brief Getter for position.
 *
 * @return The user's position.
 */
Position User::position() const
{
    return m_position;
}

/**
 * @brief Getter for positionName.
 *
 * @return User's positionName.
 */
QString User::positionName() const
{
    return m_positionName;
}

/**
 * @brief Getter for userID.
 *
 * @return The user's userID.
 */
int User::userID() const
{
    return m_userID;
}

/**
 * @brief Setter for firstName.
 *
 * @param firstName The user's firstName.
 */
void User::setFirstName(const QString &firstName)
{
    m_firstName = firstName;
}

/**
 * @brief Setter for lastName.
 *
 * @param lastName The user's lastName.
 */
void User::setLastName(const QString &lastName)
{
    m_lastName = lastName;
}

/**
 * @brief Setter for position.
 *
 * @param position The user's position.
 */
void User::setPosition(const Position &position)
{
    m_position = position;
}

/**
 * @brief Setter for userID.
 *
 * @param userID The user's userID.
 */
void User::setUserID(const int &userID)
{
    m_userID = userID;
}

/**
 * @brief Overloaded == operator for User objects.
 *
 * @param other User object to compare to.
 * @return true if User objects are equal, false otherwise.
 */
bool User::operator==(const User &other) const
{
    return (m_position == other.m_position);
}

/**
 * @brief Overloaded != operator for User objects.
 *
 * @param other User object to compare to.
 * @return true if User objects are not equal, false otherwise.
 */
bool User::operator!=(const User &other) const
{
    return !(m_position == other.m_position);
}

/**
 * @brief Overloaded < operator for User objects.
 *
 * @param other User object to compare to.
 * @return true if this User object is less than other User object, false otherwise.
 */
bool User::operator<(const User &other) const
{
    return (m_position < other.m_position);
}

/**
 * @brief Overloaded > operator for User objects.
 *
 * @param other User object to compare to.
 * @return true if this User object is greater than other User object, false otherwise.
 */
bool User::operator>(const User &other) const
{
    return (m_position > other.m_position);
}

/**
 * @brief Overloaded <= operator for User objects.
 *
 * @param other User object to compare to.
 * @return true if this User object is less than or equal to other User object, false otherwise.
 */
bool User::operator<=(const User &other) const
{
    return (m_position <= other.m_position);
}

/**
 * @brief Overloaded >= operator for User objects.
 *
 * @param other User object to compare to.
 * @return true if this User object is greater than or equal to other User object, false otherwise.
 */
bool User::operator>=(const User &other) const
{
    return (m_position >= other.m_position);
}

/**
 * @brief Represents User object as a string.
 * @return QString representation of User object.
 */
QString User::toString() const
{
    return QString("User: %1 %2 %3 %4")
        .arg(m_firstName)
        .arg(m_lastName)
        .arg(m_positionName)
        .arg(m_userID);
}
