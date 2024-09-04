# FindFFTW.cmake
find_path(FFTW_INCLUDE_DIR fftw3.h)
find_library(FFTW_LIBRARY fftw3)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFTW DEFAULT_MSG FFTW_LIBRARY FFTW_INCLUDE_DIR)

if(FFTW_FOUND)
    set(FFTW_LIBRARIES ${FFTW_LIBRARY})
    set(FFTW_INCLUDE_DIRS ${FFTW_INCLUDE_DIR})
endif()