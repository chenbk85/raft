set(raft_SRCS
    main.cpp
    )


set(depend_LIBS
    ${Boost_LIBRARIES}
    ${PROTOBUF_LIBRARY}
    )

add_executable(raft ${raft_SRCS})
target_link_libraries(raft ${depend_LIBS})
