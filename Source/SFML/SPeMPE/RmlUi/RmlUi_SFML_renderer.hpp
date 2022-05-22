#pragma once

#include <RmlUi/Core/RenderInterface.h>
#include <SFML/Graphics.hpp>
#include <SPeMPE/RmlUi/RmlUi_context_driver.hpp>

namespace jbatnozic {
namespace spempe {

class RmlUiSFMLRenderer : public Rml::RenderInterface
{
public:
    RmlUiSFMLRenderer() = default;

    //! Sets the render target
    void setRenderTarget(SizedRenderTarget* aRenderTarget);

    //! Returns the currently render target
    SizedRenderTarget* getRenderTarget() const;

    ///////////////////////////////////////////////////////////////////////////
    // INHERITED FROM RML::RENDERINTERFACE                                   //
    ///////////////////////////////////////////////////////////////////////////

    //! Called by RmlUi when it wants to render geometry that it does not wish to optimise.
    void RenderGeometry(Rml::Vertex* aVertices,
                        int aVerticesCount,
                        int* aIndices,
                        int aIndicesCount,
                        Rml::TextureHandle aTexture,
                        const Rml::Vector2f& aTranslation) override;

    //! Called by RmlUi when it wants to compile geometry it believes will be static for the forseeable future.
    Rml::CompiledGeometryHandle CompileGeometry(Rml::Vertex*, int, int*, int, Rml::TextureHandle) override;

    //! Called by RmlUi when it wants to render application-compiled geometry.
    void RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation) override;

    //! Called by RmlUi when it wants to release application-compiled geometry.
    void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry) override;

    //! Called by RmlUi when it wants to enable or disable scissoring to clip content.
    void EnableScissorRegion(bool aEnable) override;

    //! Called by RmlUi when it wants to change the scissor region.
    void SetScissorRegion(int x, int y, int width, int height) override;

    //! Called by RmlUi when a texture is required by the library.
    bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source) override;

    //! Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
    bool GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) override;

    //! Called by RmlUi when a loaded texture is no longer required.
    void ReleaseTexture(Rml::TextureHandle texture_handle) override;

    //! ?????????????????
    static constexpr Rml::TextureHandle TextureIgnoreBinding = static_cast<Rml::TextureHandle>(-1);

private:
    void _initViewport();

    SizedRenderTarget* _renderTarget = nullptr;
};

} // namespace spempe
} // namespace jbatnozic
