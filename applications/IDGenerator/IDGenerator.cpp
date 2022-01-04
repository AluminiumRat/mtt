#include <chrono>
#include <limits>
#include <random>
#include <iostream>

int main()
{
  unsigned int seed =
      unsigned int(std::chrono::system_clock::now().time_since_epoch().count());
  std::default_random_engine randomEngine(seed);
  std::uniform_int_distribution<uint64_t> distribution(
                                          std::numeric_limits<uint64_t>::min(),
                                          std::numeric_limits<uint64_t>::max());

  std::cout << distribution(randomEngine) << "ull" << std::endl;
  return 0;
}
