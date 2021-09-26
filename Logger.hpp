#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <memory>

using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::ofstream;
using std::shared_ptr;
using std::string;
using std::chrono::system_clock;

namespace Utilis
{
    class Logger
    {
    public:
        enum class Target : short
        {
            DISABLED = 1,
            STDOUT = 2,
            STDERR = 4,
            LOG_FILE = 8
        };

        enum class Level : short
        {
            DEB = 1,
            INFO = 2,
            NOTICE = 3,
            WARNING = 4,
            ERR = 5,
            CRIT = 6,
            ALERT = 7,
            EMERG = 8
        };

        /**
			 * Set the location to Logger messages.
			 *
			 * \param   Target	The Logger target
			 */
        void setTarget(Target target);

        void xorTarget(Target target);
        /**
			 * Set the severity of messages to Logger.
			 *
			 * \param	Level	The Logger level to set
			 */
        void setLevel(Level level);

        /**
			 * Set a file to Logger to if the target is LOG_FILE.
			 *
			 * \param	string	The file to which we will Logger
			 */
        short setFile(string fileName);
        short setFile(string fileName, ofstream::openmode mode);

        /**
			 * Get the current Logger level. Only messages
			 * with equal or higher severity will be written.
			 *
			 * \return	Logger::Level	The current Logger level
			 */
        Level getLevel();

        /**
			 * Convert the Logger::Level enum to a string.
			 *
			 * \param	Logger::Level	The level to convert
			 * \return	string	The converted level
			 */
        string levelToString(Level level);

        /**
			 * Log a message.
			 *
			 * \param	Level	The severity of the message
			 * \param	string	The message to write
			 */
        void write(Level level, string message);

        /**
			 * Log a Debug(lvl 1) message.
			 *
			 * \param	string	The message to write
			 */
        void LogDeb(string message);
        /**
			 * Log a Info(lvl 2) message.
			 *
			 * \param	string	The message to write
			 */
        void LogInfo(string message);
        /**
			 * Log a Notice(Lvl 3) message.
			 *
			 * \param	string	The message to write
			 */
        void LogNotice(string message);
        /**
			 * Log a Warning(lvl 4) message.
			 *
			 * \param	string	The message to write
			 */
        void LogWar(string message);
        /**
			 * Log a Error(lvl 5) message.
			 *
			 * \param	string	The message to write
			 */
        void LogErr(string message);
        /**
			 * Log a Critical(lvl 6) message.
			 *
			 * \param	string	The message to write
			 */
        void LogCrit(string message);
        /**
			 * Log a Alert(lvl 7) message.
			 *
			 * \param	string	The message to write
			 */
        void LogAlert(string message);
        /**
			 * Log a Emergency(lvl 8) message.
			 *
			 * \param	string	The message to write
			 */
        void LogEmerg(string message);

        /**
			 * Date and time will no longer be printed with each Logger message.
			 */
        void excludeTimestamp()
        {
            this->timestampEnabled = false;
        }

        /**
			 * Date and time will be printed with each Logger message.
			 */
        void includeTimestamp()
        {
            this->timestampEnabled = true;
        }

        /**
			 * Logger level will no longer be printed with each Logger message.
			 */
        void excludeLoggerLevel()
        {
            this->levelEnabled = false;
        }

        /**
			 * Logger level will be printed with each Logger message.
			 */
        void includeLoggerLevel()
        {
            this->levelEnabled = true;
        }


        // write() uses these variables to determine which messages should be written where.
        Level LoggerLevel = Level::INFO;
        short LoggerTarget = (short)Target::STDOUT;
        string LoggerFile = "log.txt";

        // Flags that change Logger style
        bool timestampEnabled = true;
        bool levelEnabled = true;

        // String representations of Logger levels
        map<Level, string> levelMap = {
            {Level::DEB, "DEBUG"},
            {Level::INFO, "INFO"},
            {Level::NOTICE, "NOTICE"},
            {Level::WARNING, "WARNING"},
            {Level::ERR, "ERROR"},
            {Level::CRIT, "CRITICAL"},
            {Level::ALERT, "ALERT"},
            {Level::EMERG, "EMERGENCY"}};
    };

    ////////////////////////
    // Bit-wise operators //
    ////////////////////////
    inline Logger::Target operator&(Logger::Target a, Logger::Target b)
    {
        return static_cast<Logger::Target>(static_cast<short>(a) & static_cast<short>(b));
    }
    inline Logger::Target operator|(Logger::Target a, Logger::Target b)
    {
        return static_cast<Logger::Target>(static_cast<short>(a) | static_cast<short>(b));
    }

