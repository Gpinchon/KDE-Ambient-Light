#pragma once
#include <cstdint>
#include <utility>
#include <stdexcept>

#include <dbus/dbus.h>

template <typename T>
class DBUSObject
{
public:
    operator T &() { return data; }
    T data;

protected:
    DBUSObject(const T &a_Value = {}) : data(a_Value) {}
};

class DBUSError : public DBUSObject<::DBusError>
{
public:
    DBUSError()
    {
        DBusError &dbus_error = *this;
        dbus_error_init(&dbus_error);
    }
};

class DBUSConnection : public DBUSObject<::DBusConnection *>
{
public:
    DBUSConnection(const DBusBusType &a_Type);
    ~DBUSConnection();
    ::DBusMessage *Send(::DBusMessage *) const;
};

class DBUSMessage : public DBUSObject<::DBusMessage *>
{
public:
    DBUSMessage(::DBusMessage *a_Msg) : DBUSObject(a_Msg){};
    ~DBUSMessage();
};

class DBUSMethodCall : public DBUSMessage
{
public:
    DBUSMethodCall(const char *bus_name,
                   const char *path,
                   const char *iface,
                   const char *method);
    template <typename... Args>
    void SetArgs(Args... a_Args);
};

class DBUSReply : public DBUSMessage
{
public:
    DBUSReply(::DBusMessage *a_Msg) : DBUSMessage(a_Msg){};
    template <typename... Args>
    void GetArgs(Args... a_Args);
};

template <typename... Args>
inline void DBUSReply::GetArgs(Args... a_Args)
{
    DBUSError error;
    dbus_message_get_args(data, &error.data, a_Args..., DBUS_TYPE_INVALID);
    if (error.data.message != nullptr)
    {
        throw std::runtime_error(std::string(error.data.name) + " : " + error.data.message);
    }
}

template <typename... Args>
inline void DBUSMethodCall::SetArgs(Args... a_Args)
{
    DBUSError error;
    dbus_message_append_args(data, a_Args..., DBUS_TYPE_INVALID);
    if (error.data.message != nullptr)
    {
        throw std::runtime_error(std::string(error.data.name) + " : " + error.data.message);
    }
}
