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

#include "CVar.hpp"
#include "CVarManager.hpp"
#include "ApplicationBase.hpp"
#include "Console.hpp"
#include <sstream>
#include <algorithm>
#include <tinyxml.h>
#include <Athena/Utility.hpp>
#include "EnumToString.hpp"

extern CVar* com_developer;
extern CVar* com_enableCheats;

CVar::CVar(const std::string& name, const std::string &value, const std::string &help, Type type, int flags)
    : m_name(name),
      m_value(value),
      m_defaultValue(value),
      m_help(help),
      m_type(type),
      m_flags(flags),
      m_allowedWrite(false)
{
    if (!orCVarManagerRef.registerCVar(this))
        delete this;
}

CVar::CVar(const std::string& name, CVar::Binding value, const std::string& help, int flags)
    : m_name(name),
      m_help(help),
      m_type(Bind),
      m_flags(flags),
      m_allowedWrite(false),
      m_binding(value)
{
    if (!orCVarManagerRef.registerCVar(this))
        delete this;
}

CVar::CVar(const std::string& name, const Colorf& value, const std::string& help, int flags)
    : m_name(name),
      m_help(help),
      m_type(Color),
      m_flags(flags),
      m_allowedWrite(false)
{
    // Unlock the cvar for writing if readonly
    unlock();

    fromColorf(value);

    // Lock the cvar
    lock();
    // Clear the modified flag, just incase lock didn't do it.
    m_flags = flags;

    if (!orCVarManagerRef.registerCVar(this))
        delete this;
}

CVar::CVar(const std::string& name, const Colorb& value, const std::string& help, int flags)
    : m_name(name),
      m_help(help),
      m_type(Color),
      m_flags(flags),
      m_allowedWrite(false)
{
    // Unlock the cvar for writing if readonly
    unlock();

    fromColorb(value);

    // Lock the cvar
    lock();
    // Clear the modified flag, just incase lock didn't do it.
    m_flags = flags;

    if (!orCVarManagerRef.registerCVar(this))
        delete this;
}

CVar::CVar(const std::string& name, const Colori& value, const std::string& help, int flags)
    : m_name(name),
      m_help(help),
      m_type(Color),
      m_flags(flags),
      m_allowedWrite(false)
{
    // Unlock the cvar for writing if readonly
    unlock();

    fromColori(value);

    // Lock the cvar
    lock();
    // Clear the modified flag, just incase lock didn't do it.
    m_flags = flags;

    if (!orCVarManagerRef.registerCVar(this))
        delete this;
}

std::string CVar::name() const
{
    return m_name;
}

std::string CVar::help() const
{
    return std::string(m_help + (m_defaultValue != std::string() ? "\ndefault: " + m_defaultValue : "") +
                       (isReadOnly() ? "[ReadOnly]" : ""));
}

float CVar::toFloat(bool* isValid) const
{
    if (m_type != Float)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "CVar %s is not a float", name().c_str());
        if (isValid)
            *isValid = false;
        return 0.0f;
    }

    try
    {
        float ret;
        std::stringstream ss;
        ss << m_value;
        ss >> ret;

        if (isValid)
            *isValid = true;

        return ret;
    }
    catch (...)
    {
        if (isValid)
            *isValid = false;
        return 0.0f;
    }
}

bool CVar::toBoolean(bool* isValid) const
{
    if (m_type != Boolean)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "CVar %s is not a boolean", name().c_str());
        if (isValid)
            *isValid = false;

        return false;
    }

    // We don't want to modify the original value;
    std::string tmp = m_value;
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);

    if (!tmp.compare("yes") || !tmp.compare("true") || !tmp.compare("1"))
    {
        if (isValid)
            *isValid = true;
        return true;
    }
    else if (!tmp.compare("no") || !tmp.compare("false") || !tmp.compare("0"))
    {
        if (isValid)
            *isValid = true;
        return false;
    }

    if (isValid)
        *isValid = false;

    return false;
}

