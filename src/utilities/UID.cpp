#include <chrono>
#include <limits>
#include <mutex>
#include <random>

#include <mtt/utilities/Log.h>
#include <mtt/utilities/SpinLock.h>
#include <mtt/utilities/UID.h>

using namespace mtt;

static std::default_random_engine uidRandomEngine(
    unsigned int(std::chrono::system_clock::now().time_since_epoch().count()));
static std::uniform_int_distribution<uint64_t> uidDistribution(
                                          std::numeric_limits<uint64_t>::min(),
                                          std::numeric_limits<uint64_t>::max());
static SpinLock uidGeneratorLock;

uint64_t UID::randomValue()
{
  std::lock_guard<SpinLock> lock(uidGeneratorLock);
  return uidDistribution(uidRandomEngine);
}