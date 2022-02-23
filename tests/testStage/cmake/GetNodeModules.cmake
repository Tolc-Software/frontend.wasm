include_guard()

# Downloads and copies the node_modules to ./EmscriptenStage
function(get_node_modules)
  # node_modules
  file(COPY
    ${PROJECT_SOURCE_DIR}/node_modules
    DESTINATION ${PROJECT_SOURCE_DIR}/EmscriptenStage/)
endfunction()

