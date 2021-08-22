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
        std::string     Name;
        long long       Start, End;
        std::thread::id ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        std::mutex m_Mutex;
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
    public:
        Instrumentor()
                : m_CurrentSession(nullptr)
        {
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession)
            {
                // If there is already a current session, then close it before beginning new one.
                // Subsequent profiling output meant for the original session will end up in the
                // newly opened session instead.  That's better than having badly formatted
                // profiling output.
                if (Log::GetCoreLogger())
                { // Edge case: BeginSession() might be before Log::Init()
                    FF_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name,
                                  m_CurrentSession->Name);
                }
                InternalEndSession();
            }
            m_OutputStream.open(filepath);
            if (m_OutputStream.is_open())
            {
                m_CurrentSession = new InstrumentationSession({name});
                WriteHeader();
            } else
            {
                if (Log::GetCoreLogger())
                { // Edge case: BeginSession() might be before Log::Init()
                    FF_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
                }
            }
        }

        void EndSession()
        {
            std::lock_guard lock(m_Mutex);
            InternalEndSession();
        }

        void WriteProfile(const ProfileResult& result)
        {
            std::stringstream json;

            json << ",{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.End - result.Start) << ',';
            json << "\"name\":\"" << result.Name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.ThreadID << ",";
            json << "\"ts\":" << result.Start;
            json << "}";

            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession)
            {
                m_OutputStream << json.str();
                m_OutputStream.flush();
            }
        }

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:

        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
            m_OutputStream.flush();
        }

        void WriteFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        // Note: you must already own lock on m_Mutex before
        // calling InternalEndSession()
        void InternalEndSession()
        {
            if (m_CurrentSession)
            {
                WriteFooter();
                m_OutputStream.close();
                delete m_CurrentSession;
                m_CurrentSession = nullptr;
            }
        }

    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name)
                : m_Name(name), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!m_Stopped)
                Stop();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(
                    m_StartTimepoint).time_since_epoch().count();
            long long end   = std::chrono::time_point_cast<std::chrono::microseconds>(
                    endTimepoint).time_since_epoch().count();

            Instrumentor::Get().WriteProfile({m_Name, start, end, std::this_thread::get_id()});

            m_Stopped = true;
        }

    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool                                                        m_Stopped;
    };

    namespace InstrumentorUtils
    {

        template<size_t N>
        struct ChangeResult
        {
            char Data[N];
        };

        template<size_t N, size_t K>
        constexpr auto CleanupOutputString(const char(& expr)[N], const char(& remove)[K])
        {
            ChangeResult<N> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;
            while (srcIndex < N)
            {
                size_t matchIndex = 0;
                while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 &&
                       expr[srcIndex + matchIndex] == remove[matchIndex])
                    matchIndex++;
                if (matchIndex == K - 1)
                    srcIndex += matchIndex;
                result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
                srcIndex++;
            }
            return result;
        }
    }
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
    #elif (defined(__FUNCSIG__) || (_MSC_VER))
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
    #define FF_PROFILE_SCOPE(name) constexpr auto fixedName = ::Firefly::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
    ::Firefly::InstrumentationTimer TOKENPASTE2(timer, __LINE__)(name)
    #define FF_PROFILE_FUNCTION() FF_PROFILE_SCOPE(FF_FUNC_SIG)
#else
    #define FF_PROFILE_BEGIN_SESSION(name, filepath)
    #define FF_PROFILE_END_SESSION()
    #define FF_PROFILE_SCOPE(name)
    #define FF_PROFILE_FUNCTION()
#endif