add_subdirectory(lib)

option(IMRT_BUILD_EXAMPLES "Build examples")
set(IMRT_BUILD_EXAMPLES ON)

if(IMRT_BUILD_EXAMPLES)
   add_subdirectory(examples/utility)
   add_subdirectory(examples/oscilloscope)
endif()
