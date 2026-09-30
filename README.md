
# psp-paf

psp-paf is mostly a header library that provides the declaration of classes and  dynamically links to Sony's `scePaf_module` for Playstation Portable. There are also few headers that recreate stuff that is not getting exported, because there is no clean way to link against it (also it's nice to have source), it's mostly theirs std recreation.

## Building
This will build the static lib and build the samples found in `/samples`
```
psp-cmake -B build -DBUILD_SAMPLES=ON
cmake --build build
```

## Usage
- Build/link against it eg. with CMake
```cmake
add_subdirectory(path/to/psp-paf)
target_link_libraries(name PRIVATE
    psp-paf
)
```

## Samples
You can currently only find one sample:
- xmb-category, it adds new category to XMB although populated items are not usuable in any way as I couldn't figure out any clean, native way to do it (feel free to open the PR)
![XMB category sample](imgs/xmb-category.png)
