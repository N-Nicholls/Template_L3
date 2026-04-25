# groups.cmake

# group Source Group 1
add_library(Group_Source_Group_1 OBJECT
  "${SOLUTION_ROOT}/src/startup_stm32l476xx.s"
  "${SOLUTION_ROOT}/src/main.c"
  "${SOLUTION_ROOT}/src/LED.c"
  "${SOLUTION_ROOT}/src/SysTimer.c"
)
target_include_directories(Group_Source_Group_1 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  "${SOLUTION_ROOT}/src"
)
target_compile_definitions(Group_Source_Group_1 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_Source_Group_1_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_Source_Group_1_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_Source_Group_1 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Source_Group_1 PUBLIC
  Group_Source_Group_1_ABSTRACTIONS
)
set(COMPILE_DEFINITIONS
  STM32L476xx
  _RTE_
)
cbuild_set_defines(AS_ARM COMPILE_DEFINITIONS)
set_source_files_properties("${SOLUTION_ROOT}/src/startup_stm32l476xx.s" PROPERTIES
  COMPILE_FLAGS "${COMPILE_DEFINITIONS}"
)
