#pragma once
#include "PCH.h"
#include "public/Core.h"

namespace GTDLi
{
class WGeneric
{

};

class IGeneric
{

};

template <typename Instance, typename WInstance>
class SingletonInstance
{
	GTD_API static Instance& Instance(void);

	GTD_API virtual ~SingletonInstance() {};
};



}

