#pragma once

extern Firefly::Application *Firefly::CreateApplication();

int main(int argc, char **argv)
{
    Firefly::Log::Init();
    FF_CORE_WARN("Initialized Log!");

    auto app = Firefly::CreateApplication();
    app->Run();
    delete app;
}
