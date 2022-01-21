#include <mtt/utilities/Log.h>

using namespace mtt;

std::mutex Log::_writeMutex;
std::ostream* Log::_stream = &std::cout;

void Log::setStream(std::ostream& stream)
{
  _stream = &stream;
}
