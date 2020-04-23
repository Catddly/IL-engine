#include <IL.h>

class SandBox : public IL::Application
{
public: 
	SandBox()
	{
	}

	~SandBox()
	{
	}

};

IL::Application* IL::CreateApplication()
{
	return new SandBox;
}
