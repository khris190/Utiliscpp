#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <mutex>
#include <string>
#include <vector>
struct Sample {
    long nsTime;
    std::string name;
    Sample() = default;
    explicit Sample(std::string const& name)
        : name(name)
    {
    }
    Sample(std::string const& name, long nsTime)
        : nsTime(nsTime)
        , name(name)
    {
    }
};

class Profiler {
private:
    std::mutex mxSamples;
    std::vector<Sample> samples;
    static Profiler* instance_;
    ~Profiler();
    Profiler();

public:
    static Profiler* getInstance();
    Profiler(Profiler& other) = delete;
    void operator=(const Profiler&) = delete;

    void AddSample(Sample sample);

    std::string getTimingsAsString(bool doClearSamples = true);
    std::vector<Sample> getTimings(bool doClearSamples = true);

    void clearSamples();
    void printProfilerData(bool doClearSamples = true);
};

#define TOKENPASTE(x, y) x##y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
// use for acurate creation to block end timing cant be used in return scope //TODO deal with that problem
#define newTimer(name) PTimer TOKENPASTE2(Timer_, __LINE__) = PTimer(name)
// use by throwing newTimer({string name}) into code block, it will measure to the end of a block
class PTimer {
private:
    Sample sample;
    std::chrono::_V2::system_clock::time_point startTime;

public:
    explicit PTimer(const std::string& name);
    ~PTimer();
};

#endif // PROFILER_HPP
