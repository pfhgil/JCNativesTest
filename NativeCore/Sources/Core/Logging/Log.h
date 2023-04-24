//
// Created by stuka on 22.04.2023.
//

#pragma once

#ifndef NATIVECORE_LOG_H
#define NATIVECORE_LOG_H

#include <iostream>

//#date "Log.cpp"

namespace Core::Logging
{
    enum MessageType
    {
        MT_ERROR,
        MT_WARNING,
        MT_SUCCESS,
        MT_INFO
    };

    class Settings
    {

    };

    void init();

    void c_printf(const MessageType& message_type, const char* text, ...);

    std::string message_type_to_string(const MessageType& messageType, const bool& add_color);
}


#endif //NATIVECORE_LOG_H
