#PACK_NAME, PACK_ROOT, PACK_VERSION and PACK_CONFIG_DIR are predefined

find_dependency(glm)

#set(${PACK_NAME}_INCLUDE_DIR "${PACK_DIR}/include")
set(${PACK_NAME}_LIB_DIR "${PACK_DIR}/lib")
set(${PACK_NAME}_SHARE_DIR "${PACK_DIR}/share")
set(${PACK_NAME}_CMAKE_DIR "${PACK_DIR}/share/${PACK_NAME}/cmake")
