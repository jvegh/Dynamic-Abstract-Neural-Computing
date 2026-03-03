set(ENV{GIT_DIR} ${CMAKE_SOURCE_DIR}/.git)
execute_process(COMMAND git describe --tags --exclude "continuous" 
                OUTPUT_VARIABLE GIT_REV
                ERROR_QUIET)

# Check whether we got any revision (which isn't
# always the case, e.g. when someone downloaded a zip
# file from Github instead of a checkout)
if ("${GIT_REV}" STREQUAL "")
    set(PROJECT_GIT_VERSION "N/A" PARENT SCOPE)
    message(WARNING "Maybe .git drive was not on when started")
else()
    string(STRIP "${GIT_REV}" PROJECT_GIT_VERSION)
endif()

string(FIND ${PROJECT_GIT_VERSION} "-" GITVERSION_LENGTH)
set(GITVERSIONLENGTH 9)
string(SUBSTRING ${PROJECT_GIT_VERSION} 0 ${GITVERSION_LENGTH} PROJECT_GIT_VERSION)
set(VERSION_FILE ${CMAKE_SOURCE_DIR}/modules/include/gen_versionnumber.h)
string(TIMESTAMP GITVERSION_TIMESTAMP)
message(highlighted ${GITVERSION_TIMESTAMP} )
string(FIND ${GITVERSION_TIMESTAMP} "T" GITVERSION_TIMESTAMPLENGTH)
set(GITVERSION_TIMESTAMPLENGTH 9)
string(SUBSTRING ${GITVERSION_TIMESTAMP} 0 ${GITVERSION_TIMESTAMPLENGTH} GITVERSION_TIMESTAMP)
message(highlighted ${GITVERSION_TIMESTAMP} )
set(PROJECT_TIMESTAMP ${GITVERSION_TIMESTAMP})
