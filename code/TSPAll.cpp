#include "MSTapprox.cpp"
#include "SimulatedAnnealing.cpp"
#include "2optLS.cpp"
#include "FarthestInsertion.cpp"
#include "BnB.cpp"
#include <cstring>

int main(int argc, const char * argv[]) {
  std::string input, algorithm;
  int cutoff_time, seed;
  for (int i = 1; i < argc; i=i+2) { 
    if (i + 1 != argc) {
      if (!strncmp(argv[i],"-inst", 5)) { // Check for command line arguments
        input = argv[i + 1];
      }
      else if (!strncmp(argv[i],"-alg",4)) {
        algorithm = argv[i + 1];
      }
      else if (!strncmp(argv[i],"-time",5)) {
        cutoff_time = atoi(argv[i + 1]);
      }
      else if (!strncmp(argv[i],"-seed",5)) {
        seed = atoi(argv[i + 1]);
      }
      else {
        std::cout << "Invalid arguments.\n";
        exit(0);
      }
    }
  }
  if (algorithm == "BnB") {
    BnB(input,cutoff_time);
  }
  else if (algorithm == "MSTApprox") {
    MSTapprox(input, std::to_string(static_cast<long long>(cutoff_time)), std::to_string(static_cast<long long>(seed)));
  }
  else if (algorithm == "Heur") {
    FarthestInsertion(input);
  }
  else if (algorithm == "LS1") {
    wrappedSimulatedAnnealing(input, algorithm, cutoff_time, seed);
  }
  else if (algorithm == "LS2") {
    wrappedOptTour(input, algorithm, cutoff_time, seed);
  }
  return 0;
}
