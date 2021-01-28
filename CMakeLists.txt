cmake_minimum_required (VERSION 3.8)

project("pitch_common")

# Add source to this project's executable.
add_library(${PROJECT_NAME} "card_enums.h"
							"messages.h" "messages.cpp"
							 "message_decoder.h" "message_decoder.cpp" "card_enums.cpp")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
