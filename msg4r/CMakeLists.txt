SET(MSG4R_SRCS
  msg4r.hpp
  msg4r.cpp
)

ADD_LIBRARY(msg4r STATIC ${MSG4R_SRCS})
set_target_properties(msg4r PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

install(FILES
  msg4r.hpp
  DESTINATION include
  CONFIGURATIONS Release
  COMPONENT libraries)

INSTALL(TARGETS
    msg4r 
    CONFIGURATIONS Release
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    COMPONENT libraries)

if(ENABLE_TESTS)
  set(unit_tests
    msg4r-binary-test
    msg4r-compiler-test
    msg4r-endianness-test
    msg4r-fail-test
    msg4r-inheritance-test
    msg4r-int-test
    msg4r-istream-test
    msg4r-json-test
    msg4r-list-test
    msg4r-map-test
    msg4r-number-parser-test
    msg4r-pair-test
    msg4r-poc-int-test
    msg4r-set-test
    msg4r-shared-ptr-test
    msg4r-state-test
    msg4r-streambuf-test
    msg4r-string-test
    msg4r-struct1-test
    msg4r-struct2-test
    msg4r-template-test
    msg4r-trump-test
    msg4r-vector-test
  )

foreach(unit_test ${unit_tests})
  add_executable("${unit_test}" "tests/${unit_test}.cpp")
  target_include_directories(${unit_test} PRIVATE ${Boost_INCLUDE_DIRS})
  target_link_libraries(${unit_test} msg4r ${Boost_LIBRARIES} ${Boost_unit_test_framework})
  if(UNIX)
    set_target_properties("${unit_test}" PROPERTIES LINK_FLAGS_RELEASE -s)
  else(UNIX)
  endif(UNIX)

  INSTALL(TARGETS
    "${unit_test}"
    CONFIGURATIONS Release
    RUNTIME DESTINATION bin
    COMPONENT unittests)

  if(ENABLE_VALGRIND)
    if(MEMORYCHECK_COMMAND)
      add_test(NAME "${unit_test}"
        COMMAND "${MEMORYCHECK_COMMAND}"
       	        "${MEMORYCHECK_COMMAND_OPTIONS}"
                "${CMAKE_CURRENT_BINARY_DIR}/${unit_test}"
      )
    else(MEMORYCHECK_COMMAND)
      add_test(NAME "${unit_test}"
        COMMAND "./${unit_test}"
      )
    endif(MEMORYCHECK_COMMAND)
  else(ENABLE_VALGRIND)
    add_test(NAME "${unit_test}"
      COMMAND "./${unit_test}"
    )
  endif(ENABLE_VALGRIND)
endforeach()

endif(ENABLE_TESTS)

