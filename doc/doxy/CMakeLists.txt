project("ScQtNeuron")

message(HIGHLIGHTED "Configuring  Doxygen docs")
set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL TRUE)


option(TAC_INTERNAL_DOCUMENTATION "Build internal documentation" OFF)
option(TAC_DOC_USE_MATHJAX "Use MathJax for rendering math in HTML docs" ON)

# Set some Doxygen flags
set(TAC_DOXY_PROJECT_NAME             "Dynamic Abstract Neural Computing")
set(MyVersion "0.1.0")
set(MyDate "2026.04.13")

set(TAC_DOXY_OUTPUT_DIRECTORY_SUFFIX  "")
set(TAC_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(TAC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
#set(TAC_DOXY_INPUT "\"${TAC_SOURCE_DIR}/neuron\" \"${TAC_SOURCE_DIR}/doc\"")  #\"${TAC_SOURCE_DIR}/modules/PROC/include\" \"${TAC_SOURCE_DIR}/modules/NEURER/include\" \"${TAC_SOURCE_DIR}/modules/MEMBRANE/include\"
#message(HIGHLIGHTED "from ${TAC_SOURCE_DIR} to ${CMAKE_SOURCE_DIR}/docs")
set(doxyfile_in ${TAC_SOURCE_DIR}/Doxyfile.in [PARENT_SCOPE])

set(TAC_DOXY_HTML_COLORSTYLE_HUE      "220")
set(TAC_DOXY_TAGFILES                 "")

if(TAC_INTERNAL_DOCUMENTATION)
  set(TAC_DOXY_INTERNAL                 "YES")
else(TAC_INTERNAL_DOCUMENTATION)
  set(TAC_DOXY_INTERNAL                 "NO")
endif(TAC_INTERNAL_DOCUMENTATION)

if (TAC_DOC_USE_MATHJAX)
  set(TAC_DOXY_USE_MATHJAX "YES")
else ()
  set(TAC_DOXY_USE_MATHJAX "NO")
endif()


configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/dynsections.js
  ${CMAKE_SOURCE_DIR}/../docs/doxy/dynsections.js
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
)

configure_file(
  ${TAC_SOURCE_DIR}/TACdoxy_header.html.in
  ${CMAKE_SOURCE_DIR}/../docs/doxy/TACdoxy_header.html
)

configure_file(
  ${TAC_SOURCE_DIR}/TACdoxy_footer.html.in
  ${CMAKE_SOURCE_DIR}/../docs/doxy/TACdoxy_footer.html
)

configure_file(
  ${TAC_SOURCE_DIR}/TACdoxy_layout.xml.in
  ${CMAKE_SOURCE_DIR}/../docs/doxy/TACdoxy_layout.xml
)

configure_file(
  ${TAC_SOURCE_DIR}/TACdoxy.css.in
  ${CMAKE_SOURCE_DIR}/../docs/doxy/TACdoxy.css
)

configure_file(
  ${TAC_SOURCE_DIR}/mycode.js
  ${CMAKE_SOURCE_DIR}/../docs/doxy/mycode.js
)

# Create references
# https://stackoverflow.com/questions/43191252/references-to-latex-equations-using-doxygen
if (UNIX OR CYGWIN)
  set( LATEX_CMD_NAME "${CMAKE_CURRENT_SOURCE_DIR}/latex2")
else()
  set( LATEX_CMD_NAME "${CMAKE_CURRENT_SOURCE_DIR}/latex2.bat")
endif()


add_definitions("-DTAC_MAKING_DOCS")

add_custom_target(
  doc-TAC-prerequisites
  ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/html/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/TAC_navtree_hacks.js           ${CMAKE_CURRENT_BINARY_DIR}/html/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/images/TAC_Logo.png  ${CMAKE_CURRENT_BINARY_DIR}/html/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ftv2pnode.png                    ${CMAKE_CURRENT_BINARY_DIR}/html/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ftv2node.png                     ${CMAKE_CURRENT_BINARY_DIR}/html/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/TACdoxy.css          ${CMAKE_CURRENT_BINARY_DIR}/html/
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(doc ALL
  COMMAND doxygen
#  COMMAND ls ../docs
#  COMMAND ${CMAKE_COMMAND} -E rename ../docs TAC-doc
#  COMMAND ${CMAKE_COMMAND} -E remove TAC-doc/TAC-doc.tgz
#  COMMAND ${CMAKE_COMMAND} -E tar cfz TAC-doc.tgz TAC-doc
#  COMMAND ${CMAKE_COMMAND} -E rename TAC-doc.tgz TAC-doc/TAC-doc.tgz
#  COMMAND ${CMAKE_COMMAND} -E rename TAC-doc ../docs
#  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

