cmake_minimum_required(VERSION 3.17)
list(APPEND WLAN_TEST_TARGETS query_wlan)

find_package(clog REQUIRED)

foreach(WLAN_TEST_TARGET ${WLAN_TEST_TARGETS})
    add_executable(query_wlan query_wlan.c)
    target_link_libraries(query_wlan PUBLIC xwlan)
	add_test(query_wlan_test query_wlan)
endforeach()
