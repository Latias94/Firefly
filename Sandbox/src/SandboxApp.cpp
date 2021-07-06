#include <Firefly.h>

class ExampleLayer : public Firefly::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        if (Firefly::Input::IsKeyPressed(FF_KEY_TAB))
            FF_TRACE("Tab key is pressed (poll)!");
    }

    void OnEvent(Firefly::Event& event) override
    {
        if (event.GetEventType() == Firefly::EventType::KeyPressed)
        {
            Firefly::KeyPressedEvent& e = (Firefly::KeyPressedEvent&) event;
            if (e.GetKeyCode() == FF_KEY_TAB)
                FF_TRACE("Tab key is pressed (event)!");
            FF_TRACE("{0}", (char) e.GetKeyCode());
        }
    }
};

class Sandbox : public Firefly::Application
{
public :
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Firefly::ImGuiLayer());
    }

    ~Sandbox()
    {
    }
};

Firefly::Application* Firefly::CreateApplication()
{
    return new Sandbox();
}
