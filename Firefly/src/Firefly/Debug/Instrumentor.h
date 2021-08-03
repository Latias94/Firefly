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
    #define FF_PROFILE_BEGIN_SESSION(name, filepath) ::Firefly::Instrumentor::Get().BeginSession(name, filepath)
    #define FF_PROFILE_END_SESSION() ::Firefly::Instrumentor::Get().EndSession()
// timer+lineNum
    #define FF_PROFILE_SCOPE(name) ::Firefly::InstrumentationTimer TOKENPASTE2(timer, __LINE__)(name)
    #define FF_PROFILE_FUNCTION() FF_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define FF_PROFILE_BEGIN_SESSION(name, filepath)
    #define FF_PROFILE_END_SESSION()
    #define FF_PROFILE_SCOPE(name)
    #define FF_PROFILE_FUNCTION()
#endif