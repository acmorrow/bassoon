add_executable (encoder_example encoder_example.cpp)
target_link_libraries(encoder_example libbassoon)
install (TARGETS encoder_example DESTINATION bin COMPONENT runtime)
