#pragma once

#include "Firefly/Core/Base.h"

extern Firefly::Application* Firefly::CreateApplication();

int main(int argc, char** argv)
{
    Firefly::Log::Init();

    FF_PROFILE_BEGIN_SESSION("Startup", "FireflyProfile-Startup.json");
    auto app = Firefly::CreateApplication();
    FF_PROFILE_END_SESSION();

    FF_PROFILE_BEGIN_SESSION("Runtime", "FireflyProfile-Runtime.json");
    app->Run();
    FF_PROFILE_END_SESSION();

    FF_PROFILE_BEGIN_SESSION("Shutdown", "FireflyProfile-Shutdown.json");
    delete app;
    FF_PROFILE_END_SESSION();
}
