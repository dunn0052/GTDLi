#pragma once

#include "public/Console.h"
#include "private/Logger.h"
#ifdef ENTRY_POINT

	#ifdef GTD_PLATFORM_WINDOWS

		extern GTDLi::Console* GTDLi::CreateConsole();

		int main(int argc, char** argv)
		{
			LOG_INFO("~-** Starting GTD Engine **-~");
			auto console = GTDLi::CreateConsole();
			if (GTDLi::RTN_OK != console->Run())
			{
				LOG_FATAL("Failed to run console!");
			}
			delete console;
		}

	#else
		#error Implement other OS please
	#endif

#endif