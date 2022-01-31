#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <vector>
#include <string>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <iostream>

struct Sample
{
    int nsTime;
    std::string name;
    Sample(){}
    Sample(std::string name)
    {
        this->name = name;
    }
    Sample(std::string name, int nsTime)
    {
        this->name = name;
        this->nsTime = nsTime;
    }
};

class Profiler
{
private:
    std::vector<Sample> samples;

public:
    Profiler()
    {
    }
    inline void AddSample(Sample sample)
    {
        bool foundSample = false;

        for (size_t i = 0; i < samples.size(); i++)
        {
            if (sample.name == samples[i].name)
            {
                samples[i].nsTime += sample.nsTime;
                foundSample = true;
                break;
            }
        }
        if (!foundSample)
        {
            samples.push_back(sample);
        }
    }

    std::vector<Sample> getTimings(bool doClearSamples = true)
    {
        std::vector<Sample> retSample;

        for (size_t i = 0; i < samples.size(); i++)
        {
            retSample.push_back(Sample(samples[i].name, samples[i].nsTime));
        }
        if (doClearSamples)
        {
            clearSamples();
        }

        return retSample;
    }
    void clearSamples()
    {
        samples.clear();
    }
    void printProfilerData(bool doClearSamples = true)
    {
        auto timings = getTimings(doClearSamples);
        for (size_t i = 0; i < timings.size(); i++)
        {
            std::cout << timings[i].name << ": " << timings[i].nsTime << "ns" << std::endl;
        }
    }
    ~Profiler()
    {
        clearSamples();
    }
};

static Profiler profiler;

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
// use for acurate creation to block end timing
#define newTimer(name) PTimer TOKENPASTE2(Timer_, __LINE__) = PTimer(name)
// use by throwing newTimer(string name) into code block that has to be measured
class PTimer
{
private:
    Sample sample;
    std::chrono::_V2::system_clock::time_point startTime;

public:
    PTimer(std::string name)
    {
        sample.name = name;
        startTime = std::chrono::system_clock::now();
    }
    ~PTimer()
    {
        sample.nsTime = std::chrono::duration<int, std::nano>(std::chrono::system_clock::now() - startTime).count();
        profiler.AddSample(sample);
    }
};

#endif // PROFILER_HPP
