// This file is part of Orion.
//
// Orion is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Orion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Orion.  If not, see <http://www.gnu.org/licenses/>

#ifndef CVAR_HPP
#define CVAR_HPP

#include <string>

#include "IKeyboardManager.hpp"
#include "IMouseManager.hpp"
#include "IJoystickManager.hpp"
#include "Color.hpp"

class TiXmlNode;

struct Joystick
{
    int     Button;
    int     Axis;
    bool    NegativeAxis;
    bool    valid;
};

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
        All      = -1, // NOTE: is this really necessary? It seems rather overkill
        System   = (1 << 0),
        Game     = (1 << 1),
        Gui      = (1 << 2),
        Cheat    = (1 << 3),
        Hidden   = (1 << 4),
        ReadOnly = (1 << 5),
        Archive  = (1 << 6),
        Modified = (1 << 7)
    };

    class Binding
    {
    public:
        Key               KeyVal;
        MouseButton       MouseButtonVal;
        Joystick          Joysticks[IJoystickManager::MaxJoysticks];

        Binding();
        Binding(Key key);
        Binding(MouseButton btn);
        Binding(int joy, int btn);
        Binding(int joy, int axis, bool neg = false);
        void clear();
    };

    CVar(const std::string& name, const std::string& value, const std::string& help, Type type, int flags);
    CVar(const std::string& name, Binding value, const std::string& help, int flags);
    CVar(const std::string& name, const Colorf& value, const std::string& help, int flags);
    CVar(const std::string& name, const Colorb& value, const std::string& help, int flags);
    CVar(const std::string& name, const Colori& value, const std::string& help, int flags);



    std::string name()                            const;
    std::string help()                            const;
    float       toFloat (bool* isValid  =nullptr) const;
    bool        toBoolean(bool* isValid =nullptr) const;
    int         toInteger(bool* isValid =nullptr) const;
    std::string toLiteral(bool* isValid =nullptr) const;
    Colorf      toColorf (bool* isValid =nullptr) const;
    Colorb      toColorb (bool* isValid =nullptr) const;
    Colori      toColori (bool* isValid =nullptr) const;
    Binding     toBinding(bool* isValid =nullptr) const;

    bool fromFloat  (const float val);
    bool fromBoolean(const bool val);
    bool fromInteger(const int val);
    bool fromLiteral(const std::string& val);
    bool fromColorf (const Colorf& val);
    bool fromColorb (const Colorb& val);
    bool fromColori (const Colori& val);

    bool fromBinding(Binding binding);

    bool isFloat()       const;
    bool isBoolean()     const;
    bool isInteger()     const;
    bool isLiteral()     const;
    bool isBinding()     const;
    bool isColor()       const;
    bool isModified()    const;
    bool isReadOnly()    const;
    bool isCheat()       const;
    bool isHidden()      const;
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
    Binding     m_binding;
};

class CVarUnlocker
{
public:
    CVarUnlocker(CVar* cvar);
    ~CVarUnlocker();
private:
    CVar* m_cvar;
};

#endif // CVAR_HPP

