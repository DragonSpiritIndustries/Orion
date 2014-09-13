#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <Vector2.hpp>
class Viewport
{
public:
    Viewport();
    Viewport(const float x, const float y, const int width, const int height);
    Viewport(const Vector2f& location, const Vector2i& size);

    void setLocation(float x, float y);
    void setLocation(const Vector2f& location);
    Vector2f location() const;
    float x() const;
    float y() const;

    void setSize(const int w, const int h);
    void setSize(const Vector2i& size);

    void setWidth(const int w);
    void setHeight(const int h);

    int  width() const;
    int  height() const;

    Vector2i size() const;
    Vector2i center();
private:
    Vector2f m_location;
    Vector2i m_size;
};

#endif // VIEWPORT_HPP
