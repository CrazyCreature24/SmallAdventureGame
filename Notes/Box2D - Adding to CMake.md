#Box2D[[Box2D - Overview]]

The Box2D library comes setup for CMake

In the root of the Box2D repo (https://github.com/erincatto/box2d) is a cmakelists.txt that will build the library along with some test projects

We're only interested in the Box2D library itself, that's contained in: **src/CMakeLists.txt**

To include this CMakeLists file in ours, add the following after the *project* block in our cmake file:
```CMake
include( GNUInstallDirs )
add_subdirectory( Framework/Libraries/box2D/src )
```

Finally, add *box2d* to our *target_link_libraries* list
