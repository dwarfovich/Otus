#include <boost/program_options.hpp>

#include <iostream>
int main(int argc, char* argv[])
{
    namespace  po= boost::program_options;
    po::options_description description("Allowed options");
    description.add_options()("help", "produce help message")("compression", po::value<int>(), "set compression level");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << description << "\n";
        return 1;
    }

    if (vm.count("compression")) {
        std::cout << "Compression level was set to " << vm["compression"].as<int>() << ".\n";
    } else {
        std::cout << "Compression level was not set.\n";
    }
    return 0;
}