    Logger Log;

    void Utilis::Logger::setTarget(Utilis::Logger::Target target)
    {
        this->LoggerTarget = (short)target;
    }

    void Utilis::Logger::xorTarget(Utilis::Logger::Target target)
    {
        this->LoggerTarget ^= (short)target;
    }

    void Utilis::Logger::setLevel(Utilis::Logger::Level level)
    {
        this->LoggerLevel = level;
    }

    short Utilis::Logger::setFile(string fileName)
    {
        // Make sure we can open the file for writing
        ofstream LoggerFile(fileName, ofstream::app);
        if (!LoggerFile.is_open())
        {
            // Utilis::Logger the failure and return an error code
            this->write(Utilis::Logger::Level::ERR, "Failed to open Logger file '" + fileName + "'");
            return 1;
        }
        this->LoggerFile = fileName;
        return 0;
    }
    short Utilis::Logger::setFile(string fileName, ofstream::openmode mode)
    {
        ofstream LoggerFile(fileName, mode);
        if (!LoggerFile.is_open())
        {
            // Utilis::Logger the failure and return an error code
            this->write(Utilis::Logger::Level::ERR, "Failed to open Logger file '" + fileName + "'");
            return 1;
        }
        this->LoggerFile = fileName;
        return 0;
    }

    Utilis::Logger::Level Utilis::Logger::getLevel()
    {
        return this->LoggerLevel;
    }

    string Utilis::Logger::levelToString(Utilis::Logger::Level level)
    {
        return this->levelMap[level];
    }

    void Utilis::Logger::write(Utilis::Logger::Level level, string message)
    {
        string toLogger;

        // Only Logger if we're at or above the pre-defined severity
        if (level < this->LoggerLevel)
        {
            return;
        }

        // Utilis::Logger::Target::DISABLED takes precedence over other targets
        if ((this->LoggerTarget & (short)Utilis::Logger::Target::DISABLED) == (short)Utilis::Logger::Target::DISABLED)
        {
            return;
        }

        // Append the Logger level if enabled
        if (this->levelEnabled)
        {
            toLogger += this->levelToString(level);
        }

        // Append the current date and time if enabled
        if (this->timestampEnabled)
        {
            std::time_t time = system_clock::to_time_t(system_clock::now());
            struct tm *timeStruct = std::localtime(&time);

            char timeStr[80];
            strftime(timeStr, 80, "%d/%b/%Y:%H:%M:%S %z", timeStruct);
            toLogger += " [" + string(timeStr) + "]";
        }

        // Append the message to our Logger statement
        toLogger += " " + message;

        // Utilis::Logger to stdout if it's one of our targets
        if ((this->LoggerTarget & (short)Utilis::Logger::Target::STDOUT) == (short)Utilis::Logger::Target::STDOUT)
        {
            std::cout << toLogger << std::endl;
        }

        // Utilis::Logger to stderr if it's one of our targets
        if ((this->LoggerTarget & (short)Utilis::Logger::Target::STDERR) == (short)Utilis::Logger::Target::STDERR)
        {
            std::cerr << toLogger << std::endl;
        }

        // Utilis::Logger to a file if it's one of our targets and we've set a LoggerFile
        if ((this->LoggerTarget & (short)Utilis::Logger::Target::LOG_FILE) == (short)Utilis::Logger::Target::LOG_FILE && this->LoggerFile != "")
        {
            ofstream LoggerFile(this->LoggerFile, ofstream::app);
            LoggerFile << toLogger << "\n";
            LoggerFile.close();
        }
    }
    
    void Utilis::Logger::LogDeb(string message)
    {
        this->write(Logger::Level::DEB, message);
    }
    void Utilis::Logger::LogInfo(string message)
    {
         this->write(Logger::Level::INFO, message);
    }
    void Utilis::Logger::LogNotice(string message)
    {
         this->write(Logger::Level::NOTICE, message);
    }
    void Utilis::Logger::LogWar(string message)
    {
         this->write(Logger::Level::WARNING, message);
    }
    void Utilis::Logger::LogErr(string message)
    {
         this->write(Logger::Level::ERR, message);
    }
    void Utilis::Logger::LogCrit(string message)
    {
         this->write(Logger::Level::CRIT, message);
    }
    void Utilis::Logger::LogAlert(string message)
    {
         this->write(Logger::Level::ALERT, message);
    }
    void Utilis::Logger::LogEmerg(string message)
    {
         this->write(Logger::Level::EMERG, message);
    }

}
#endif