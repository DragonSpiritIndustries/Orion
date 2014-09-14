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

#ifndef TRANSFORMABLECOMONENT_HPP
#define TRANSFORMABLECOMONENT_HPP

#include "IComponent.hpp"
#include "Vector2.hpp"

class TransformableComponent : public IComponent
{
public:
    TransformableComponent(const std::string& name);

    Vector2f position() const;
    void setPosition(float x, float y);
    void setPosition(const Vector2f& position);
    void move(float x, float y);
    void move(const Vector2f& offset);

    Vector2f scale() const;
    void setScale(float x, float y);
    void setScale(const Vector2f& scale);
    float rotation() const;
    void setRotation(float rotation);
    //!< Creates a new TransformableComponent
    //!< CAN be used directly, but not advised
    static IComponent* newComponent(const std::string& name);

private:
    Vector2f m_position;
    Vector2f m_scale;
    float    m_rotation;
};

#endif // TRANSFORMABLECOMONENT_HPP
