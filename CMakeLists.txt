cmake_minimum_required(VERSION 3.0.0) 
project(hangman, VERSION 1.0)


add_executable(hangman)
target_sources(hangman
  PRIVATE
  src/main.cpp
)
