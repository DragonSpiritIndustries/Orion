#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "Color.hpp"
#include "IKeyboardManager.hpp"
#include <vector>
#include <fstream>

class ApplicationBase;
class Console
{
public:
    static const char ColorChar = '^';
    /*!
     * \enum State
     * \brief The State enumerator is used to determine the Console's current state.
     */
    enum State
    {
        Opened,  //!< The Console is open and accepting input
        Closed,  //!< The Console is closed and not drawn
        Opening, //!< The Console is in the process of opening, but not accepting input
        Closing  //!< The Console is in the process of closing, and is still drawn
    };

    /*!
     * \brief The Level enum
     */
    enum Level
    {
        Message, //!< The lowest level, it is not prepended to the text, usually for chat text, or simple messages that don't need attention
        Info,    //!< Usually information about resources, may also contain information relevant to the games state.
        Warning, //!< Usually used when the engine can't find a resource, which isn't usually a big deal
        Error,   //!< Used when the engine encounters a non-fatal error, like a packet drop
        Fatal    //!< Used when the engine encounters a fatal error, like a required resource isn't available, e.g fonts/debug.ttf
    };
    enum
    {
        MAX_LEN = 255 //! The maximum length the command text can be.
    };

    /*!
     * \brief The LogEntry struct
     */
    struct LogEntry
    {
        /*!
         * \brief level
         */
        Level level;
        /*!
         * \brief timestamp
         */
        std::string timestamp;
        /*!
         * \brief message
         */
        std::string message;
    };

    /*!
     * \brief Console
     * \param logfile
     */
    Console(const std::string& logfile="log.txt");

    virtual void initialize();
    virtual void shutdown();
    virtual bool isInitialized() const;
    virtual bool isOpen() const;
    virtual bool isClosed() const;
    virtual void handleText(int utf8);
    virtual void handleInput(Key key, bool alt, bool control, bool shift, bool system);
    virtual void handleMouseWheel(int delta, int x, int y);
    virtual void draw();
    virtual void print(Level level, const std::string &fmt, ...);
    virtual void clear();
    virtual void toggleConsole();
    virtual Colorb consoleColor();
    virtual Colorb textColor();

    static Console& instanceRef();
    static Console* instancePtr();
protected:
    virtual void doAutoComplete();
    virtual void drawHistory();
    virtual void drawSeparator();
    virtual void drawVersion();
    virtual void parseCommand();
    virtual void parseCVars();
    virtual void addEntry(const Level level, const std::string& message, const std::string& timestamp, const std::string label = std::string());
    std::ofstream m_log;
    int                   m_startString;
    std::vector<LogEntry> m_history;
};


#define orConsoleRef     Console::instanceRef()
#define orConsolePtr     Console::instancePtr()

#endif // CONSOLE_HPP
