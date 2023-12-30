#include "Logger.hpp"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <memory>
#include <mutex>
#include <stdio.h>
#include <string>

using std::chrono::system_clock;

// TODO use string_view?
Logger logger;

void Logger::setTarget(Target target) { this->LoggerTarget = (short)target; }
void Logger::xorTarget(Target target) { this->LoggerTarget ^= (short)target; }
void Logger::orTarget(Target target) { this->LoggerTarget |= (short)target; }

void Logger::setLevel(Level level) { this->LoggerLevel = level; }

Level Logger::getLevel() const { return this->LoggerLevel; }

string Logger::levelToString(Level level) const { return levelMap.at(level); }

short Logger::setFile(const string& fileName, bool deleteFile, const std::source_location location)
{
    if (this->LoggingFileStream.is_open()) {
        this->LoggingFileStream.close();
    }
    // Make sure we can open the file for writing
    if (deleteFile) {
        remove(fileName.c_str());
    }
    this->LoggingFileStream.open(fileName, ofstream::app);
    if (!this->LoggingFileStream.is_open()) {
        // Logger the failure and return an error code
        this->write(Level::ERR, ("Failed to open Logger file '" + fileName + "'").c_str(), location);
        return 1;
    }
    this->LoggerFile = fileName;
    return 0;
}

short Logger::setFile(
    const string& fileName, ofstream::openmode mode, bool deleteFile, const std::source_location location)
{
    if (this->LoggingFileStream.is_open()) {
        this->LoggingFileStream.close();
    }
    if (deleteFile) {
        remove(fileName.c_str());
    }
    this->LoggingFileStream.open(fileName, mode);
    if (!this->LoggingFileStream.is_open()) {
        // Logger the failure and return an error code
        this->write(Level::ERR, ("Failed to open Logger file '" + fileName + "'").c_str(), location);
        return 1;
    }
    this->LoggerFile = fileName;
    return 0;
}

void Logger::write(Level level, const char* message, const std::source_location location)
{
    // Only log if we're at or above the pre-defined severity
    if (level < this->LoggerLevel) {
        return;
    }
    // Target::DISABLED takes precedence over other targets
    if (this->LoggerTarget == (short)Target::DISABLED) {
        return;
    }
    size_t offset = 0;
    if (this->LoggerTarget & (short)Target::LOG_FILE && !this->LoggingFileStream.is_open()) {
        setFile(this->LoggerFile);
    }

    // Append the message to our Logger statement
    if (this->fileEnabled || this->timestampEnabled || this->levelEnabled) {
        getLoggerfunctionInfo(level, location);
    }

    if (loggerMessageSize < loggerFunctionInfoStringSize + strlen(message) + 10) {
        loggerMessageSize = loggerFunctionInfoStringSize + strlen(message) + 10;
        free(loggerMessageString);
        loggerMessageString = (char*)malloc(sizeof(char) * loggerMessageSize);
    }
    if (this->fileEnabled || this->timestampEnabled || this->levelEnabled) {
        offset += cpyChar(loggerMessageString, loggerFunctionInfoString);
        offset += cpyChar(loggerMessageString + offset, ":\n");
    }
    offset += cpyChar(loggerMessageString + offset, message);
    offset += cpyChar(loggerMessageString + offset, "\n");

    // printf makes printing a bit faster
    // Logger to stdout if it's one of our targets
    if ((this->LoggerTarget & (short)Target::STDOUT)) {
        std::scoped_lock<std::mutex> lock(mxLog);
        fprintf(stdout, "%s", loggerMessageString);
    }

    // Logger to stderr if it's one of our targets
    if ((this->LoggerTarget & (short)Target::STDERR)) {
        std::scoped_lock<std::mutex> lock(mxLog);
        fprintf(stderr, "%s", loggerMessageString);
    }

    // Logger to a file if it's one of our targets and we've set a LoggerFile
    if ((this->LoggerTarget & (short)Target::LOG_FILE) && this->LoggerFile != "") {
        std::scoped_lock<std::mutex> lock(mxLog);
        this->LoggingFileStream << loggerMessageString;
        this->LoggingFileStream.flush();
    }
}

char* Logger::getLoggerfunctionInfo(Level level, const std::source_location location)
{
    size_t offset = 0;
    size_t fullSize = 0;
    // Append the current date and time if enabled
    if (this->timestampEnabled) {
        if (std::time_t time = system_clock::to_time_t(system_clock::now()); this->lastTime < time) {
            this->lastTime = time;
            struct tm const* timeStruct = std::localtime(&time);
            strftime(&this->timeStr[1], 199, "%d/%b/%Y %H:%M:%S", timeStruct);
        }
        offset = cpyChar(timeString + offset, this->timeStr);
        offset += cpyChar(timeString + offset, "] \0");
        fullSize += offset;
    }

    if (this->levelEnabled) {
        offset = cpyChar(levelString, levelMap.at(level));
        offset += cpyChar(levelString + offset, " \0");
        fullSize += offset;
    }

    if (this->fileEnabled) {
        size_t fileNameSize = strlen(location.file_name());
        size_t funcNameSize = strlen(location.function_name());
        size_t stringSize = fileNameSize + funcNameSize + 20 + 5;

        if (stringSize > fileStringSize) {
            fileStringSize = stringSize + 8;
            free(fileString);
            fileString = (char*)malloc(sizeof(char) * (stringSize));
        }
        offset = cpyChar(fileString, location.file_name());
        offset += cpyChar(fileString + offset, ":");
        offset += cpyChar(fileString + offset, location.line());
        offset += cpyChar(fileString + offset, ";");
        offset += cpyChar(fileString + offset, location.column());
        offset += cpyChar(fileString + offset, "  ");
        offset += cpyChar(fileString + offset, location.function_name());
        cpyChar(fileString + offset, "\0");
        fullSize += offset;
    }
    if (fullSize > loggerFunctionInfoStringSize) {
        loggerFunctionInfoStringSize = fullSize;
        free(loggerFunctionInfoString);
        loggerFunctionInfoString = (char*)malloc(sizeof(char) * (fullSize));
    }
    offset = cpyChar(loggerFunctionInfoString, timeString);
    offset += cpyChar(loggerFunctionInfoString + offset, levelString);
    cpyChar(loggerFunctionInfoString + offset, fileString);

    return loggerFunctionInfoString;
}
