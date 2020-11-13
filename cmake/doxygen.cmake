find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(doxyfile_in ${FP256_DOC_DIR}/Doxyfile.in)
    set(doxyfile ${FP256_DOC_DIR}/Doxyfile)
    set(mainpagefile ${FP256_DOC_DIR}/mainpage.md ${FP256_DOC_DIR}/intro.md)
    if(EXISTS ${doxyfile_in})
        configure_file(${doxyfile_in} ${doxyfile} @ONLY)
        add_custom_target(
            doc 
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
            DEPENDS ${doxyfile} ${mainpagefile})
    else()
        message(WARNING "doxyfile.in not found.")
    endif()
else()
    message(WARNING "doxygen not found.")
endif()
