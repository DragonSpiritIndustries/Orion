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

#ifndef OBJECTMANAGER_HPP
#define OBJECTMANAGER_HPP

#include <vector>
#include <memory>

class Object;
class ApplicationBase;
class ObjectManager
{
public:
    typedef std::vector<Object*>::iterator         Iterator;
    typedef std::vector<Object*>::const_iterator   ConstIterator;
    typedef std::vector<Object*>::reverse_iterator ReverseIterator;

    ObjectManager();
    ~ObjectManager();
    void addObject(Object* obj);
    void removeObject(Object* obj);
    void removeObject(const std::string& name);
    Object* takeObject(const std::string& name);
    Object* object(const std::string& name);

    void initialize();
    void shutdown();
    static ObjectManager& instanceRef();
    static ObjectManager* instancePtr();

    void draw();
private:
    std::vector<Object*> m_objects;
    static std::shared_ptr<ObjectManager> m_instance;
    static int objectId;
};

#define orObjectManagerRef ObjectManager::instanceRef()
#define orObjectManagerPtr ObjectManager::instancePtr()

#endif // OBJECTMANAGER_HPP
