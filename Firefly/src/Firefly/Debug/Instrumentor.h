#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>

namespace Firefly
{
    struct ProfileResult
    {
        const std::string name;
        long long         start, end;
        uint32_t          threadID;
    };

    class Instrumentor
    {
        std::string   m_SessionName   = "None";
        std::ofstream m_OutputStream;
        int           m_ProfileCount  = 0;
        std::mutex    m_Lock;
        bool          m_ActiveSession = false;

        Instrumentor() {}

    public:

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

        ~Instrumentor()
        {
            EndSession();
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            if (m_ActiveSession)
            { EndSession(); }
            m_ActiveSession = true;
            m_OutputStream.open(filepath);
            WriteHeader();
            m_SessionName = name;
        }

        void EndSession()
        {
            if (!m_ActiveSession)
            { return; }
            m_ActiveSession = false;
            WriteFooter();
            m_OutputStream.close();
            m_ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& result)
        {
            std::lock_guard<std::mutex> lock(m_Lock);

            if (m_ProfileCount++ > 0)
            { m_OutputStream << ","; }

            std::string name = result.name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.end - result.start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.threadID << ",";
            m_OutputStream << "\"ts\":" << result.start;
            m_OutputStream << "}";
        }

        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        }

        void WriteFooter()
        {
            m_OutputStream << "]}";
        }
    };

    class InstrumentationTimer
    {
        ProfileResult m_Result;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool                                                        m_Stopped;

    public:

        InstrumentationTimer(const std::string& name)
                : m_Result({name, 0, 0, 0}), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!m_Stopped)
            { Stop(); }
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            m_Result.start    = std::chrono::time_point_cast<std::chrono::microseconds>(
                    m_StartTimepoint).time_since_epoch().count();
            m_Result.end      = std::chrono::time_point_cast<std::chrono::microseconds>(
                    endTimepoint).time_since_epoch().count();
            m_Result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile(m_Result);

            m_Stopped = true;
        }
    };
}

#define FF_PROFILE 1
#if FF_PROFILE

// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
        #define FF_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
        #define FF_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__FUNCSIG__)
        #define FF_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
        #define FF_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
        #define FF_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
        #define FF_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
        #define FF_FUNC_SIG __func__
    #else
        #define FF_FUNC_SIG "FF_FUNC_SIG unknown!"
    #endif

    #define FF_PROFILE_BEGIN_SESSION(name, filepath) ::Firefly::Instrumentor::Get().BeginSession(name, filepath)
    #define FF_PROFILE_END_SESSION() ::Firefly::Instrumentor::Get().EndSession()
// timer+lineNum
    #define FF_PROFILE_SCOPE(name) ::Firefly::InstrumentationTimer TOKENPASTE2(timer, __LINE__)(name)
    #define FF_PROFILE_FUNCTION() FF_PROFILE_SCOPE(FF_FUNC_SIG)
#else
    #define FF_PROFILE_BEGIN_SESSION(name, filepath)
    #define FF_PROFILE_END_SESSION()
    #define FF_PROFILE_SCOPE(name)
    #define FF_PROFILE_FUNCTION()
#endif