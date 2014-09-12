External libraries
==================

This repository - collection of third-party c++ libraries.
Libraries complemented with cmake build system.

Usage
-----

You can use this repository as external project of as git submodule.

### Use as cmake external project

For fast start using libraries in your new cmake-based project.
TODO

### Use as submodule

1.  Initialize git submodule

        git submodule add http://github.com/halex2005/external-cmake-based external
        git commit -m "add external libraries as submodule"
        git submodule init
        git submodule update

2. Add support of this libraries to CMakeLists.txt in the root for compiling and use headers

        include_directories(external)
        add_subdirectory(external)

3. Use headers in your source files

        #include <catch/catch.hpp>

4. Fine tune dependencies and link libraries for your targets

        add_executable(your-target ${SOURCES})
        add_dependencies(your-target nanodbc pugixml)
        target_link_libraries(your-target nanodbc pugixml)


Featured libraries
------------------

- [catch](https://github.com/philsquared/Catch) - one header unit-test framework
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp/) - json parser
- [nanodbc](https://github.com/lexicalunit/nanodbc) - c++ wrapper for ODBC
- [pugixml](http://pugixml.org) - one of the fastest xml libraries, has xpath support

Licensing
---------

Because this is collection of different libraries, you should carefully read and agree license of underlying libraries which you going to use.
