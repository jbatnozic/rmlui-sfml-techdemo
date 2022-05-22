#pragma once

#include <RmlUi/Core.h>
#include <SFML/Window.hpp>

#include <memory>

namespace jbatnozic {
namespace spempe {

class RmlUiSFMLSystem;
class RmlUiSFMLRenderer;
class RmlUiContextDriver;

class RmlUiBackend {
public:
    struct BackendLifecycleGuard {
        ~BackendLifecycleGuard();
    };

    static std::unique_ptr<BackendLifecycleGuard> initialize();

    //! Convenience function to convert from a SFML key to RmlUi key.
    static Rml::Input::KeyIdentifier translateKey(sf::Keyboard::Key aKey);

    //! Convenience function to get the current state of modifier keys (ctrl,
    //! alt, shift) in RmlUi format.
    static int getKeyModifiers();

private:
    friend class RmlUiContextDriver;

    static RmlUiSFMLSystem*   getSystem();
    static RmlUiSFMLRenderer* getRenderer();
};

} // namespace spempe
} // namespace jbatnozic
