cmake_minimum_required(VERSION 3.19)
project(ft_ping)

set(CMAKE_CXX_STANDARD 14)

include_directories(includes)

add_executable(ft_ping
        includes/ft_ping.h
        srcs/helpers.c
        srcs/helpers_other.c
        srcs/main.c
        srcs/packet.c
        srcs/parse.c
        srcs/ping.c
        srcs/prepare.c
        srcs/print.c
        srcs/sendrecv.c
        srcs/socket.c)