int CVar::toInteger(bool* isValid) const
{
    if (m_type != Integer)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "CVar %s is not an integer", name().c_str());
        if (isValid)
            *isValid = false;
        return -1;
    }

    try
    {
        int ret;
        std::stringstream ss;
        ss << m_value;
        ss >> ret;
        if (isValid)
            *isValid = true;
        return ret;
    }
    catch(...)
    {
        if (isValid)
            *isValid = false;
        return -1;
    }
}

std::string CVar::toLiteral(bool* isValid) const
{
    if (m_type != Literal  && (com_developer && com_developer->toBoolean()))
    {
        orConsoleRef.print(orConsoleRef.Warning, "CVar %s is not a literal", name().c_str());
        if (isValid != NULL)
            *isValid = false;
    }
    else if (isValid != NULL)
        *isValid = true;

    // Even if it's not a literal, it's still safe to return
    return m_value;
}

static const float inv255 = (1.f/255.f);
Colorf CVar::toColorf(bool* isValid) const
{
    if (m_type != Color)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "CVar %s is not a color", name().c_str());

        if (isValid != NULL)
            *isValid = false;

        return Colorf();
    }

    if (isValid != NULL)
        *isValid = true;

    int r, g, b, a;
    std::stringstream ss;

    ss << m_value;
    ss >> r;
    ss >> g;
    ss >> b;
    ss >> a;

    return Colorf(r*inv255, g*inv255, b*inv255, a*inv255);
}

Colorb CVar::toColorb(bool* isValid) const
{
    if (m_type != Color)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "CVar %s is not a color", name().c_str());

        if (isValid != NULL)
            *isValid = false;

        return Colorb();
    }

    if (isValid != NULL)
        *isValid = true;

    int r, g, b, a;
    std::stringstream ss;

    ss << m_value;
    ss >> r;
    ss >> g;
    ss >> b;
    ss >> a;

    Colorb ret(r, g, b, a);
    return ret;
}

Colori CVar::toColori(bool* isValid) const
{
    if (m_type != Color)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "CVar %s is not a color", name().c_str());

        if (isValid != NULL)
            *isValid = false;

        return Colori();
    }

    if (isValid != NULL)
        *isValid = true;

    int r, g, b, a;
    std::stringstream ss;

    ss << m_value;
    ss >> r;
    ss >> g;
    ss >> b;
    ss >> a;

    return Colori(r, g, b, a);
}

CVar::Binding CVar::toBinding(bool* isValid) const
{
    if (m_type != Bind)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "CVar %s is not a binding", name().c_str());

        if (isValid != NULL)
            *isValid = false;

        return Binding();
    }

    if (isValid != NULL)
        *isValid = true;

    return m_binding;
}

bool CVar::fromFloat(const float val)
{
    if (isCheat() && (!com_developer->toBoolean() && !com_enableCheats->toBoolean()))
        return false;

    if (m_type != Float)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "Invalid assignment, CVar %s is not a float", name().c_str());

        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        orConsoleRef.print(orConsoleRef.Warning, "CVar %s is readonly.", name().c_str());
        return false;
    }

    std::stringstream ss;
    ss << val;
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}

bool CVar::fromBoolean(const bool val)
{
    if (isCheat() && (!com_developer->toBoolean() && !com_enableCheats->toBoolean()))
        return false;

    if (m_type != Boolean)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "Invalid assignment, CVar %s is not a boolean", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        orConsoleRef.print(orConsoleRef.Warning, "CVar %s is readonly.", name().c_str());
        return false;
    }

    std::stringstream ss;
    ss << val;
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}

bool CVar::fromInteger(const int val)
{
    if (isCheat() && (!com_developer->toBoolean() && !com_enableCheats->toBoolean()))
        return false;

    if (m_type != Integer)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "Invalid assignment, CVar %s is not an integer", name().c_str());

        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        orConsoleRef.print(orConsoleRef.Warning, "CVar %s is readonly.", name().c_str());
        return false;
    }

    std::stringstream ss;
    ss << val;
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}

