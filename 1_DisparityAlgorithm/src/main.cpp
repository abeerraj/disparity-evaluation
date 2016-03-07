#include <iostream>
#include <boost/program_options.hpp>

int main(int argc, const char * argv[]) {
  namespace po = boost::program_options;
  po::options_description desc("Options");
  desc.add_options()
  ("help", "Print help messages")
  ("add", "additional options")
  ("like", "this");

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
  } catch(po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
    std::cerr << desc << std::endl;
    return 1;
  }

  if (vm.count("help")) {
    std::cout << "Basic Command Line Parameter App" << std::endl
    << desc << std::endl;
    return 0;
  }

  // default behaviour
  //executeEvalSuiteWithJson();

  po::notify(vm);
  return 0;
}
