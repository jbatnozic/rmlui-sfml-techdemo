#pragma once

#include <RmlUi/Core.h>

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>

namespace jbatnozic {
namespace spempe {

//! TODO(description)
class SizedRenderTarget {
public:
    virtual ~SizedRenderTarget() = default;

    virtual sf::RenderTarget& operator*() = 0;

    virtual sf::RenderTarget* operator->() = 0;

    virtual sf::Vector2u getSize() const = 0;
};

//! TODO(description)
template <class taRenderTarget>
class ConcreteSizedRenderTarget : public SizedRenderTarget {
public:
    explicit ConcreteSizedRenderTarget(taRenderTarget& aRenderTarget)
        : _renderTarget{aRenderTarget} {}

    sf::RenderTarget& operator*() {
        return _renderTarget;
    }

    sf::RenderTarget* operator->() {
        return std::addressof(_renderTarget);
    }

    sf::Vector2u getSize() const override {
        return _renderTarget.getSize();
    }

private:
    taRenderTarget& _renderTarget;
};

class RmlUiContextDriver {
public:
    RmlUiContextDriver(const std::string& aContextName,
                       std::unique_ptr<SizedRenderTarget> aRenderTarget);

    template <class taRenderTarget>
    RmlUiContextDriver(const std::string& aContextName,
                       taRenderTarget& aRenderTarget);

    Rml::Context& operator*();

    const Rml::Context& operator*() const;

    Rml::Context* operator->();

    const Rml::Context* operator->() const;

    void render();

    //! Returns: Whether the event was consumed by an element (true) in the context or not (false).
    bool processEvent(sf::Event aEvent);

    void update();

    //! Sets a new render target for this context driver.
    //! It's recommended to use the templated verion below.
    void setRenderTarget(std::unique_ptr<SizedRenderTarget> aRenderTarget);

    template <class taRenderTarget>
    void setRenderTarget(taRenderTarget& aRenderTarget);

private:
    std::unique_ptr<SizedRenderTarget> _renderTarget;
    Rml::Context* _context = nullptr;
};

template <class taRenderTarget>
RmlUiContextDriver::RmlUiContextDriver(const std::string& aContextName,
                                       taRenderTarget& aRenderTarget)
    : RmlUiContextDriver{
        aContextName,
        std::make_unique<ConcreteSizedRenderTarget<taRenderTarget>>(aRenderTarget)
    }
{
}

template <class taRenderTarget>
void RmlUiContextDriver::setRenderTarget(taRenderTarget& aRenderTarget) {
    setRenderTarget(
        std::make_unique<ConcreteSizedRenderTarget<taRenderTarget>>(aRenderTarget)
    );
}

} // namespace spempe
} // namespace jbatnozic
