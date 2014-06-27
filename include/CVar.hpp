#ifndef CVAR_HPP
#define CVAR_HPP

#include <string>

#include "IKeyboardManager.hpp"
#include "IMouseManager.hpp"
#include "Color.hpp"

class TiXmlNode;

class CVar
{
public:

    enum Type
    {
        Boolean,
        Integer,
        Float,
        Literal,
        Color,
        Bind
    };

    enum Flags
    {
        All      = -1,
        System   = (1 << 0),
        Game     = (1 << 1),
        Gui      = (1 << 2),
        Cheat    = (1 << 3),
        ReadOnly = (1 << 4),
        Archive  = (1 << 5),
        Modified = (1 << 6)
    };

    CVar(const std::string& name, const std::string& value, const std::string& help, Type type, int flags);
//    CVar(const std::string& name, Binding value, const std::string& help, int flags);
    CVar(const std::string& name, const Colorf& value, const std::string& help, int flags);
    CVar(const std::string& name, const Colorb& value, const std::string& help, int flags);
    CVar(const std::string& name, const Colori& value, const std::string& help, int flags);



    std::string name()                         const;
    std::string help()                         const;
    float       toFloat (bool* isValid  =NULL) const;
    bool        toBoolean(bool* isValid =NULL) const;
    int         toInteger(bool* isValid =NULL) const;
    std::string toLiteral(bool* isValid =NULL) const;
    Colorf      toColorf (bool* isValid =NULL) const;
    Colorb      toColorb (bool* isValid =NULL) const;
    Colori      toColori (bool* isValid =NULL) const;
//    Binding     toBinding(bool* isValid =NULL) const;

    bool fromFloat  (const float val);
    bool fromBoolean(const bool val);
    bool fromInteger(const int val);
    bool fromLiteral(const std::string& val);
    bool fromColorf (const Colorf& val);
    bool fromColorb (const Colorb& val);
    bool fromColori (const Colori& val);

 //   bool fromBinding(Binding binding);

    bool isFloat()       const;
    bool isBoolean()     const;
    bool isInteger()     const;
    bool isLiteral()     const;
    bool isBinding()     const;
    bool isModified()    const;
    bool isReadOnly()    const;
    bool isCheat()       const;
    bool isArchive()     const;
    void clearModified();
    void setModified();
    void clearBindings();
    bool tryBinding(const std::string& binding);

    Type type() const;
    int flags() const;

    void deserialize(TiXmlNode* rootNode);
    void serializeCVar(TiXmlNode* rootNode, bool oldDeveloper);
    void serializeBinding(TiXmlNode* rootNode);

    /*!
     * \brief Unlocks the CVar for writing if it is ReadOnly.
     * <b>Handle with care!!!</b> if you use unlock(), make sure
     * you lock the cvar using lock()
     * \see lock
     */
    void unlock();

    /*!
     * \brief Locks the CVar to prevent writing if it is ReadOnly.
     * Unlike it's partner function unlock, lock is harmless
     * \see unlock
     */
    void lock();
protected:
    std::string m_name;
    std::string m_value;
    std::string m_defaultValue;
    std::string m_help;
    Type        m_type;
    int         m_flags;
    bool        m_allowedWrite;
};


#endif // CVAR_HPP

