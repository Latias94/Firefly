#include <Firefly.h>
#include <Firefly/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Firefly
{
    class FireflyEditor : public Application
    {
    public :
        FireflyEditor() : Application("Firefly Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~FireflyEditor()
        {
        }
    };

    Firefly::Application* Firefly::CreateApplication()
    {
        return new FireflyEditor();
    }
}
