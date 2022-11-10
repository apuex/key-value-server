if(ENABLE_TESTS)
  set(unit_tests
    data-to-string-test
  )

foreach(unit_test ${unit_tests})
  add_executable("${unit_test}" "tests/${unit_test}.cpp")
  target_include_directories(${unit_test} PRIVATE . ${Boost_INCLUDE_DIRS})
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

