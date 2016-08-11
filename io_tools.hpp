#ifndef GUARD_DHTPIM_CLI_TOOLS_HPP
#define GUARD_DHTPIM_CLI_TOOLS_HPP
#include <iostream>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>

#include "opt.hpp"

// Output interactive or unformatted output on stream.
void disp(std::string content, std::ostream& stream = std::cout);

// Verbose output.
void verbose(std::string content);

// Use GNU readline.
std::string input(const std::string& prompt = "");

// Init GNU readline.
void io_init();

#endif
