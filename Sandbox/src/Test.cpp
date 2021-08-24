#define ENTRY_POINT
#include "public\GTDLi.h"

class Sandbox : public GTDLi::Console
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{

	}
};

GTDLi::Console* GTDLi::CreateConsole()
{
	return new Sandbox();
}