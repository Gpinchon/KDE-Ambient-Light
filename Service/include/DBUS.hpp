#pragma once
#include <any>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <vector>

#include <dbus/dbus.h>
namespace DBUS {
class Exception : public std::exception {
public:
    explicit Exception(const std::string& a_Message)
        : _message("DBUS Exception : " + a_Message)
    {
    }
    const char* what() const noexcept override { return _message.c_str(); };

private:
    const std::string _message;
};

template <typename T>
class Object {
public:
    operator T&() { return data; }
    T data;

protected:
    Object(const T& a_Value = {})
        : data(a_Value)
    {
    }
    virtual ~Object() = default;
};

class Error : public Object<::DBusError> {
public:
    Error();
    ~Error() override;
};

class Connection : public Object<::DBusConnection*> {
public:
    Connection(const DBusBusType& a_Type);
    ~Connection() override;
    [[nodiscard]] ::DBusMessage* Send(::DBusMessage*) const;
    void SendNoReply(::DBusMessage*) const;
};

class Message : public Object<::DBusMessage*> {
public:
    Message(::DBusMessage* a_Msg)
        : Object(a_Msg) {};
    ~Message() override;
};

class MethodCall : public Message {
public:
    MethodCall(const char* bus_name,
        const char* path,
        const char* iface,
        const char* method);
    template <typename... Args>
    void SetArgs(Args... a_Args);
};

inline ::DBusMessageIter GetMessageIter(DBusMessage* a_Message)
{
    DBusMessageIter iter;
    dbus_message_iter_init(a_Message, &iter);
    return iter;
}

inline ::DBusMessageIter GetMessageIterEnd()
{
    ::DBusMessageIter iter;
    std::memset(&iter, 1, sizeof(::DBusMessageIter));
    return iter;
}

class ReplyIterator {
public:
    ReplyIterator(::DBusMessageIter const& a_Iter)
        : iter(a_Iter)
    {
    }
    bool operator==(const ReplyIterator& a_Other) const
    {
        return std::memcmp(&iter, &a_Other.iter, sizeof(::DBusMessageIter)) == 0;
    }
    bool operator!=(const ReplyIterator& a_Other) const
    {
        return !(*this == a_Other);
    }
    auto& operator++()
    {
        if (!dbus_message_iter_next(&iter)) {
            iter = GetMessageIterEnd();
        }
        return *this;
    }
    std::any operator*();
    ::DBusMessageIter iter;
};

class Reply : public Message {
public:
    Reply(::DBusMessage* a_Msg)
        : Message(a_Msg) {};
    auto begin()
    {
        return ReplyIterator(GetMessageIter(*this));
    }
    auto end() const
    {
        return GetMessageIterEnd();
    }
    std::vector<std::any> GetArgs()
    {
        std::vector<std::any> args;
        for (auto arg : *this) {
            args.push_back(arg);
        }
        return args;
    }
};

template <typename... Args>
inline void MethodCall::SetArgs(Args... a_Args)
{
    Error error;
    dbus_message_append_args(data, a_Args..., DBUS_TYPE_INVALID);
    if (error.data.message != nullptr) {
        throw Exception(std::string(error.data.name) + " : " + error.data.message);
    }
}

} // namespace DBUS