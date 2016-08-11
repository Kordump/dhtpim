#ifndef GUARD_DHTPIM_MSG_HPP_
#define GUARD_DHTPIM_MSG_HPP_
#include <string>

#include <opendht.h>

#include "tools.hpp"

// Simple format for our messages.
struct msg : public std::string
{
    msg(std::string username, std::string text)
        : std::string(
            "[" + std::to_string(get_timestamp().count()) + "]"
            " <" + username + "> " + text)
    { };

    operator dht::Value()
    {
        return dht::Value::pack(std::string(*this));
    }
};

#endif
