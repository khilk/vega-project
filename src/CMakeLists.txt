add_library(libraytracer geometry/vector.cpp geometry/geometry.cpp geometry/vector.hpp geometry/geometry.hpp geometry/ray.hpp geometry/sphere.hpp geometry/triangle.hpp
        geometry/intersection.hpp scene/light.hpp scene/material.hpp
        scene/object.hpp scene/reader.hpp scene/reader.cpp scene/scene.hpp
        raytracer/matrix.cpp raytracer/matrix.hpp raytracer/raytracer.hpp raytracer/render_options.hpp
        raytracer/camera_options.hpp raytracer/image.hpp
        raytracer/raytracer.cpp raytracer/image.cpp)

find_package(PNG REQUIRED)
if (PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
    target_link_libraries(libraytracer ${PNG_LIBRARY})
else ()
    message(STATUS "Seems like PNG is not installed on your machine.")
endif ()

find_package(JPEG REQUIRED) # TODO(khilk): add checker + fetch content

target_link_libraries(libraytracer ${PNG_LIBRARY} ${JPEG_LIBRARIES})
target_include_directories(libraytracer PRIVATE ${RT_SOURCE_DIR}/src)
