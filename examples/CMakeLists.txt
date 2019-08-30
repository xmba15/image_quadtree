list(APPEND EXAMPLES
  ExImageQuadtree
)

include(example_utility)

add_definitions(-DIMAGE_PATH="${PROJECT_SOURCE_DIR}/docs/images")

find_package(OpenCV 3 REQUIRED)

list(APPEND PUBLIC_LIBS
  ${PROJECT_TARGET_LIB_NAME}
)

list(APPEND PRIVATE_LIBS
  ${OpenCV_LIBS}
)

foreach(EXAMPLE ${EXAMPLES})
  __build_example(
    ${EXAMPLE}
    "${PUBLIC_LIBS}"
    "${PRIVATE_LIBS}"
  )
  target_include_directories(${EXAMPLE}
    PRIVATE
      ${OpenCV_INCLUDE_DIRS}
  )
endforeach(EXAMPLE)
