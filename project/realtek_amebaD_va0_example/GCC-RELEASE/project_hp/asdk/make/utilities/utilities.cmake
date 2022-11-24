cmake_minimum_required(VERSION 3.6)

project(utilities)

set(dir "${sdk_root}/component/common/utilities")
set(matter_app_dir "${sdk_root}/component/common/application/matter/application")
set(matter_protobuf_dir "${sdk_root}/component/common/application/matter/protobuf")

list(
    APPEND ${list}

    ${dir}/tcptest.c
	${dir}/ssl_client.c
	${dir}/ssl_client_ext.c
	${dir}/http_client.c
	${dir}/xml.c
	${dir}/cJSON.c
    ${matter_app_dir}/matter_dcts.c
    ${matter_app_dir}/matter_timers.c
    ${matter_app_dir}/matter_utils.c
    ${matter_app_dir}/matter_wifis.c
    ${matter_protobuf_dir}/ameba_factory.pb.c
    ${matter_protobuf_dir}/nanopb/pb_common.c
    ${matter_protobuf_dir}/nanopb/pb_decode.c
    ${matter_protobuf_dir}/nanopb/pb_encode.c
)


#target_include_directories(
#    ${target}
#    PUBLIC
#    ${sdk_root}/component/common/network/libcoap/include
#)
