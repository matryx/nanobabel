@rem -DMINIMAL_BUILD=OFF -DBUILD_GUI=ON -DENABLE_TESTS=ON

@if NOT EXIST build_x64 mkdir build_x64
@cd build_x64

cmake.exe ^
    -DCMAKE_INSTALL_PREFIX=..\install ^
    -G "Visual Studio 15 2017 Win64" ^
    -DLIBXML2_LIBRARIES="%CD%\..\libs\x64\libxml2.lib" ^
    -DMINIMAL_BUILD=OFF ^
    -DLIBXML2_INCLUDE_DIR=. ^
    -DZLIB_LIBRARY="%CD%\..\libs\x64\zlib1.lib" ^
    -DZLIB_INCLUDE_DIR=. ^
    -DINCHI_LIBRARY="%CD%\..\libs\x64\libinchi.lib" ^
    -DINCHI_INCLUDE_DIR=. ^
    -DEIGEN2_INCLUDE_DIR="%EIGEN2_INCLUDE_DIR%" ^
    -DRUN_SWIG=OFF ^
    -DWITH_INCHI=OFF ^
    -DJAVA_BINDINGS=OFF ^
    -DCSHARP_BINDINGS=OFF ^
    -DENABLE_TESTS=OFF ^
    -DBUILD_GUI=OFF ^
    -DwxWidgets_ROOT_DIR="%WXWIN%" ^
    -DwxWidgets_LIB_DIR="%WXWIN%/lib/vc_lib" ^
    -DwxWidgets_CONFIGURATION=msw %1 %2 %3 %4 %5 %6 %7 %8 ..\..

@cd ..
