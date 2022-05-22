
add_executable("RmlUi.SFML"
    "Main.cpp"

    "SPeMPE/RmlUi/RmlUi_backend.cpp"
    "SPeMPE/RmlUi/RmlUi_context_driver.cpp"
    "SPeMPE/RmlUi/RmlUi_SFML_renderer.cpp"
    "SPeMPE/RmlUi/RmlUi_SFML_system.cpp"
)

target_link_libraries("RmlUi.SFML"
PUBLIC
    "CONAN_PKG::glew"
    "CONAN_PKG::rmlui"
    "CONAN_PKG::sfml"
)

target_include_directories("RmlUi.SFML" PRIVATE .)

