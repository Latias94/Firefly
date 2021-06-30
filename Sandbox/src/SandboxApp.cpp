#include <Firefly.h>

class Sandbox : public Firefly::Application
{
public :
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

Firefly::Application* Firefly::CreateApplication()
{
	return new Sandbox();
}
