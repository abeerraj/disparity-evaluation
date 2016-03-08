#include <iostream>
#include <boost/program_options.hpp>

int main(int argc, const char * argv[]) {
  namespace po = boost::program_options;
  po::options_description desc("Options");
  desc.add_options()
  ("dataset", po::value<std::string>()->required(), "Dataset from which the sequence is selected")
  ("sequence", po::value<std::string>()->required(), "Sequence on which to execute the algorithm")
  ("help", "Print help messages");

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
  } catch(po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
    std::cerr << desc << std::endl;
    return 1;
  }

  if (vm.count("dataset")) {
    std::string dataset = vm["dataset"].as<std::string>();
    std::cout << "Dataset:" << std::endl;
    std::cout << dataset << std::endl;
  }
  if (vm.count("sequence")) {
    std::string sequence = vm["sequence"].as<std::string>();
    std::cout << "Sequence:" << std::endl;
    std::cout << sequence << std::endl;
  }
  if (vm.count("help")) {
    std::cout << "Basic Command Line Parameter App" << std::endl;
    std::cout << desc << std::endl;
    return 0;
  }

  try {
    po::notify(vm);
  } catch(po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
    std::cerr << desc << std::endl;
    return 1;
  }

  // TODO generate all available bitmasks for the given sequence in dataset


  return 0;
}
