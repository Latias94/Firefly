#include "ffpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <filesystem>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Firefly
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
        FF_CORE_ASSERT(false, "Unknown shader type '{0}'", type);
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        std::string source        = ReadFile(filepath);
        auto        shaderSources = PreProcess(source);
        Compile(shaderSources);

        // Extract name from filepath
        // assets/shaders/Texture.glsl -> Texture
        std::filesystem::path path = filepath;
        m_Name = path.stem().string();
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
            : m_Name(name)
    {
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER]   = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(sources);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        std::string   result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        } else
        {
            FF_CORE_ERROR("Could not open file '{0}'", filepath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;
        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos             = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            FF_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t      begin = pos + typeTokenLength + 1;
            std::string type  = source.substr(begin, eol - begin);
            FF_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] =
                    source.substr(nextLinePos,
                                  pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
        }
        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program                        = glCreateProgram();
        FF_CORE_ASSERT(shaderSources.size() <= 2, "We only support two shaders for now");
        std::array<GLuint, 2> glShaderIDs{};
        int                   glShaderIDIndex = 0;
        for (auto const&[type, source]: shaderSources)
        {
            // Create an empty shader handle
            GLuint shader = glCreateShader(type);

            // Send the vertex shader source code to GL
            // Note that std::string's .c_str is NULL character terminated.
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            // Compile the vertex shader
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);

                // Use the infoLog as you see fit.
                FF_CORE_ERROR("{0}", infoLog.data());
                FF_CORE_ASSERT(false, "{0}{1}", type, "Shader compilation failure!");
                break;
            }
            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*) &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);

            for (auto id : glShaderIDs)
            {
                // Don't leak shaders either.
                glDeleteShader(id);
            }

            // Use the infoLog as you see fit.
            FF_CORE_ERROR("{0}", infoLog.data());
            FF_CORE_ASSERT(false, "{0}", "Shader link failure!");
            // In this simple program, we'll just leave
            return;
        }

        for (auto id : glShaderIDs)
        {
            // Always detach shaders after a successful link.
            glDetachShader(program, id);
        }

        m_RendererID = program;
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        UploadUniformInt(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        UploadUniformFloat4(name, value);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        UploadUniformMat4(name, value);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, values);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, values);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }


    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}