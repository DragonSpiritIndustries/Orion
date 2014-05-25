#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <vector>

class IComponent;
class Object
{
public:
    typedef std::vector<Object*>::iterator         Iterator;
    typedef std::vector<Object*>::const_iterator   ConstIterator;
    typedef std::vector<Object*>::reverse_iterator ReverseIterator;
    Object(const std::string& name);

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
private:
    std::string m_name;
    std::vector<Object*>    m_children;
    std::vector<IComponent*> m_components;
};

#endif // OBJECT_HPP
