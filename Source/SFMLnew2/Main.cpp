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

#include <iostream>

#include <RmlUi/Core.h>

#include <RmlUi_SFMLAdapters/SFML_system_interface.hpp>
#include <RmlUi_SFMLAdapters/SFML_render_interface.hpp>
#include <RmlUi_SFMLAdapters/RmlUi_Renderer_GL2.hpp>

// #include <RmlUi/Core/Input.h>
#include <RmlUi/Debugger/Debugger.h>
#include <ShellFileInterface.h>

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

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "RmlUi with SFML2");

    if (!window.isOpen()) {
        return 1;
    }

    window.setVerticalSyncEnabled(true);

    sf::View view(sf::FloatRect(0.f, 0.f, (float)window.getSize().x, (float)window.getSize().y));
    window.setView(view);

    // INIT RMLUI

    RmlUiSFMLRenderer renderer;
    // renderer.SetWindow(&window);

    RmlUiSFMLSystemInterface systemInterface;

    // NOTE: if fonts and rml are not found you'll probably have to adjust
    // the path information in the string
    //Rml::String root = "D:\\SYNCHRONIZED\\DEV2\\C++\\rmlui-test\\_Build\\Debug\\bin\\";
    //ShellFileInterface fileInterface(root);

    //Rml::SetFileInterface(&fileInterface);
    Rml::SetRenderInterface(&renderer);
    Rml::SetSystemInterface(&systemInterface);

    RmlGL2::SetViewport(WINDOW_W, WINDOW_H);

    if (!Rml::Initialise()) {
        return 1;
    }

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

    for (const FontFace& face : font_faces)
    {
        Rml::LoadFontFace("assets/" + face.filename, face.fallback_face);
    }

    // CREATE CONTEXT

    Rml::Context* context = Rml::CreateContext("default",
                                               Rml::Vector2i(window.getSize().x, window.getSize().y));

    Rml::Debugger::Initialise(context);

    Rml::ElementDocument* document = context->LoadDocument("assets/demo.rml");

    if (document)
    {
        document->Show();
        fprintf(stdout, "\nDocument loaded");
    }
    else
    {
        fprintf(stdout, "\nDocument is nullptr");
    }

    // MAIN LOOP

    while (window.isOpen())
    {
        // RmlGL2::BeginFrame();

        sf::Event event;

        window.clear();

        sf::CircleShape circle(50.f);
        circle.setPosition(100.f, 100.f);
        circle.setFillColor(sf::Color::Blue);
        circle.setOutlineColor(sf::Color::Red);
        circle.setOutlineThickness(10.f);

        //view.reset(sf::FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y));
        //window.setView(view);

        window.draw(circle);

        updateView(window, view);

        context->Render();
        window.display();

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Resized:
                updateView(window, view);
                break;
            case sf::Event::MouseMoved:
                context->ProcessMouseMove(event.mouseMove.x, event.mouseMove.y,
                                          systemInterface.GetKeyModifiers());
                break;
            case sf::Event::MouseButtonPressed:
                context->ProcessMouseButtonDown(event.mouseButton.button,
                                                systemInterface.GetKeyModifiers());
                break;
            case sf::Event::MouseButtonReleased:
                context->ProcessMouseButtonUp(event.mouseButton.button,
                                              systemInterface.GetKeyModifiers());
                break;
            case sf::Event::MouseWheelMoved:
                context->ProcessMouseWheel(float(-event.mouseWheel.delta),
                                           systemInterface.GetKeyModifiers());
                break;
            case sf::Event::TextEntered:
                if (event.text.unicode > 32)
                    context->ProcessTextInput(Rml::Character(event.text.unicode));
                break;
            case sf::Event::KeyPressed:
                context->ProcessKeyDown(systemInterface.TranslateKey(event.key.code),
                                        systemInterface.GetKeyModifiers());
                break;
            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Num1:
                    multiplier = 2.f;
                    //updateView(window, view);
                    break;
                case sf::Keyboard::Num2:
                    multiplier = 1.f;
                    //updateView(window, view);
                    break;
                case sf::Keyboard::Num3:
                    multiplier = .5f;
                    //updateView(window, view);
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

                context->ProcessKeyUp(systemInterface.TranslateKey(event.key.code),
                                      systemInterface.GetKeyModifiers());
                break;
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            };
        };

        context->Update();
    };

    // END PROGRAM

    Rml::Shutdown();

    return 0;
}
catch (const std::exception& ex) {
    std::cout << "Exception caught: " << ex.what() << std::endl;
}