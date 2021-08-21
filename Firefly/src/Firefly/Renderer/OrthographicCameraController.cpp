#include "ffpch.h"
#include "Firefly/Renderer/OrthographicCameraController.h"
#include "Firefly/Core/KeyCodes.h"
#include "Firefly/Core/Input.h"

namespace Firefly
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
            : m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
              m_AspectRatio(aspectRatio), m_Rotation(rotation)
    {

    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        FF_PROFILE_FUNCTION();

        if (Input::IsKeyPressed(FF_KEY_A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(FF_KEY_D))
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;

        if (Input::IsKeyPressed(FF_KEY_S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(FF_KEY_W))
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(FF_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * ts;
            if (Input::IsKeyPressed(FF_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        FF_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(FF_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(FF_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    void OrthographicCameraController::CalculateView()
    {
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    void OrthographicCameraController::OnResize(float width, float height)
    {
        m_AspectRatio = width / height;
        CalculateView();
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        FF_PROFILE_FUNCTION();

        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        CalculateView();
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        FF_PROFILE_FUNCTION();

        OnResize((float) e.GetWidth(), (float) e.GetHeight());
        return false;
    }
}