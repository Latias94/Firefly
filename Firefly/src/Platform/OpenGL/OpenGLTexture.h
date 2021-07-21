#pragma once

#include "Firefly/Renderer/Texture.h"

namespace Firefly
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        uint32_t GetWidth() const override { return m_Width; }

        uint32_t GetHeight() const override { return m_Height; }

        virtual void Bind(uint32_t slot = 0) const override;
    private:
        std::string m_Path; // for hot reload maybe
        uint32_t    m_Width, m_Height;
        uint32_t    m_RendererID;
    };
}