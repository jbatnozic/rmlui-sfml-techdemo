
#include <iostream>

#include <RmlUi/Core.h>

#include <SPeMPE/RmlUi/RmlUi_backend.hpp>
#include <SPeMPE/RmlUi/RmlUi_context_driver.hpp>

#include <RmlUi/Debugger/Debugger.h>

namespace spe = jbatnozic::spempe;

float multiplier = 1.f;

void updateView(sf::RenderWindow& window, sf::View& view)
{
    view.reset(sf::FloatRect(0.f, 0.f, window.getSize().x * multiplier, window.getSize().y * multiplier));
    window.setView(view);
}

int main(int /*argc*/, char** /*argv*/) try {
    // INIT WINDOW

    constexpr int WINDOW_W = 1024;
    constexpr int WINDOW_H = 768;

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Main Window");

    sf::RenderWindow anotherWindow(sf::VideoMode(WINDOW_W / 2, WINDOW_H / 2), "Another Window");

    if (!window.isOpen() || !anotherWindow.isOpen()) {
        return 1;
    }

    window.setVerticalSyncEnabled(true);

    sf::View view(sf::FloatRect(0.f, 0.f, (float)window.getSize().x, (float)window.getSize().y));
    window.setView(view);

    // INIT RMLUI

    auto backendGuard = spe::RmlUiBackend::initialize();

    // LOAD FONTS

    struct FontFace {
        Rml::String filename;
        bool fallback_face;
    };
    FontFace font_faces[] = {
        { "LatoLatin-Regular.ttf",    false },
        { "LatoLatin-Italic.ttf",     false },
        { "LatoLatin-Bold.ttf",       false },
        { "LatoLatin-BoldItalic.ttf", false },
        { "NotoEmoji-Regular.ttf",    true  },
    };

    for (const FontFace& face : font_faces) {
        Rml::LoadFontFace("assets/" + face.filename, face.fallback_face);
    }

    // CREATE CONTEXT

    spe::RmlUiContextDriver context{"context123", window};

    Rml::Debugger::Initialise(&*context);

    Rml::ElementDocument* document = context->LoadDocument("assets/demo.rml");

    if (document) {
        document->Show();
        fprintf(stdout, "\nDocument loaded");
    }
    else {
        fprintf(stdout, "\nDocument is nullptr");
    }

    // MAIN LOOP

    while (window.isOpen() /* && anotherWindow.isOpen()*/)
    {
        static sf::Event event;

        window.clear();
        anotherWindow.clear();

        sf::CircleShape circle(50.f);
        circle.setPosition(100.f, 100.f);
        circle.setFillColor(sf::Color::Blue);
        circle.setOutlineColor(sf::Color::Red);
        circle.setOutlineThickness(10.f);

        window.draw(circle);

        // Render on one window
        context.setRenderTarget(window);
        context.render();

        // Render on another window
        context.setRenderTarget(anotherWindow);
        context.render();

        window.display();
        anotherWindow.display();

        while (window.pollEvent(event)) {
            if (context.processEvent(event)) {
                continue;
            }

            switch (event.type) {
            case sf::Event::Resized:
                updateView(window, view);
                break;

            case sf::Event::KeyReleased:
                switch (event.key.code)    {
                case sf::Keyboard::Num1:
                    multiplier = 2.f;
                    updateView(window, view);
                    break;
                case sf::Keyboard::Num2:
                    multiplier = 1.f;
                    updateView(window, view);
                    break;
                case sf::Keyboard::Num3:
                    multiplier = .5f;
                    updateView(window, view);
                    break;
                case sf::Keyboard::F8:
                    Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
                    break;
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                default:
                    break;
                }
                break;

            case sf::Event::Closed:
                window.close();
                break;

            default:
                break;
            };
        };

        while (anotherWindow.pollEvent(event)) {}

        context.update();
    };

    // END PROGRAM

    return 0;
}
catch (const std::exception& ex) {
    std::cout << "Exception caught: " << ex.what() << std::endl;
}