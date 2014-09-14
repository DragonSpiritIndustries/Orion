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

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <vector>
#include <iostream>
#include "Vector2.hpp"

class IComponent;
class ScriptResource;
class ApplicationBase;
class asILockableSharedBool;
class asIScriptObject;
class asIScriptContext;
class Object
{
public:
    typedef std::vector<Object*>::iterator         Iterator;
    typedef std::vector<Object*>::const_iterator   ConstIterator;
    typedef std::vector<Object*>::reverse_iterator ReverseIterator;
    Object(const std::string& name);
    virtual ~Object();

    /*!
     * \brief Returns the objects name
     * \return The name of the object
     */
    std::string name() const;
    /*!
     * \brief setName Sets the name of the object, names should be unique, but it does not check for uniqueness
     * \param name The new name for the object
     */
    void setName(const std::string& name);
    /*!
     * \brief addChild Appends a child object to the end of the list,
     * does not do any sorting
     * \param child The child to insert
     */
    void addChild(Object* child);
    /*!
     * \brief removeChild Removes a child by name, it deletes the child completely
     * use takeChild to remove and pass the child.
     * \param name The name of the child to remove
     */
    void removeChild(const std::string& name);
    /*!
     * \brief removeChild Removes a child by index, it deletes the child completely
     * use takeChild to remove and pass the child.
     * \param index The index of the child to remove.
     */
    void removeChild(int index);
    /*!
     * \brief child Returns a child by name, returns nullptr if the child doesn't exist
     * \param name The child to locate
     * \return The child
     */
    void removeChild(Object* child);

    /*!
     * \brief child Returns a child by name, returns nullptr if the child doesn't exist
     * \param name The child to locate
     * \return The child
     */
    Object* child(const std::string& name);

    /*!
     * \brief child Returns a child by index, returns nullptr if the child doesn't exist
     * \param index The index to locate
     * \return The child
     */
    Object* child(int index);
    /*!
     * \brief child Returns a child by name, returns nullptr if the child doesn't exist
     * if the child exists, it removes it from the list
     * \param name The child to locate
     * \return The requested child
     */
    Object* takeChild(const std::string& name);
    /*!
     * \brief child Returns a child by index, returns nullptr if the child doesn't exist
     * if the child exists, it removes it from the list
     * \param index The index to locate
     * \return The child
     */
    Object* takeChild(int index);

    virtual void onCreate();
    virtual void onDestroyed();

    virtual void onDraw();
    virtual void setPosition(float x, float y);
    virtual void setPosition(const Vector2f& position);
    virtual Vector2f position() const;
    virtual void move(float x, float y);
    virtual void move(const Vector2f& amount);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    virtual int state();
    virtual void setState(int state);
    virtual void onThink(float delta);
    virtual void onUpdate(float delta);

    int  addRef();
    int  release();
    void destroyAndRelease();
    asILockableSharedBool* weakRefFlag();

    /*!
     * \brief Adds a new component to the components and returns it
     * \param type The Component type to add i.e Transformable
     * \param componentName The name of the component
     * \return nullptr on failure the instance of the new component otherwise
     */
    IComponent* addComponent(const std::string& type, const std::string& componentName);
    void removeComponent(IComponent* component);
protected:
    ScriptResource*        m_script;
    asILockableSharedBool* m_weakRefFlag;
    asIScriptContext*      m_scriptContext;

    std::string m_name;
    Vector2f m_position;
    std::vector<Object*>    m_children;
    std::vector<IComponent*> m_components;
    int m_refCount;
    int m_id;
    int m_state;

};

#endif // OBJECT_HPP
