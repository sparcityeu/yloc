#pragma once

#ifndef YLOC_CLI_H
#define YLOC_CLI_H

#endif // YLOC_CLI_H

#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_utility.hpp> // print_graph
#include <boost/graph/graphviz.hpp>      // write_graphviz
#include <cstdio>
#include <chrono>
#include <ctime>
#include <getopt.h>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <yloc/modules/module.h>
#include <yloc/modules/adapter.h>

#include "yloc/yloc.h"

/**
 * The Default behaviour of getopt_long is, that a optional argument must follow the option-Character directly (e.g. -d8000).
 * From a user perspective optional arguments should be treated like required arguments (e.g. -d 8000 should be possible).
 * In order to achieve such behaviour we must define a helper macro.
 * Compare: https://cfengine.com/blog/2021/optional-arguments-with-getopt-long/
 * Code taken from https://github.com/NorthernTechHQ/libntech
 */
#define OPTIONAL_ARGUMENT_IS_PRESENT \
    ((optarg == NULL && optind < argc && argv[optind][0] != '-') \
     ? (bool) (optarg = argv[optind++]) \
     : (optarg != NULL))

using namespace yloc;

static struct option long_options[] =
    {
        {"help", no_argument, nullptr, 'h'},
        {"list-modules", no_argument, nullptr, 'M'},
        {"list-component-types", no_argument, nullptr, 'C'},
        {"list-properties-per-component", no_argument, nullptr, 'P'},
        {"list-output-formats", no_argument, nullptr, 'O'},
        {"filter-component-types", required_argument, nullptr, 'c'},
        {"filter-hardware-properties", required_argument, nullptr, 'p'},
        {"dynamic-probing-period", required_argument, nullptr, 'd'},
        {"dynamic-probing-frequency", required_argument, nullptr, 'q'},
        {"output", required_argument, nullptr, 'o'},
        {"output-format", required_argument, nullptr, 'f'},
        {nullptr, 0, nullptr, 0}
};

template < class Name > class csv_writer
{
public:
    explicit csv_writer(Name _name) : name(_name) {}
    template < class VertexOrEdge >
    void operator()(std::ostream& out, const VertexOrEdge& v) const
    {
        out << (name, v);
    }

private:
    Name name;
};

// Default Constants
const std::array<std::string, 2> VALID_OUTPUT_FORMATS = {"dot", "csv"};
const int DEFAULT_PROBING_FREQUENCY = 1000; // in milliseconds [ms]

/**
 * compare: https://stackoverflow.com/a/35157784
 * @return
 */
