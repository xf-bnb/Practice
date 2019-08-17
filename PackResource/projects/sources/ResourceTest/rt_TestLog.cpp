#include <mutex>
#include <cstdarg>
#include <iostream>

#include "rt_TestLog.h"

template<unsigned int n>
inline bool _FormatLog(char(&buf)[n])
{
    time_t seconds = 0;
    if ((seconds = time(nullptr)) < 0)
        return false;

    tm t;

#ifdef _MSC_VER

    if (localtime_s(&t, &seconds) != 0)
        return false;

#else

    if (localtime_r(&seconds, &t) == nullptr)
        return false;

#endif  // _MSC_VER

    return (0 < std::snprintf(buf, n, "%02d:%02d:%02d.%03u", t.tm_hour, t.tm_min, t.tm_sec, _CurrentMillisecond() % 1000));
}

bool TestLog::_output(const char * extra, const char * text) const
{
    _LogMutex.lock();
    std::cout << extra << "[" << std::this_thread::get_id() << "]-> " << text << std::endl;
    _LogMutex.unlock();

    return true;
}

bool TestLog::operator()(const char * format, ...) const
{
    char extra[32]{ 0 };
    if (_FormatLog(extra))
    {
        va_list args;
        va_start(args, format);

        char strText[_MaxLog]{ 0 };
        bool result = (0 <= vsnprintf(strText, _MaxLog, format, args) && _output(extra, strText));

        va_end(args);

        return result;
    }

    return false;
}
