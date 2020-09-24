project(BRL-CAD)

cmake_minimum_required(VERSION 3.0)

add_executable(lemon lemon.c)
install(TARGETS lemon DESTINATION bin)

include(CPack)

