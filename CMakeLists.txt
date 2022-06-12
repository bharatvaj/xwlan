cmake_minimum_required(VERSION 3.17)
project(xwlan VERSION 0.0.1)

find_package(clog REQUIRED)

file(GLOB XWLAN_SRCS
	xwlan.c
)

if(ENABLE_NATIVE_WIFI)
	add_compile_definitions(__XWLAN_ENABLE_NATIVE_WIFI__)
	file(GLOB XWLAN_NATIVE_WIFI_SRCS
		private/nativewifi.c
	)
	list(APPEND XWLAN_SRCS ${XWLAN_NATIVE_WIFI_SRCS})
	if(WIN32)
		list(APPEND XWLAN_REQUIRED_LIBS Wlanapi)
	else()
		message("NativeWifi not supported other than Window")
	endif()
endif()

add_library(xwlan ${XWLAN_SRCS})

target_link_libraries(xwlan PUBLIC ${XWLAN_REQUIRED_LIBS})
target_link_libraries(xwlan PUBLIC clog::clog)

target_include_directories(xwlan PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
