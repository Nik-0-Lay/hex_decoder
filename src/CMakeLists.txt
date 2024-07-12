set(DECODER
    decoder/decoder.h
    decoder/decoder.cpp
    decoder/drofa_protocol.h
    decoder/drofa_protocol.cpp
    decoder/drofa_protocol.ui
    decoder/korshun_protocol.h
    decoder/korshun_protocol.cpp
    decoder/korshun_protocol.ui
    decoder/window.h
    decoder/window.cpp
    decoder/window.ui
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${DECODER})

qt_add_executable(decoder)

target_sources(decoder 
    PRIVATE
    main.cpp
    ${DECODER}
)

target_include_directories(decoder PRIVATE  
    ${CMAKE_CURRENT_LIST_DIR}/decoder 
)

target_link_libraries(decoder PUBLIC 
    Qt6::Widgets
)

if(MSVC)
    set_target_properties(decoder PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BIN_PATH})
endif(MSVC)

#  Call windeployqt with current build configuration directory as argument
add_custom_command(
  TARGET decoder
  POST_BUILD
  COMMAND
    CD $<$<CONFIG:Debug>:${BIN_DBG}>$<$<CONFIG:Release>:${BIN_REL}> &&
    ${WINDEPLOYQT_EXECUTABLE} decoder.exe
       --verbose 1 --dir . --plugindir plugins --no-translations --compiler-runtime
)