bool CVar::fromLiteral(const std::string& val)
{
    if (isCheat() && (!com_developer->toBoolean() && !com_enableCheats->toBoolean()))
        return false;

    if (m_type != Literal)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "Invalid assignment, CVar %s is not a literal", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        orConsoleRef.print(orConsoleRef.Warning, "CVar %s is readonly, restart in developer mode.", name().c_str());
        return false;
    }

    m_value = val;
    m_flags |= Modified;
    return true;
}

bool CVar::fromColorf(const Colorf& val)
{
    if (isCheat() && (!com_developer->toBoolean() && !com_enableCheats->toBoolean()))
        return false;

    if (m_type != Color)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "Invalid assignment, CVar %s is not a color", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        orConsoleRef.print(orConsoleRef.Warning, "CVar %s is readonly, restart in developer mode.", name().c_str());
        return false;
    }

    std::stringstream ss;
    unsigned char r, g, b, a;
    r = static_cast<unsigned char>(val.r);
    g = static_cast<unsigned char>(val.g);
    b = static_cast<unsigned char>(val.b);
    a = static_cast<unsigned char>(val.a);

    ss << (unsigned)r << " " <<
          (unsigned)g << " " <<
          (unsigned)b << " " <<
          (unsigned)a;

    m_value = ss.str();
    m_flags |= Modified;
    return true;
}


bool CVar::fromColorb(const Colorb& val)
{
    if (isCheat() && (!com_developer->toBoolean() && !com_enableCheats->toBoolean()))
        return false;

    if (m_type != Color)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "Invalid assignment, CVar %s is not a color", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        orConsoleRef.print(orConsoleRef.Warning, "CVar %s is readonly, restart in developer mode.", name().c_str());
        return false;
    }

    std::stringstream ss;


    ss << (((atUint32)val.r) & 255) << " ";
    ss << (((atUint32)val.g) & 255) << " ";
    ss << (((atUint32)val.b) & 255) << " ";
    ss << (((atUint32)val.a) & 255) << " ";
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}


bool CVar::fromColori(const Colori& val)
{
    if (isCheat() && (!com_developer->toBoolean() && !com_enableCheats->toBoolean()))
        return false;

    if (m_type != Color)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "Invalid assignment, CVar %s is not a color", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        orConsoleRef.print(orConsoleRef.Warning, "CVar %s is readonly, restart in developer mode.", name().c_str());
        return false;
    }

    std::stringstream ss;
    ss << (unsigned)val.r << " " <<
          (unsigned)val.g << " " <<
          (unsigned)val.b << " " <<
          (unsigned)val.a;
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}

