cmake_minimum_required(VERSION 3.22)
project(
  TIC_TAC_TOE
  VERSION 1
  DESCRIPTION "Game Tic-tac-toe"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra -Wpedantic")

set(SOURCES
    main.c
    server.c
    client.c
    game_logic.c
)

add_executable(TIC_TAC_TOE ${SOURCES})