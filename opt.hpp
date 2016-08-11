#ifndef GUARD_DHTPIM_OPT_HPP
#define GUARD_DHTPIM_OPT_HPP
#include <cstdlib>
#include <memory>

#include <iostream>

#ifndef NDEBUG
    #include <stdexcept>
#endif

#include "cmdline.h"

struct opt
{
private:
    static std::unique_ptr<gengetopt_args_info> args_info;

public:
    opt(int argc, char** argv)
    {
        opt::args_info = std::make_unique<gengetopt_args_info>();
        if(cmdline_parser(argc, argv, opt::args_info.get()))
            exit(1);
        args = args_type();
    }

    struct args_type
    {
        gengetopt_args_info& operator()()
        {
#ifndef NDEBUG
            if(!opt::args_info)
                throw std::logic_error("Wrapper getopt ill-initialized.");
#endif
            return *opt::args_info;
        }
    };

    static args_type args;

    ~opt()
    {
        cmdline_parser_free(opt::args_info.get());
        opt::args_info = nullptr;
    }
};

#endif
