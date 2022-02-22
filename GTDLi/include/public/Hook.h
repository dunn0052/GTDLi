#pragma once
#include "Core.h"
#include "Retcodes.h"
#include <vector>

namespace GTDLi
{
	template<class Func>
	class Hook
	{
	public:
		Hook<Func>() : m_Functions() {}
		~Hook<Func>() {}

		Hook* operator +=(Func&& function)
		{
			m_Functions.push_back(std::forward<Func>(function));
			return this;
		}

		Hook* operator -=(Func function)
		{
			m_Functions.erase(std::forward<Func>(function));
			return this;
		}

		template <typename ... Params>
		RETCODE Invoke(Params&& ... params)
		{
			for (const Func& function : m_Functions)
			{
				function(std::forward<Params>(params) ...);
			}

			return RTN_OK;
		}

	private:

		std::vector<Func> m_Functions;

	};


}
