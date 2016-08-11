#include "cli_tools.hpp"

void disp(std::string content)
{
    std::cout << "\r\n"
    "\x1b[A\x1b[A " << content << "\n"
    "\x1b[2K" << std::endl;
}

std::string input(const std::string& prompt)
{
    const char* line = readline(prompt.c_str());
    if(line && *line)
        add_history(line);

    if(line)
        return std::string(line);
    return std::string("");
}

