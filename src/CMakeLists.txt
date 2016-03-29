set(raft_SRCS
    main.cpp
    servers/raft_server.cpp

    ${PROJECT_SOURCE_DIR}/src/proto/gen_cpp/raft.pb.cc
    ${PROJECT_SOURCE_DIR}/src/proto/gen_cpp/raft.grpc.pb.cc
    )



set(grpc_LIBS
    grpc++
    grpc
    )

set(depend_LIBS
    ${Boost_LIBRARIES}
    ${grpc_LIBS}
    ${PROTOBUF_LIBRARY}
    )


add_custom_command(OUTPUT 
    ${PROJECT_SOURCE_DIR}/src/proto/gen_cpp/raft.pb.cc
    ${PROJECT_SOURCE_DIR}/src/proto/gen_cpp/raft.grpc.pb.cc
    COMMAND echo "GENERATING PROTOBUF FILES"
    COMMAND mkdir -p gen_cpp
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ARGS --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` --cpp_out=gen_cpp --grpc_out=gen_cpp raft.proto
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/proto
    ) 

add_executable(raft ${raft_SRCS})
target_link_libraries(raft ${depend_LIBS})
