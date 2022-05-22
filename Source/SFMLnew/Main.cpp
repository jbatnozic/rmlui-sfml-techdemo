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

//#include <GL/glew.h>

#include <RmlUi/Core.h>

#include <RmlUi_SFMLAdapters/RmlUi_Backend.h>
#include <RmlUi_SFMLAdapters/RmlUi_Platform_SFML.h>
#include <RmlUi_SFMLAdapters/RmlUi_Renderer_GL2.h>

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

    if (!Backend::InitializeInterfaces()) {
        return 1;
    }

    if (!Rml::Initialise()) {
        return 1;
    }

    Backend::OpenWindow("RmlUi + SFML", WINDOW_W, WINDOW_H, true);

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
        Rml::Vector2i(WINDOW_W, WINDOW_H));

    Rml::Debugger::Initialise(context);
    Backend::SetContext(context);

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

    while (!sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
    {
        Backend::BeginFrame();

        Backend::PresentFrame();

        Backend::EventLoop(
            []() {
                Backend::RequestExit();
            });
        
        context->Update();
    };

    // END PROGRAM

    Rml::Shutdown();

    Backend::ShutdownInterfaces();

    return 0;
}
catch (const std::exception& ex) {
    std::cout << "Exception caught: " << ex.what() << std::endl;
}