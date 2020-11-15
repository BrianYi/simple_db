cmake_minimum_required (VERSION 3.10)
project (simple_db)
add_subdirectory(src bin)
# add_subdirectory(include)
# add_subdirectory(bin)

install(FILES COPYRIGHT README DESTINATION share/doc/simple_db)
install(PROGRAMS simple_db DESTINATION bin)
install(DIRECTORY doc/ DESTINATION share/doc/simple_db)
