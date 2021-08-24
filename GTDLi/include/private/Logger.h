#pragma once
#include "public/Core.h"
#ifdef GTD_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#if GTD_DEBUG_MODE
    #define LOG_TEMPLATE( MESSAGE, LEVEL,  ... ) GTDLi::Logger::Instance().Log(GTDLi::LogLevel::LEVEL, #LEVEL , __FILE__, __LINE__, MESSAGE, __VA_ARGS__ )
#else
    #define LOG_TEMPLATE( MESSAGE, LEVEL, TYPE, ... )
#endif


#define LOG_DEBUG( MESSAGE, ... ) LOG_TEMPLATE( MESSAGE, DEBUG, __VA_ARGS__ ) 	
#define LOG_INFO( MESSAGE, ... ) LOG_TEMPLATE( MESSAGE, INFO, __VA_ARGS__ ) 	
#define LOG_WARN( MESSAGE, ... ) LOG_TEMPLATE( MESSAGE, WARN, __VA_ARGS__ ) 	
#define LOG_FATAL( MESSAGE, ... ) LOG_TEMPLATE( MESSAGE, FATAL, __VA_ARGS__ )

#if GTD_ENABLE_ASSERTS
    #define GTD_ASSERT( PREDICATE, ... ) { if(!( PREDICATE )) { LOG_FATAL("Assertion Failed: %s", __VA_ARGS__); __debugbreak(); }}
#endif

constexpr auto LOGGING_FORMAT = "[%s][%s : %i] ";

namespace GTDLi
{
    enum class GTD_API LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        FATAL,
        NONE
    };

    class Logger
    {
    public:
        GTD_API static Logger& Instance(void);

        GTD_API void Log(LogLevel level, const char* debugLevel, const char* fileName, int lineNum, const char* format, ...);

    private:
        Logger() {}
        ~Logger() {/* should clean up any file streams here*/ };
        Logger(Logger const&) = delete;
        void operator = (Logger const&) = delete;
        LogLevel mLogLevel = LogLevel::NONE;

        // colors
        static const WORD COLOR_BLUE;
        static const WORD COLOR_GREEN;
        static const WORD COLOR_YELLOW;
        static const WORD COLOR_RED;
        // pointer to cmdline
        static const HANDLE console;

        // thread guard
        //std::mutex mMutex;
    };
}