#pragma once

#include <iostream>
#include <sstream>

class SilentOutput
{
    std::streambuf* orig_cout;
    std::streambuf* orig_cerr;
    std::ostringstream null;

public:
    SilentOutput()
    {
        orig_cout = std::cout.rdbuf(null.rdbuf());
        orig_cerr = std::cerr.rdbuf(null.rdbuf());
    }

    ~SilentOutput() {
        std::cout.rdbuf(orig_cout);
        std::cerr.rdbuf(orig_cerr);
    }
};
