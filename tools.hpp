#ifndef GUARD_DHTPIM_TOOLS_HPP_
#define GUARD_DHTPIM_TOOLS_HPP_
#include <chrono>
#include <unordered_map>

#include <opendht/dhtrunner.h>

#include "cli_tools.hpp"

// Map type used for message deduplication.
using map_type = std::unordered_map<std::string, std::chrono::milliseconds>;

// Get timestamp in milliseconds.
std::chrono::milliseconds get_timestamp();

// Start listenning to statefull keychain.
size_t listen(dht::DhtRunner& node, std::string chain, map_type& map);

#endif
