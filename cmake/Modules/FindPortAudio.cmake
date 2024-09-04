# FindPortAudio.cmake
find_path(PORTAUDIO_INCLUDE_DIR portaudio.h)
find_library(PORTAUDIO_LIBRARY portaudio)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PortAudio DEFAULT_MSG PORTAUDIO_LIBRARY PORTAUDIO_INCLUDE_DIR)

if(PORTAUDIO_FOUND)
    set(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARY})
    set(PORTAUDIO_INCLUDE_DIRS ${PORTAUDIO_INCLUDE_DIR})
endif()