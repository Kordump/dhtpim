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

void io_init()
{
    // Use GNU readline history.
    using_history();
}

void usage(const char* command_line)
{
    std::cout
        << "Usage : "                                           "\n"
        << " - " << command_line << " <host> <port>"            "\n"
        << "   - <host>: Host of the bootstrap node."           "\n"
        << "   - <port>: Port of the bootstrap node."           "\n"
        << " - " << command_line << " bootstrap.ring.cx 4222"   "\n"
        << std::endl;
}

