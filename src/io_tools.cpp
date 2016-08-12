#include "io_tools.hpp"

void disp(std::string content, std::ostream& stream)
{
        stream << content << std::endl;
}

void verbose(std::string content)
{
    if(opt::args().verbose_flag)
        disp(content, std::cerr);
}

std::string input(const std::string& prompt)
{
    const char* line = readline(prompt.c_str());
    if(line && *line)
        add_history(line);

    if(line)
    {
        if(opt::args().interactive_flag)
            std::cout << "\x1b[A\x1b[2K\r";

        return std::string(line);
    } else {
        return std::string("/quit");
    }
}

void io_init()
{
    // Use GNU readline history.
    using_history();
}

