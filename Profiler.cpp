#include "Profiler.hpp"
#include <chrono>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>

// Profiler profiler;
Profiler::Profiler() { }

Profiler* Profiler::getInstance()
{
    if (instance_ == nullptr) {
        instance_ = new Profiler();
    }
    return instance_;
}

void Profiler::AddSample(Sample sample)
{
    std::scoped_lock<std::mutex> lock(mxSamples);
    for (auto& sampleTmp : samples) {
        if (sampleTmp.name == sample.name) {
            sampleTmp.nsTime += sample.nsTime;
            return;
        }
    }
    samples.push_back(sample);
}

std::string Profiler::getTimingsAsString(bool doClearSamples)
{
    std::string retString = "";
#ifdef DEBUG
    retString = "DEBUG TIMINGS!!!\n ";
#endif

    std::vector<Sample> localSamples = getTimings(doClearSamples);
    long time = 0;
    for (auto const& localSample : localSamples) {
        retString += localSample.name;
        retString += ": ";
        time = localSample.nsTime;
        retString += std::to_string(time) + "ns.  ";
        time /= 1000000; // change to ms.
        if (time >= 1) {
            retString += std::to_string(time) + "ms.  ";
            time /= 1000; // change to s.
            if (time >= 1) {
                retString += std::to_string(time) + "s.";
            }
        }
        retString += "\n";
    }
    if (localSamples.size()) {
        return retString;
    } else {
        return "no timings";
    }
}

std::vector<Sample> Profiler::getTimings(bool doClearSamples)
{
    std::vector<Sample> retSample;

    mxSamples.lock();
    for (auto const& sample : samples) {
        retSample.emplace_back(sample);
    }
    mxSamples.unlock();
    if (doClearSamples) {
        clearSamples();
    }

    return retSample;
}

void Profiler::clearSamples()
{
    std::scoped_lock<std::mutex> lock(mxSamples);
    samples.clear();
}

void Profiler::printProfilerData(bool doClearSamples)
{
    for (size_t i = 0; i < samples.size(); i++) {
        std::cout << samples[i].name << ": " << samples[i].nsTime << "ns" << std::endl;
    }
    if (doClearSamples) {
        clearSamples();
    }
}

Profiler::~Profiler()
{
    clearSamples();
    if (instance_) {
        delete instance_;
    }
}

Profiler* Profiler::instance_;

PTimer::PTimer(const std::string& name)
{
    sample.name = name;
    startTime = std::chrono::system_clock::now();
}

PTimer::~PTimer()
{
    sample.nsTime = std::chrono::duration<long, std::nano>(std::chrono::system_clock::now() - startTime).count();
    Profiler::getInstance()->AddSample(sample);
}
