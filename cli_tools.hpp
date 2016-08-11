#ifndef GUARD_DHTPIM_CLI_TOOLS_HPP
#define GUARD_DHTPIM_CLI_TOOLS_HPP
#include <iostream>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>

// Nice display with VT100 terminal control escape sequences.
void disp(std::string content);

// Use GNU readline.
std::string input(const std::string& prompt);

#endif
