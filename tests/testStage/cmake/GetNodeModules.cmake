include_guard()

# Downloads and copies the node_modules to ./EmscriptenStage
function(get_node_modules)
  # Install
  execute_process(COMMAND npm install
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_FILE ${PROJECT_SOURCE_DIR}/node_modules)

  # node_modules
  file(COPY
    ${PROJECT_SOURCE_DIR}/node_modules
    DESTINATION ${PROJECT_SOURCE_DIR}/EmscriptenStage/)
endfunction()

