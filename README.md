# `rubix-imgproc` :camera:
Library/module that processes an image of a Rubik's cube face and produces a 3D matrix of its sides, as much as possible.

## Features
- C++ and Python API/component supported
- ROS node capable
- Intended for use in robotic environments

## Dependencies
- OpenCV 4+
   - specifically, OpenCV `4.13.0` was pulled in at the tiem of repo creation, simply because it was the latest version at the time

### Setup with `w64devkit` / GCC on Windows
When I started this repository, I spent most of my time developing on a Windows laptop. I use GCC that comes with the [`w64devkit`](https://github.com/skeeto/w64devkit) from the venerable Chris Wellons. To get OpenCV installed and used in other projects, the following steps were taken:
0. Install `w64devkit` or equivalent, obviously
1. Install [`cmake`](https://cmake.org/download/), because the OpenCV repository is set up that way, and it'd be a reasonable pain to reverse engineer and simply `make` it
2. Clone the [C++ OpenCV repository](https://github.com/opencv/opencv/)
3. Run CMake to build and install the necessary library files and headers, along with optional documentation.
   - CMake on Windows will generally default to using Visual Studio, and you can also tell it using `-G` to try MinGW. I wanted to point it to GCC in my `w64devkit` installation, so I just had to pass 3 CMake flags that tell it where to look for `gcc`, `g++`, and the installation directory.
   - Additionally, I didn't want to build everything under the sun of OpenCV. This application will at worst do edge/contour and/or corner detection with low resolution JPEG files. So, I simply needed [`core`](https://docs.opencv.org/4.12.0/d0/de1/group__core.html), [`imgproc`](https://docs.opencv.org/4.12.0/d7/dbd/group__imgproc.html), and [`imgcodecs`](https://docs.opencv.org/4.12.0/d4/da8/group__imgcodecs.html). During development, however, I knew I'd also want to display images, so I also wanted [`highgui`](https://docs.opencv.org/4.12.0/d7/dfc/group__highgui.html).
   - Overall, my command was (note that the `-S . -B 'build'` mean I ran this from the root of the repo):
```bash
cmake -DCMAKE_C_COMPILER:FILEPATH=/c/w64devkit/bin/gcc.exe \
      -DCMAKE_CXX_COMPILER:FILEPATH=/c/w64devkit/bin/g++.exe \
      -DCMAKE_INSTALL_PREFIX:FILEPATH=/c/w64devkit \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
      -DBUILD_LIST=core,imgproc,imgcodecs,highgui \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_PERF_TESTS=OFF \
      -DBUILD_opencv_apps=OFF \
      -DBUILD_opencv_python3=OFF \
      -DBUILD_opencv_python2=OFF \
      -DBUILD_opencv_java=OFF \
      -DWITH_QT=OFF \
      -DWITH_OPENGL=OFF \
      -DWITH_OPENCL=OFF \
      -DWITH_CUDA=OFF \
      -DWITH_IPP=OFF \
      -DWITH_TBB=OFF \
      -DWITH_EIGEN=OFF \
      -DWITH_V4L=OFF \
      -DWITH_FFMPEG=OFF \
      -DWITH_GSTREAMER=OFF \
      --no-warn-unused-cli \
      -S . \
      -B 'build' \
      -G "Unix Makefiles"
```

5. Then, to build and install, simply `cmake --build 'build' --config Release --target install`. This took about 15-20 minutes.
6. CMake opted to place its library output files for this in `w64devkit/x64/mingw/lib/` and `/bin` instead of `w64devkit/lib` and `/bin`, and by default, the GCC that comes with `w64devkit` doesn't search for libraries where CMake put them. The solution is just to define or add to your `LIBRARY_PATH` environment variable the list of additional directories to search in.
7. Now, you can use the OpenCV libraries and headers in your application. You simply need to add the `-lopencv_<module_name><version>` list to your final output executable's link command.
