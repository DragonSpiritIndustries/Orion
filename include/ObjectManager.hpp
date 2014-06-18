#ifndef OBJECTMANAGER_HPP
#define OBJECTMANAGER_HPP

#include <vector>
#include <memory>

class Object;
class ObjectManager
{
public:
    typedef std::vector<Object*>::iterator         Iterator;
    typedef std::vector<Object*>::const_iterator   ConstIterator;
    typedef std::vector<Object*>::reverse_iterator ReverseIterator;

    ~ObjectManager();
    void addObject(Object* obj);
    void removeObject(Object* obj);
    void removeObject(const std::string& name);
    Object* takeObject(const std::string& name);

    void shutdown();
    static ObjectManager& instanceRef();
    static ObjectManager* instancePtr();
private:
    std::vector<Object*> m_objects;
    static std::shared_ptr<ObjectManager> m_instance;
};

#define orObjectManagerRef ObjectManager::instanceRef()
#define orObjectManagerPtr ObjectManager::instancePtr()

#endif // OBJECTMANAGER_HPP
