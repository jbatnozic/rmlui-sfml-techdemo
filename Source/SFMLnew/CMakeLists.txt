
add_executable("RmlUi.SFMLnew"
    "Main.cpp"
    "RmlUi_SFMLAdapters/RmlUi_Platform_SFML.cpp"
    "RmlUi_SFMLAdapters/RmlUi_Backend_SFML_GL2.cpp"
    "RmlUi_SFMLAdapters/RmlUi_Renderer_GL2.cpp"
    "ShellFileInterface.cpp"
)

target_link_libraries("RmlUi.SFMLnew"
PUBLIC
    "CONAN_PKG::rmlui"
    "CONAN_PKG::sfml"
    "shlwapi"
)

target_include_directories("RmlUi.SFMLnew" PRIVATE .)

