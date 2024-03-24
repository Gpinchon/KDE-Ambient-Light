#include <DBUS.hpp>
#include <Tools.hpp>

#include <dbus/dbus.h>

DBusError GetDBUSError()
{
    DBusError error;
    dbus_error_init(&error);
    return error;
}

auto GetDBUSConnection(const DBusBusType &a_Type)
{
    auto dbus_error = GetDBUSError();
    auto dbus_conn = dbus_bus_get(a_Type, &dbus_error);
    Log() << "Connected to D-Bus as \"" << dbus_bus_get_unique_name(dbus_conn) << "\"." << std::endl;
    return dbus_conn;
}

DBUSConnection::DBUSConnection(const DBusBusType &a_Type)
    : DBUSObject(GetDBUSConnection(a_Type))
{
}

DBUSConnection::~DBUSConnection()
{
    dbus_connection_unref(data);
}

::DBusMessage *DBUSConnection::Send(::DBusMessage *a_Msg) const
{
    auto dbus_error = GetDBUSError();
    auto dbus_msg = dbus_connection_send_with_reply_and_block(
        data,
        a_Msg, DBUS_TIMEOUT_USE_DEFAULT,
        &dbus_error);
    if (dbus_error.message != nullptr)
        throw DBUSException(dbus_error.message);
    return dbus_msg;
}

DBUSMessage::~DBUSMessage()
{
    DBusMessage *dbus_msg = *this;
    dbus_message_unref(dbus_msg);
}

DBUSMethodCall::DBUSMethodCall(const char *bus_name,
                               const char *path,
                               const char *iface,
                               const char *method)
    : DBUSMessage(dbus_message_new_method_call(
          bus_name,
          path,
          iface,
          method))
{
}
