#include "ffpch.h"
#include "LayerStack.h"

namespace Firefly
{
    LayerStack::LayerStack()
    {
    }

    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        // m_LayerInsert record the insert position
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    // layer push into first half list
    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.end())
        {
            layer->OnDetach();
            // not deallocate yet, deallocate layer when application shut down
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    // overlay layer push into second half list, cause overlay always render in the last.
    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
        }
    }
}