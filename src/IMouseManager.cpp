#include "IMouseManager.hpp"
#include "IApplication.hpp"

IMouseManager::IMouseManager()
{
    orApplicationRef.updateSignal().connect<IMouseManager, &IMouseManager::onUpdate>(this);
}

void IMouseManager::onUpdate(float)
{
    for (std::pair<const MouseButton, bool>& button : m_released)
        button.second = false;
}
