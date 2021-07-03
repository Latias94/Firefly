#pragma once

extern Firefly::Application *Firefly::CreateApplication();

int main(int argc, char **argv)
{
    Firefly::Log::Init();
    FF_CORE_WARN("Initialized Log!");
    int a = 5;
    FF_INFO("Hello! Var={0}", a);

    auto app = Firefly::CreateApplication();
    app->Run();
    delete app;
}
