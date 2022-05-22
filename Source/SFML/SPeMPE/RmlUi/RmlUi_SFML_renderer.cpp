
#include <SPeMPE/RmlUi/RmlUi_SFML_renderer.hpp>

#include <RmlUi/Core.h>

#include <GL/glew.h>

namespace jbatnozic {
namespace spempe {

void RmlUiSFMLRenderer::setRenderTarget(SizedRenderTarget* aRenderTarget) {
    _renderTarget = aRenderTarget;
}

SizedRenderTarget* RmlUiSFMLRenderer::getRenderTarget() const {
    return _renderTarget;
}

///////////////////////////////////////////////////////////////////////////
// INHERITED FROM RML::RENDERINTERFACE                                   //
///////////////////////////////////////////////////////////////////////////

void RmlUiSFMLRenderer::RenderGeometry(
    Rml::Vertex* aVertices,
    int aVerticesCount,
    int* aIndices,
    int aIndicesCount,
    Rml::TextureHandle aTexture,
    const Rml::Vector2f& aTranslation
) {
    if (!_renderTarget) {
        return;
    }
    (*_renderTarget)->pushGLStates();
    _initViewport();

    glTranslatef(aTranslation.x, aTranslation.y, 0);

    Rml::Vector<Rml::Vector2f> positions(aVerticesCount);
    Rml::Vector<Rml::Colourb> colors(aVerticesCount);
    Rml::Vector<Rml::Vector2f> texCoords(aVerticesCount);

    for (int i = 0; i < aVerticesCount; i++)
    {
        positions[i] = aVertices[i].position;
        colors[i] = aVertices[i].colour;
        texCoords[i] = aVertices[i].tex_coord;
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, &positions[0]);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (auto sfTexture = reinterpret_cast<sf::Texture*>(aTexture)) {
        sf::Texture::bind(sfTexture);
    }
    else {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, 0);
    };

    glDrawElements(GL_TRIANGLES, aIndicesCount, GL_UNSIGNED_INT, aIndices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glColor4f(1.f, 1.f, 1.f, 1.f);

    (*_renderTarget)->popGLStates();
}

Rml::CompiledGeometryHandle RmlUiSFMLRenderer::CompileGeometry(Rml::Vertex*, int, int*, int, const Rml::TextureHandle) {
    return reinterpret_cast<Rml::CompiledGeometryHandle>(nullptr);
}

void RmlUiSFMLRenderer::RenderCompiledGeometry(Rml::CompiledGeometryHandle, const Rml::Vector2f&) {
    RMLUI_ASSERT(false);
}

void RmlUiSFMLRenderer::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle) {
    RMLUI_ASSERT(false);
}

void RmlUiSFMLRenderer::EnableScissorRegion(bool aEnable) {
    if (aEnable) {
        glEnable(GL_SCISSOR_TEST);
    }
    else {
        glDisable(GL_SCISSOR_TEST);
    }
}

void RmlUiSFMLRenderer::SetScissorRegion(int x, int y, int width, int height) {
    if (!_renderTarget) {
        return;
    }
    glScissor(x, (*_renderTarget)->getSize().y - (y + height), width, height);
}

bool RmlUiSFMLRenderer::LoadTexture(Rml::TextureHandle& texture_handle,
                                    Rml::Vector2i& texture_dimensions,
                                    const Rml::String& source) {
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

    if (!texture->loadFromMemory(buffer, buffer_size))
    {
        delete[] buffer;
        delete texture;

        return false;
    };
    delete[] buffer;

    texture_handle     = reinterpret_cast<Rml::TextureHandle>(texture);
    texture_dimensions = Rml::Vector2i(texture->getSize().x, texture->getSize().y);

    return true;
}

bool RmlUiSFMLRenderer::GenerateTexture(Rml::TextureHandle& texture_handle,
                                        const Rml::byte* source,
                                        const Rml::Vector2i& source_dimensions) {
    sf::Texture* texture = new sf::Texture();

    if (!texture->create(source_dimensions.x, source_dimensions.y)) {
        delete texture;
        return false;
    }

    texture->update(source, source_dimensions.x, source_dimensions.y, 0, 0);
    texture_handle = reinterpret_cast<Rml::TextureHandle>(texture);

    return true;
}

void RmlUiSFMLRenderer::ReleaseTexture(Rml::TextureHandle texture_handle) {
    delete (reinterpret_cast<sf::Texture*>(texture_handle));
}

///////////////////////////////////////////////////////////////////////////
// PRIVATE                                                               //
///////////////////////////////////////////////////////////////////////////

void RmlUiSFMLRenderer::_initViewport() {
    if (!_renderTarget) {
        return;
    }

    glViewport(0, 0, _renderTarget->getSize().x, _renderTarget->getSize().y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, _renderTarget->getSize().x, _renderTarget->getSize().y, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

} // namespace spempe
} // namespace jbatnozic
