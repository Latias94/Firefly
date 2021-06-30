#pragma once

#ifdef FF_PLATFORM_WINDOWS

extern Firefly::Application* Firefly::CreateApplication();

int main(int argc, char** argv)
{
	printf("Firefly Engine");
	auto app = Firefly::CreateApplication();
	app->Run();
	delete app;
}
#endif
