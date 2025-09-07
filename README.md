# Goob 2

After two years I want to reimplement my old unfinished project "[Goob](https://github.com/schrodlm/goob_the_renderer)" - a simple rendering engine based on https://github.com/ssloy/tinyrenderer. I want to see if I can come up with a better architecture and actually finish the project.

## Project structure
```
goob/
├── build/
├── src/
│   ├── image/
│   │   ├── TGAImage.cpp
│   │   ├── ...
│   │   └── CMakeLists.txt
│   ├── renderer/
│   │   ├── goob.cpp
│   │   ├── ...
│   │   └── CMakeLists.txt
│   └── CMakeLists.txt
├── include/
│   ├── image/
│   │   ├── TGAImage.hpp
│   │   └── ...
│   ├── renderer/
│   │   ├── goob.hpp
│   │   └── ...
│   └── CMakeLists.txt
├── test/
├── docs/
├── assets/
├── sandbox/
│   ├── main.cpp
│   └── CMakeLists.txt
└── CMakeLists.txt
```
