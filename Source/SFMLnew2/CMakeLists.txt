
add_executable("RmlUi.SFMLnew2"
    "Main.cpp"
    "RmlUi_SFMLAdapters/RmlUi_Renderer_GL2.cpp"
    "RmlUi_SFMLAdapters/SFML_render_interface.cpp"
    "RmlUi_SFMLAdapters/SFML_system_interface.cpp"
    "ShellFileInterface.cpp"
)

target_link_libraries("RmlUi.SFMLnew2"
PUBLIC
    "CONAN_PKG::rmlui"
    "CONAN_PKG::sfml"
    "shlwapi"
)

target_include_directories("RmlUi.SFMLnew2" PRIVATE .)

