#include "user.h"

user::user()
{

}

QString user::getName() const
{
    return name;
}

void user::setName(const QString &value)
{
    name = value;
}

QString user::getEmail() const
{
    return email;
}

void user::setEmail(const QString &value)
{
    email = value;
}

QString user::getPassword() const
{
    return password;
}

void user::setPassword(const QString &value)
{
    password = value;
}
