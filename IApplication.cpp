#include "IApplication.hpp"
IApplication* IApplication::m_instance = nullptr;


IApplication::IApplication()
{
    m_instance = this;
}

IApplication& IApplication::instanceByRef()
{
    return *m_instance;
}

IApplication* IApplication::instanceByPtr()
{
    return  m_instance;
}