bool CVar::fromBinding(CVar::Binding binding)
{
    if (isCheat() && (!com_developer->toBoolean() && !com_enableCheats->toBoolean()))
        return false;

    if (m_type != Bind)
    {
        if (com_developer && com_developer->toBoolean())
            orConsoleRef.print(orConsoleRef.Warning, "Invalid assignment, CVar %s is not a binding", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        orConsoleRef.print(orConsoleRef.Warning, "CVar %s is readonly, restart in developer mode.", name().c_str());
        return false;
    }

    m_binding = binding;
    return true;
}


bool CVar::isFloat() const
{
    return m_type == Float;
}

bool CVar::isBoolean() const
{
    return m_type == Boolean;
}

bool CVar::isInteger() const
{
    return m_type == Integer;
}

bool CVar::isLiteral() const
{
    return m_type == Literal;
}

bool CVar::isBinding() const
{
    return m_type == Bind;
}

bool CVar::isColor() const
{
    return m_type == Color;
}

bool CVar::isModified() const
{
    return (m_flags & Modified) == Modified;
}

bool CVar::isReadOnly() const
{
    return (m_flags & ReadOnly) == ReadOnly;
}

bool CVar::isCheat() const
{
    return (m_flags & Cheat) == Cheat;
}

bool CVar::isHidden() const
{
    return (m_flags & Hidden) == Hidden;
}

bool CVar::isArchive() const
{
    return (m_flags & Archive) == Archive;
}

void CVar::clearModified()
{
    if (isArchive())
        orCVarManagerRef.requestSerialization(this);
    m_flags &= ~Modified;
}

void CVar::setModified()
{
    m_flags |= Modified;
}

void CVar::clearBindings()
{
    m_binding.clear();
}

bool CVar::tryBinding(const std::string& binding)
{
    std::string bindingName = binding;
    Athena::utility::tolower(bindingName);
    if (binding.compare("mouse") != -1)
    {
        for (int i = 0; i < (int)MouseButton::COUNT; i++)
        {
            std::string name = enumToStdString((MouseButton)i);
            Athena::utility::tolower(name);
            if(!name.compare(bindingName))
            {
                m_binding.MouseButtonVal = (MouseButton)i;
                return true;
            }
        }
    }

    if (binding.compare("joy") > -1)
    {
        for (int i = 0; i < IJoystickManager::MaxJoysticks; i++)
        {
            // first axis
            for (int j = 0; j < orJoystickManagerRef.maxAxes(i); j++)
            {
                // First try axis
                std::string axisName = Athena::utility::sprintf("joy%i.%i", i, j);
                std::string negAxisName = Athena::utility::sprintf("-joy%i.%i", i, j);

                m_binding.Joysticks[i].Axis = j;
                m_binding.Joysticks[i].valid = true;
                if (!axisName.compare(bindingName))
                {
                    m_binding.Joysticks[i].NegativeAxis = false;
                    return true;
                }
                else if (!negAxisName.compare(bindingName))
                {
                    m_binding.Joysticks[i].NegativeAxis = true;
                    return true;
                }
            }

            // Now buttons
            for (int j = 0; j < orJoystickManagerRef.maxAxes(i); j++)
            {
                // First try axis
                std::string buttonName = Athena::utility::sprintf("joy%i.button%i", i, j);
                if (!buttonName.compare(bindingName))
                {
                    m_binding.Joysticks[i].Button = j;
                    m_binding.Joysticks[i].valid = true;
                    return true;
                }
            }
        }
    }

    // Now for keyboard
    for (int i = 0; i < (int)Key::KEYCOUNT; i++)
    {
        std::string keyName = enumToStdString((Key)i);
        Athena::utility::tolower(keyName);
        if (!bindingName.compare(keyName))
        {
            m_binding.KeyVal = (Key)i;
            return true;
        }
    }

    // Oops user specified an invalid key!
    return false;
}

CVar::Type CVar::type() const
{
    return m_type;
}

int CVar::flags() const
{
    return m_flags;
}

void CVar::deserialize(TiXmlNode* rootNode)
{
    int oldFlags = m_flags;
    unlock();
    if (rootNode == NULL)
        return;
    TiXmlElement* cvarNode = NULL;
    if (type() != Bind)
        cvarNode = rootNode->FirstChildElement("CVar");
    else
        cvarNode = rootNode->FirstChildElement("Bind");

    while (cvarNode != nullptr)
    {
        if (type() != Bind)
        {
            if (!std::string(cvarNode->Attribute("name")).compare(name()))
                break;

            cvarNode = cvarNode->NextSiblingElement("CVar");
        }
        else
        {
            if (!std::string(cvarNode->Attribute("action")).compare(name()))
                break;

            cvarNode = cvarNode->NextSiblingElement("Bind");
        }
    }

    if (!cvarNode)
        return;

    switch(type())
    {
        case Bind:
        {
            std::string tmp;
            if (cvarNode->Attribute("key"))
            {
                tmp = cvarNode->Attribute("key");
                Athena::utility::tolower(tmp);
                for (int k = 0; k < (int)Key::KEYCOUNT; k++)
                {
                    std::string keyName = enumToStdString((Key)k);
                    Athena::utility::tolower(keyName);
                    if (!keyName.compare(tmp))
                        m_binding.KeyVal = (Key)k;
                }
            }

            if (cvarNode->Attribute("mouseButton"))
            {
                tmp = cvarNode->Attribute("mouseButton");
                for (int m = 0; m < (int)MouseButton::COUNT; m++)
                {
                    std::string name = enumToStdString((MouseButton)m);
                    Athena::utility::tolower(name);
                    if (!name.compare(tmp))
                        m_binding.MouseButtonVal = (MouseButton)m;
                }
            }

            for (int j = 0; j < IJoystickManager::MaxJoysticks; j++)
            {
                std::stringstream ss;
                ss << j;
                TiXmlElement* joyNode = cvarNode->FirstChildElement("Joy");
                while (joyNode != NULL)
                {
                    if (!std::string(joyNode->Attribute("id")).compare(ss.str()))
                        break;
                    joyNode = joyNode->NextSiblingElement("Joy");
                }

                if (!joyNode)
                    continue;

                bool set = false;
                if (joyNode->Attribute("button"))
                {
                    int button;
                    joyNode->Attribute("button", &button);
                    m_binding.Joysticks[j].Button = button;
                    set = true;
                }
                if (joyNode->Attribute("axis"))
                {
                    int axis;
                    joyNode->Attribute("axis", &axis);
                    m_binding.Joysticks[j].Axis = axis;
                    set = true;
                }

                if (joyNode->Attribute("isAxisNegative"))
                {
                    tmp = joyNode->Attribute("isAxisNegative");
                    m_binding.Joysticks[j].NegativeAxis = Athena::utility::parseBool(tmp);
                    set = true;
                }
                m_binding.Joysticks[j].valid = set;
            }
        }
            break;
        case Color:
        {
            int r, g, b, a;
            cvarNode->Attribute("r", &r);
            cvarNode->Attribute("g", &g);
            cvarNode->Attribute("b", &b);
            cvarNode->Attribute("a", &a);
            fromColorb(Colorb(r, g, b, a));
        }
            break;
        default:
            m_value = cvarNode->FirstChild()->ToText()->ValueStr();
            break;
    }
    lock();
    m_flags = oldFlags;
}

void CVar::serializeCVar(TiXmlNode* rootNode, bool oldDeveloper)
{
    if (rootNode == NULL)
        return;

    TiXmlElement* cvarNode = rootNode->FirstChildElement("CVar");

    while (cvarNode != NULL)
    {
        if (!std::string(cvarNode->Attribute("name")).compare(name()))
            break;
        cvarNode = cvarNode->NextSiblingElement("CVar");
    }

    TiXmlText* text = NULL;
    TiXmlText* oldText = NULL;


    if (!cvarNode)
    {
        cvarNode = new TiXmlElement("CVar");
        cvarNode->SetAttribute("name", name());
        rootNode->LinkEndChild(cvarNode);
    }
    else
    {
        if (cvarNode->FirstChild())
        {
            oldText = cvarNode->FirstChild()->ToText();
        }
    }

    switch(type())
    {
        case CVar::Boolean:
        {
            cvarNode->SetAttribute("type", "boolean");
            std::string val;
            if (com_developer == this)
                val = (oldDeveloper ? "true" : "false");
            else
                val = (toBoolean() ? "true" : "false");

            text = new TiXmlText(val);
            break;
        }
        case CVar::Integer:
            cvarNode->SetAttribute("type", "integer");
            break;
        case CVar::Float:
            cvarNode->SetAttribute("type", "float");
            break;
        case CVar::Literal:
        {
            cvarNode->SetAttribute("type", "literal");
            text = new TiXmlText(toLiteral());
            text->SetCDATA(true);
            break;
        }
        case CVar::Color:
        {
            Colori col = toColori();
            cvarNode->SetAttribute("type", "color");
            cvarNode->SetAttribute("r", (col.r & 255));
            cvarNode->SetAttribute("g", (col.g & 255));
            cvarNode->SetAttribute("b", (col.b & 255));
            cvarNode->SetAttribute("a", (col.a & 255));
        }
            break;
        default: break;
    }

    if (!text && type() != Color)
        text = new TiXmlText(toLiteral());

    if (oldText && type() != Color)
    {
        cvarNode->RemoveChild(oldText);
    }

    if (text && type() != Color)
        cvarNode->LinkEndChild(text);
}

void CVar::serializeBinding(TiXmlNode* rootNode)
{
    TiXmlElement* bindNode = rootNode->FirstChildElement("Bind");

    while (bindNode != NULL)
    {
        if (!std::string(bindNode->Attribute("action")).compare(name()))
            break;

        bindNode = bindNode->NextSiblingElement("Bind");
    }

    if (!bindNode)
    {
        bindNode = new TiXmlElement("Bind");
        bindNode->SetAttribute("action", name());
        rootNode->LinkEndChild(bindNode);
    }

    if (m_binding.KeyVal != Key::UNKNOWN)
        bindNode->SetAttribute("key", enumToStdString(m_binding.KeyVal));
    if (m_binding.MouseButtonVal != MouseButton::UNKNOWN)
        bindNode->SetAttribute("mouseButton", enumToStdString(m_binding.MouseButtonVal));

    for (int i = 0; i < IJoystickManager::MaxJoysticks; i++)
    {
        if (!m_binding.Joysticks[i].valid)
            continue;

        std::stringstream ss;
        ss << i;
        TiXmlElement* joyNode = bindNode->FirstChildElement("Joy");
        while (joyNode != NULL)
        {
            if (!std::string(joyNode->Attribute("id")).compare(ss.str()))
                break;

            joyNode = joyNode->NextSiblingElement("Joy");
        }

        if (!joyNode)
        {
            joyNode = new TiXmlElement("Joy");
            joyNode->SetAttribute("id", i);
            bindNode->LinkEndChild(joyNode);
        }

        if (m_binding.Joysticks[i].Button != -1)
            joyNode->SetAttribute("button", m_binding.Joysticks[i].Button);

        if (m_binding.Joysticks[i].Axis != -1)
        {
            joyNode->SetAttribute("axis", m_binding.Joysticks[i].Axis);
            joyNode->SetAttribute("isAxisNegative", (m_binding.Joysticks[i].NegativeAxis ? "true" : "false"));
        }
    }
}

void CVar::unlock()
{
    if (!isReadOnly())
        return;

    if (!m_allowedWrite)
    {
        m_allowedWrite = true;
        m_flags &= ~ReadOnly;
    }
}

void CVar::lock()
{
    if (!isReadOnly())
        return;

    if (m_allowedWrite)
    {
        m_flags |= ReadOnly;
        m_allowedWrite = false;
        clearModified();
    }
}

CVar::Binding::Binding()
{
    clear();
}

CVar::Binding::Binding(Key key)
{
    clear();
    KeyVal = key;
}

CVar::Binding::Binding(MouseButton btn)
{
    MouseButtonVal = btn;
}

CVar::Binding::Binding(int joy, int btn)
{
    if (joy < -1 || joy > IJoystickManager::MaxJoysticks)
        return;

    Joysticks[joy].Button = btn;
    Joysticks[joy].valid = true;
}

CVar::Binding::Binding(int joy, int axis, bool neg)
{
    if (joy < -1 || joy > IJoystickManager::MaxJoysticks)
        return;

    Joysticks[joy].Axis = axis;
    Joysticks[joy].NegativeAxis = neg;
    Joysticks[joy].valid = true;
}

void CVar::Binding::clear()
{
    KeyVal = Key::UNKNOWN;
    MouseButtonVal = MouseButton::UNKNOWN;

    for (int i = 0; i < IJoystickManager::MaxJoysticks; i++)
    {
        Joysticks[i].Button       = -1;
        Joysticks[i].Axis         = -1;
        Joysticks[i].NegativeAxis = false;
        Joysticks[i].valid        = false;
    }
}

CVarUnlocker::CVarUnlocker(CVar* cvar)
    : m_cvar(cvar)
{
    if (m_cvar)
        m_cvar->unlock();
}

CVarUnlocker::~CVarUnlocker()
{
    if (m_cvar)
        m_cvar->lock();
}
