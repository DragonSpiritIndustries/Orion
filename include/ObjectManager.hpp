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
