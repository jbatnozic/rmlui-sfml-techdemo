/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 Nuno Silva
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <RmlUi_SFMLAdapters/SFML_render_interface.hpp>

#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/FileInterface.h>
#include <SFML/Graphics.hpp>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

void RmlUiSFMLRenderer::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture,
                                              const Rml::Vector2f& translation)
{
    if (texture)
    {
        sf::Texture::bind((sf::Texture*)texture);
        texture = RenderInterface_GL2::TextureIgnoreBinding;
    }

    RenderInterface_GL2::RenderGeometry(vertices, num_vertices, indices, num_indices, texture, translation);
}

bool RmlUiSFMLRenderer::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source)
{
    Rml::FileInterface* file_interface = Rml::GetFileInterface();
    Rml::FileHandle file_handle = file_interface->Open(source);
    if (!file_handle)
        return false;

    file_interface->Seek(file_handle, 0, SEEK_END);
    size_t buffer_size = file_interface->Tell(file_handle);
    file_interface->Seek(file_handle, 0, SEEK_SET);

    char* buffer = new char[buffer_size];
    file_interface->Read(buffer, buffer_size, file_handle);
    file_interface->Close(file_handle);

    sf::Texture* texture = new sf::Texture();
    texture->setSmooth(true);

    bool success = texture->loadFromMemory(buffer, buffer_size);

    delete[] buffer;

    if (success)
    {
        texture_handle = (Rml::TextureHandle)texture;
        texture_dimensions = Rml::Vector2i(texture->getSize().x, texture->getSize().y);
    }
    else
    {
        delete texture;
    }

    return success;
}

bool RmlUiSFMLRenderer::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
{
    sf::Texture* texture = new sf::Texture();
    texture->setSmooth(true);

    if (!texture->create(source_dimensions.x, source_dimensions.y))
    {
        delete texture;
        return false;
    }

    texture->update(source, source_dimensions.x, source_dimensions.y, 0, 0);
    texture_handle = (Rml::TextureHandle)texture;

    return true;
}

void RmlUiSFMLRenderer::ReleaseTexture(Rml::TextureHandle texture_handle)
{
    delete (sf::Texture*)texture_handle;
}
