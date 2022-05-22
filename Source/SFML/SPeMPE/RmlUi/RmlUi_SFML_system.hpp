#pragma once

#include <RmlUi/Core/SystemInterface.h>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

namespace jbatnozic {
namespace spempe {

class RmlUiSFMLSystem : public Rml::SystemInterface
{
public:
    ~RmlUiSFMLSystem() override;

    double GetElapsedTime() override;

    bool LogMessage(Rml::Log::Type aType, const Rml::String& aMessage) override;

    void SetClipboardText(const Rml::String& aText) override;

    void GetClipboardText(Rml::String& aText) override;

private:
    sf::Clock _clock;
};

} // namespace spempe
} // namespace jbatnozic
