
#include <SPeMPE/RmlUi/RmlUi_SFML_system.hpp>

#include <iostream> // TODO Temp.

namespace jbatnozic {
namespace spempe {

RmlUiSFMLSystem::~RmlUiSFMLSystem() = default;

double RmlUiSFMLSystem::GetElapsedTime() {
    return _clock.getElapsedTime().asSeconds();
}

bool RmlUiSFMLSystem::LogMessage(Rml::Log::Type aType, const Rml::String& aMessage) {
    // TODO Temp.
    std::cout << aMessage << std::endl;
    return true;
}

void RmlUiSFMLSystem::SetClipboardText(const Rml::String& aText) {
    sf::Clipboard::setString(
        sf::String::fromUtf8(aText.begin(), aText.end())
    );
}

void RmlUiSFMLSystem::GetClipboardText(Rml::String& aText) {
    aText = sf::Clipboard::getString().toAnsiString(); // TODO should I use toUtf8 instead?
}

} // namespace spempe
} // namespace jbatnozic
