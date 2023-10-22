#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "Profiler.hpp"
#include <experimental/source_location>
#include <fstream>
#include <iostream>
#include <map>

#ifdef DEBUG
#define DEFAULT_ENABLE_FILE_INFO true;
#define DEFAULT_LOG_LEVEL Level::DEB;
#else
#define DEFAULT_ENABLE_FILE_INFO false;
#define DEFAULT_LOG_LEVEL Level::INFO;
#endif

using std::map;
using std::ofstream;
using std::string;

size_t cpyChar(char* dest, const char* src);
size_t cpyChar(char* dest, unsigned int src);

extern std::mutex mxLog;

enum class Target : short { DISABLED = 0,
    STDOUT = 1,
    STDERR = 2,
    LOG_FILE = 4 };

enum class Level : short { DEB = 1,
    INFO = 2,
    NOTICE = 3,
    WARNING = 4,
    ERR = 5,
    CRIT = 6,
    ALERT = 7,
    EMERG = 8 };

// String representations of Logger levels
static const map<Level, const char*> levelMap = {
    { Level::DEB, "DEBUG" },
    { Level::INFO, "INFO" },
    { Level::NOTICE, "NOTICE" },
    { Level::WARNING, "WARNING" },
    { Level::ERR, "ERROR" },
    { Level::CRIT, "CRITICAL" },
    { Level::ALERT, "ALERT" },
    { Level::EMERG, "EMERGENCY" } //
};

// TODO fix file being created even if im not logging to file
class Logger {
private:
    std::mutex mxLog;

public:
    // write() uses these variables to determine which messages should be written where.
    Level LoggerLevel = DEFAULT_LOG_LEVEL;
    short LoggerTarget = 0;
    string LoggerFile = "log.log";
    ofstream LoggingFileStream;

    // this can speed up time stamp aquisition by 75%
    std::time_t lastTime = 0;
    char timeStr[200] = "[";

    // Flags that change Logger style
    bool timestampEnabled = true;
    bool levelEnabled = true;
    bool fileEnabled = DEFAULT_ENABLE_FILE_INFO;
    bool deletePrevLog = true;

    ~Logger()
    {
        this->LoggingFileStream.close();
        free(timeString);
        free(levelString);
        free(fileString);
        free(loggerFunctionInfoString);
        free(loggerMessageString);
    }

#pragma region Target and level

    /* Set the location to Logger messages.
     *
     * \param   Target	The Logger target
     */
    void setTarget(Target target);

    void xorTarget(Target target);
    void orTarget(Target target);
    /* Set the severity of messages to Logger.
     *
     * \param	Level	The Logger level to set
     */
    void setLevel(Level level);

    /* Get the current Logger level. Only messages
     * with equal or higher severity will be written.
     *
     * \return	Level	The current Logger level
     */
    Level getLevel() const;

    /* Convert the Level enum to a string.
     *
     * \param	Level	The level to convert
     * \return	string	The converted level
     */
    string levelToString(Level level) const;

#pragma endregion Target and level

#pragma region setFile
    /* Set a file to Logger to if the target is LOG_FILE.
     *
     * \param	string	The file to which we will Logger
     */
    short setFile(const string& fileName, bool deleteFile = false,
        const std::experimental::source_location location = std::experimental::source_location::current());

    short setFile(const string& fileName, ofstream::openmode mode, bool deleteFile = false,
        const std::experimental::source_location location = std::experimental::source_location::current());
#pragma endregion setFile

    /* Log a message.
     *
     * \param	Level	The severity of the message
     * \param	string	The message to write
     */
    void write(Level level, const char* message, const std::experimental::source_location location);

