/// \file

#include <boost/format.hpp>
#include <cstdlib>
#include <iostream>

/// Program entry point.
int main(void) {
    std::cout << boost::format("Hello %1%") % "World" << std::endl;

    return EXIT_SUCCESS;
}
