#include <DBUS.hpp>
#include <Tools.hpp>

#include <dbus/dbus.h>

namespace DBUS {
auto GetDBUSConnection(const DBusBusType& a_Type)
{
    Error error;
    auto dbus_conn = dbus_bus_get(a_Type, &error.data);
    Log() << "Connected to D-Bus as \"" << dbus_bus_get_unique_name(dbus_conn) << "\"." << std::endl;
    return dbus_conn;
}

Error::Error()
{
    DBusError& dbus_error = *this;
    dbus_error_init(&dbus_error);
}

Error::~Error()
{
    DBusError& dbus_error = *this;
    dbus_error_free(&dbus_error);
}

Connection::Connection(const DBusBusType& a_Type)
    : Object(GetDBUSConnection(a_Type))
{
}

Connection::~Connection()
{
    dbus_connection_unref(data);
}

::DBusMessage* Connection::Send(::DBusMessage* a_Msg) const
{
    Error error;
    auto dbus_msg = dbus_connection_send_with_reply_and_block(
        data,
        a_Msg, DBUS_TIMEOUT_USE_DEFAULT,
        &error.data);
    if (error.data.message != nullptr) {
        ::Error() << error.data.message << std::endl;
        throw Exception(error.data.message);
    }
    return dbus_msg;
}

void Connection::SendNoReply(::DBusMessage* a_Msg) const
{
    Message { Send(a_Msg) };
}

Message::~Message()
{
    DBusMessage* dbus_msg = *this;
    dbus_message_unref(dbus_msg);
}

MethodCall::MethodCall(const char* bus_name,
    const char* path,
    const char* iface,
    const char* method)
    : Message(dbus_message_new_method_call(
          bus_name,
          path,
          iface,
          method))
{
}

std::any ReplyIterator::operator*()
{
    std::any ret;
    DBusBasicValue val;
    auto type = dbus_message_iter_get_arg_type(&iter);
    if (type == DBUS_TYPE_VARIANT) {
        ::DBusMessageIter subIter;
        dbus_message_iter_recurse(&iter, &subIter);
        return *ReplyIterator(subIter);
    }
    dbus_message_iter_get_basic(&iter, &val);
    switch (type) {
    case DBUS_TYPE_BYTE:
        ret = std::byte(val.byt);
        break;
    case DBUS_TYPE_BOOLEAN:
        ret = bool(val.bool_val);
        break;
    case DBUS_TYPE_INT16:
        ret = val.i16;
        break;
    case DBUS_TYPE_UINT16:
        ret = val.u16;
        break;
    case DBUS_TYPE_INT32:
        ret = val.i32;
        break;
    case DBUS_TYPE_UINT32:
        ret = val.u32;
        break;
    case DBUS_TYPE_INT64:
        ret = val.i64;
        break;
    case DBUS_TYPE_UINT64:
        ret = val.u64;
        break;
    case DBUS_TYPE_DOUBLE:
        ret = val.dbl;
        break;
    case DBUS_TYPE_STRING:
        ret = std::string(val.str);
        break;
    case DBUS_TYPE_OBJECT_PATH:
        ret = std::string(val.str);
        break;
    case DBUS_TYPE_SIGNATURE:
        ret = std::string(val.str);
        break;
    case DBUS_TYPE_UNIX_FD:
        ret = val.fd;
        break;
    default:
        throw DBUS::Exception("Unknown arg type");
    }
    return ret;
}

} // namespace DBUS