    char* getLoggerfunctionInfo(Level level, const std::experimental::source_location location);

#pragma region Logs
    /* Log a Debug(lvl 1) message.
     *
     * \param	string	The message to write
     */
    void inline logDeb(const char* message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::DEB, message, location);
    }
    void inline logDeb(std::string const& message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::DEB, message.c_str(), location);
    }
    /* Log a Info(lvl 2) message.
     *
     * \param	string	The message to write
     */
    void inline logInfo(const char* message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::INFO, message, location);
    }
    void inline logInfo(std::string const& message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::INFO, message.c_str(), location);
    }
    /* Log a Notice(Lvl 3) message.
     *
     * \param	string	The message to write
     */
    void inline logNotice(const char* message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::NOTICE, message, location);
    }
    void inline logNotice(std::string const& message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::NOTICE, message.c_str(), location);
    }
    /* Log a Warning(lvl 4) message.
     *
     * \param	string	The message to write
     */
    void inline logWar(const char* message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::WARNING, message, location);
    }
    void inline logWar(std::string const& message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::WARNING, message.c_str(), location);
    }
    /* Log a Error(lvl 5) message.
     *
     * \param	string	The message to write
     */
    void inline logErr(const char* message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::ERR, message, location);
    }

    void inline logErr(std::string const& message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::ERR, message.c_str(), location);
    }
    /* Log a Critical(lvl 6) message.
     *
     * \param	string	The message to write
     */
    void inline logCrit(const char* message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::CRIT, message, location);
    }
    void inline logCrit(std::string const& message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::CRIT, message.c_str(), location);
    }
    /* Log a Alert(lvl 7) message.
     *
     * \param	string	The message to write
     */
    void inline logAlert(const char* message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::ALERT, message, location);
    }
    void inline logAlert(std::string const& message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::ALERT, message.c_str(), location);
    }
    /* Log a Emergency(lvl 8) message.
     *
     * \param	string	The message to write
     */
    void inline logEmerg(const char* message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::EMERG, message, location);
    }
    void inline logEmerg(std::string const& message,
        const std::experimental::source_location location = std::experimental::source_location::current())
    {
        this->write(Level::EMERG, message.c_str(), location);
    }

#pragma endregion Logs

#pragma region boolSets

    /* Date and time will no longer be printed with each Logger message.
     */
    void excludeTimestamp() { this->timestampEnabled = false; }

    /* Date and time will be printed with each Logger message.
     */
    void includeTimestamp() { this->timestampEnabled = true; }

    /* Logger level will no longer be printed with each Logger message.
     */
    void excludeLoggerLevel() { this->levelEnabled = false; }

    /* Logger level will be printed with each Logger message.
     */
    void includeLoggerLevel() { this->levelEnabled = true; }

    /* Function info will no longer be printed with each Logger message.
     */
    void excludeFunctionInfo() { this->fileEnabled = false; }

    /* Function info will be printed with each Logger message.
     */
    void includeFunctionInfo() { this->fileEnabled = true; }

#pragma endregion boolSets

protected:
#define tempStringStartSize 256
    size_t fileStringSize = tempStringStartSize;
    size_t loggerFunctionInfoStringSize = tempStringStartSize;
    size_t loggerMessageSize = tempStringStartSize * 2;

    char* timeString = (char*)malloc(sizeof(char) * tempStringStartSize);
    char* levelString = (char*)malloc(sizeof(char) * 64);
    char* fileString = (char*)malloc(sizeof(char) * tempStringStartSize);

    char* loggerFunctionInfoString = (char*)malloc(sizeof(char) * tempStringStartSize);
    char* loggerMessageString = (char*)malloc(sizeof(char) * tempStringStartSize * 2);
};

extern Logger logger;

#pragma region Bit - wise operators
inline Target operator&(Target a, Target b)
{
    return static_cast<Target>(static_cast<short>(a) & static_cast<short>(b));
}
inline Target operator|(Target a, Target b)
{
    return static_cast<Target>(static_cast<short>(a) | static_cast<short>(b));
}
#pragma endregion Bit - wise operators
// __attribute__ ((warning("unsafe memory management")))
inline size_t cpyChar(char* dest, const char* src)
{
    size_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = src[i];
    return i;
}
// __attribute__ ((warning("unsafe memory management")))
inline size_t cpyChar(char* dest, unsigned int src)
{
    size_t y = 0;
    char* numbers = (char*)malloc(sizeof(char) * 10);

    while (src >= 10) {
        numbers[y] = '0' + src % 10;
        src /= 10;
        y++;
    }
    numbers[y] = '0' + src;
    y++;
    size_t i = 0;
    for (i = 0; i < y; i++) {
        dest[i] = numbers[y - i - 1];
    }
    dest[i] = '\0';

    free(numbers);
    return i;
}

#endif