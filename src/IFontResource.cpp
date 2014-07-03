#include "IFontResource.hpp"
#include FONTRESOURCE_IMPL_HEADER
#include "ResourceManager.hpp"

IFontResource::IFontResource(const std::string& path)
    : IResource(path)
{
}


REGISTER_RESOURCE(FONTRESOURCE_IMPL, "ttf otf", loadFont);
