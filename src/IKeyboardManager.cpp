#include "IKeyboardManager.hpp"

void IKeyboardManager::onUpdate(float)
{
    for (std::pair<const Key, bool>& key : m_releasedKeys)
        key.second = false;
}
