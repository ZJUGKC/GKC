
SET_TARGET_SUBSYSTEM("Background")

SET_TARGET_PATH_COPY(GkcSys TRUE "platform" "machine" "config" "config" "${PROJECT_SOURCE_DIR}/others/redist" "*" "${LIBRARY_OUTPUT_PATH}")
#setup scripts
FILE(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/_pack")
SET_TARGET_PATH_COPY(GkcSys TRUE "platform" "machine" "no-config" "no-config" "${PROJECT_SOURCE_DIR}/setup" "*" "${PROJECT_BINARY_DIR}/_pack")

SET_TARGET_SRC_PATH("platform" "machine" "user-config" "Debug" "${PROJECT_SOURCE_DIR}/others/redist")
INSTALL(DIRECTORY "${TARGET_SRC_PATH}/"
	DESTINATION core/assemblies COMPONENT runtime
	CONFIGURATIONS Debug)
SET_TARGET_SRC_PATH("platform" "machine" "user-config" "Release" "${PROJECT_SOURCE_DIR}/others/redist")
INSTALL(DIRECTORY "${TARGET_SRC_PATH}/"
	DESTINATION core/assemblies COMPONENT runtime
	CONFIGURATIONS Release)