std::string date_time_in_milliseconds()
{
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%F %T"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}

void show_help()
{
    std::cout << "Usage: yloc-cli [OPTION]...\n"
           "Get information about the hardware topology of your system.\n"
           "Output can be formatted as dot for graph representation,\n"
           "or as csv for tabular representation.\n"
           "If no additional OPTION is given, the command will print\n"
           "a dot-format graph representation of the host machine to stdout.\n"
           "\n"
           "Mandatory arguments to long options are mandatory for short options too.\n"
           "\n"
           "  -M, --list-modules        list available modules and exit;\n"
           "                            modules can provide additional hardware information,\n"
           "                            see <https://github.com/sparcityeu/yloc#yloc-modules>\n"
           "  -C, --list-component-types\n"
           "                            list available component types and exit;\n"
           "                            based on the abstract machine model at ylocs core;\n"
           "                            listed component types can be used to filter\n"
           "                            specific information, see option -c\n"
           "  -P, --list-hardware-properties\n"
           "                            list hardware properties and exit;\n"
           "                            based on the abstract machine model at ylocs core;\n"
           "                            listed properties can be used to filter\n"
           "                            specific information, see option -p\n"
           "  -F, --list-output-formats\n"
           "                            list available formats and exit;\n"
           "                            listed output-formats can be used as\n"
           "                            file extension for option -o\n"
           "                            and also as argument for option -f\n"
           "\n"
           "  -c, --filter-component-types=COMPONENT1,COMPONENT2\n"
           "                            only include listed component-types in resulting output\n"
           "  -p, --filter-hardware-properties=PROPERTY1,PROPERTY2\n"
           "                            only include listed hardware properties in resulting output\n"
           "  -d, --dynamic-probing-period=PROBING-PERIOD\n"
           "                            periodically probe hardware to see dynamic changes;\n"
           "                            process will keep running until the given PROBING-PERIOD is\n"
           "                            reached, the PROBING-PERIOD should be in milliseconds;\n"
           "                            the default probing-frequency is 1000 ms,\n"
           "                            setting the PROBING-PERIOD to -1, triggers an endless run;\n"
           "                            a custom probing-frequency can be set with option -q;\n"
           "                            if an output-file is given and format is dot, the output-file\n"
           "                            will be overwritten for each sampling point\n"
           "                            (to enable live observation of a graph),\n"
           "                            csv output will be appended (to enable further analysis)\n"
           "  -q, --dynamic-probing-frequency=PROBING-FREQUENCY\n"
           "                            sets a custom PROBING-FREQUENCY for the probing period;\n"
           "                            PROBING-FREQUENCY must be in milliseconds (default 1000 ms);\n"
           "                            only has an effect when used together with option -d;\n"
           "\n"
           "  -o, --output=FILE.EXTENSION\n"
           "                            specify output file;\n"
           "                            the FILE can have any or even no EXTENSION;\n"
           "                            if -o is used without option -f, and the FILE has an\n"
           "                            EXTENSION that is listed within the supported output formats\n"
           "                            (option -F), the EXTENSION will also determine the output-format;\n"
           "                            if FILE has no supported EXTENSION, format will default to dot\n"
           "  -f, --output-format=FORMAT\n"
           "                            specify output format;\n"
           "                            see list of supported output formats with option -F;\n"
           "                            if option -o and -f are used together,\n"
           "                            -f will determine format regardless of any file-extension\n"
           "  -h, --help        display this help and exit\n"
           "\n"
           "Examples\n"
           "  Get overview of hardware topology\n"
           "                            yloc-cli\n"
           "  Pipe output to GrahpViz to get an image\n"
           "                            yloc-cli | dot -Tpng > full_graph.png\n"
           "  Pipe output to GrahpViz to open as image in a new window\n"
           "                            yloc-cli | dot -Tx11\n"
           "  Filter specific information\n"
           "                            yloc-cli -c Core,Cache -p numa_affinity,memory\n"
           "  Monitor hardware properties\n"
           "                            yloc-cli -d 4000 -q 1000 -f csv\n"
           "  Start an endless run\n"
           "                            yloc-cli -d -1 -f csv -o hardware_information.txt\n"
           "Further information: <https://github.com/sparcityeu/yloc>\n";
}

void show_output_formats()
{
    std::cout << "Valid output formats to use as argument for option -f.\n"
           "Can also be used as extension for the output-file specified with option -o.\n"
           "See options -o and -f  in `yloc-cli --help`.\n"
           "\n"
           "  dot               graph description language of the Graphviz project\n"
           "                    (see <https://en.wikipedia.org/wiki/Graphviz>),\n"
           "                    well suited for further visualisation\n"
           "  csv               comma separated values, stores tabular data as plain text;\n"
           "                    well suited for further analysis with spreadsheet applications, databases\n"
           "                    or Python and R\n"
           "\n"
           "not yet implemented (contact authors of GitHub-Repo if needed):\n"
           "  json              JavaScript Object Notation, stores data as key-value pairs and arrays;\n"
           "                    well suited for data interchange between different programming languages\n";
}

void print_file_written(const std::string& file_name)
{
    std::cout << "File [" <<  file_name.c_str() << "] written.\n";
}