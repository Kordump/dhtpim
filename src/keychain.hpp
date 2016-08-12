#ifndef GUARD_DHTPIM_KEYCHAIN_HPP_
#define GUARD_DHTPIM_KEYCHAIN_HPP_
#include <chrono>
#include <string>

#include "tools.hpp"

// A keychain, a « moving » InfoHash key.
template<size_t period = 60000>
struct keychain
{
    keychain(std::string name, std::string salt = "")
        : name("#" + name + "-" + salt)
    { };

    std::string ahead(std::chrono::milliseconds from_time) const
    {
        auto stamp = (get_timestamp() + from_time).count();
        stamp = stamp / period;

        // The chain move to a new InfoHash every period.
        return name + "-" + std::to_string(stamp);
    }

    std::string ahead(int from_time) const
    {
        return ahead(std::chrono::milliseconds(from_time));
    }

    operator std::string() const
    {
        // Referring to « chain » refer to present chain.
        return ahead(0);
    }

private:
    std::string name;
};

#endif
