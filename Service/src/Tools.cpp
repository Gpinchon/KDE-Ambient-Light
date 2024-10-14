#include <string>
#include <memory>
#include <array>
#include <stdexcept>
#include <iostream>

std::string exec(const std::string &a_Cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(a_Cmd.c_str(), "r"), &pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    if (result.empty()) return result;
    // remove trailing \n
    return {result.begin(), result.end() - 1};
}

class NullBuffer : public std::streambuf
{
public:
    int overflow(int c) final { return c; }
};

std::ostream &Log()
{
#ifdef _DEBUG
    return std::cout;
#endif
    static NullBuffer null_buffer;
    static std::ostream null_stream(&null_buffer);
    return null_stream;
}

std::ostream &Error()
{
    return std::cerr << "ERROR : ";
}