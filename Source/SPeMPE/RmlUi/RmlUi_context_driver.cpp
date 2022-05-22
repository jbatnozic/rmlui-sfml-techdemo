
#include <SPeMPE/RmlUi/RmlUi_context_driver.hpp>
#include <SPeMPE/RmlUi/RmlUi_backend.hpp>

#include <RmlUi/Core.h>

#include <cassert>
#include <stdexcept>
#include <utility>

#include "RmlUi_SFML_renderer.hpp"

namespace jbatnozic {
namespace spempe {

RmlUiContextDriver::RmlUiContextDriver(const std::string& aContextName,
                   std::unique_ptr<SizedRenderTarget> aRenderTarget)
    : _renderTarget{std::move(aRenderTarget)}
{
    assert(_renderTarget && "Render target must not be null!");

    _context = Rml::CreateContext(aContextName, 
                                  Rml::Vector2i{
                                      static_cast<int>(_renderTarget->getSize().x),
                                      static_cast<int>(_renderTarget->getSize().y)
                                  },
                                  nullptr);
    if (!_context) {
        std::runtime_error{"RmlUiContextDriver - Could not create context " + aContextName};
    }
}

Rml::Context& RmlUiContextDriver::operator*() {
    return *_context;
}

const Rml::Context& RmlUiContextDriver::operator*() const {
    return *_context;
}

Rml::Context* RmlUiContextDriver:: operator->() {
    return _context;
}

const Rml::Context* RmlUiContextDriver::operator->() const {
    return _context;
}

void RmlUiContextDriver::render() {
    auto* renderer = RmlUiBackend::getRenderer();
    assert(renderer);

    renderer->setRenderTarget(_renderTarget.get());
    _context->Render();
    renderer->setRenderTarget(nullptr);
}

bool RmlUiContextDriver::processEvent(sf::Event aEvent) {
    const auto modifiers = RmlUiBackend::getKeyModifiers();

    switch (aEvent.type) {
    case sf::Event::MouseMoved:
        return !_context->ProcessMouseMove(aEvent.mouseMove.x, 
                                           aEvent.mouseMove.y,
                                           modifiers);

    case sf::Event::MouseButtonPressed:
        return !_context->ProcessMouseButtonDown(aEvent.mouseButton.button,
                                                 modifiers);

    case sf::Event::MouseButtonReleased:
        return !_context->ProcessMouseButtonUp(aEvent.mouseButton.button,
                                               modifiers);

    case sf::Event::MouseWheelMoved:
        return !_context->ProcessMouseWheel(static_cast<float>(-aEvent.mouseWheel.delta),
                                            modifiers);

    case sf::Event::TextEntered:
        // note: 0..31 are control characters
        if (aEvent.text.unicode > 31) {
            return !_context->ProcessTextInput(Rml::Character(aEvent.text.unicode));
        }
        return false;

    case sf::Event::KeyPressed:
        return !_context->ProcessKeyDown(RmlUiBackend::translateKey(aEvent.key.code),
                                         modifiers);

    case sf::Event::KeyReleased:
        return !_context->ProcessKeyUp(RmlUiBackend::translateKey(aEvent.key.code),
                                       modifiers);

    default:
        return false;
    }
}

void RmlUiContextDriver::update() {
    _context->Update();
}

void RmlUiContextDriver::setRenderTarget(std::unique_ptr<SizedRenderTarget> aRenderTarget) {
    _renderTarget = std::move(aRenderTarget);
}

} // namespace spempe
} // namespace jbatnozic
