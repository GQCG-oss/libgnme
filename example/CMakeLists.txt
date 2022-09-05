set(EXAMPLES
    overlap
)

foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.C)
    target_link_libraries(${EXAMPLE} gnme armadillo BLAS::BLAS LAPACK::LAPACK)
endforeach()
