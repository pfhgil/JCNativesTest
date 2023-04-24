#include "Log.h"

#include <chrono>
#include <ctime>
#include <cstdarg>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void Core::Logging::init()
{
    c_printf(MessageType::MT_ERROR, "Log init: this is error message!");
    c_printf(MessageType::MT_WARNING, "Log init: this is warning message!");
    c_printf(MessageType::MT_SUCCESS, "Log init: this is success message!");
    c_printf(MessageType::MT_INFO, "Log init: this is info message!");
}

// TODO: исправить вывод
void Core::Logging::c_printf(const MessageType& message_type, const char* text, ...)
{
    std::string msg_str = std::string { text };

    char buf[64];

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", std::localtime(&time));

    std::string final_string = std::string(buf) + " (len: " + std::to_string(msg_str.length()) + ") | [" + message_type_to_string(message_type, true) + ANSI_COLOR_RESET + "]: " + text + "\n";

    va_list args_ptr;
    va_start(args_ptr, text);
    vprintf(final_string.c_str(),args_ptr);
    va_end(args_ptr);
}

std::string Core::Logging::message_type_to_string(const MessageType& messageType, const bool& add_color)
{
    std::string ret;
    switch(messageType)
    {
        case MessageType::MT_ERROR: return add_color ? (ret + ANSI_COLOR_RED + "MT_ERROR") : "MT_ERROR";
        case MessageType::MT_WARNING: return add_color ? (ret + ANSI_COLOR_YELLOW + "MT_WARNING") : "MT_WARNING";
        case MessageType::MT_SUCCESS: return add_color ? (ret + ANSI_COLOR_GREEN + "MT_SUCCESS") : "MT_SUCCESS";
        case MessageType::MT_INFO: return add_color ? (ret + ANSI_COLOR_MAGENTA + "MT_INFO") : "MT_INFO";
        default: return "MT_UNKNOWN_TYPE";
    }
}
