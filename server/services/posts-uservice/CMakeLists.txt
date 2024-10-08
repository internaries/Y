cmake_minimum_required(VERSION 3.12)
project(posts_uservice CXX)


# Adding userver dependency
find_package(userver COMPONENTS core postgresql QUIET)
if(NOT userver_FOUND)  # Fallback to subdirectory usage
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)
    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_STACKTRACE OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_PATCH_LIBPQ OFF CACHE BOOL "" FORCE)


    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/userver)
        message(STATUS "Using userver framework from third_party/userver")
        add_subdirectory(third_party/userver)
    else()
        message(FATAL_ERROR "Either install the userver or provide a path to it")
    endif()
endif()

add_compile_definitions(USERVER_POSTGRES_ENABLE_LEGACY_TIMESTAMP=1)


userver_setup_environment()


# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
    src/api/v1/posts/create_post.hpp
    src/api/v1/posts/create_post.cpp
    src/api/v1/posts/get_post.hpp
    src/api/v1/posts/get_post.cpp
    src/api/v1/users/posts/last_posts.hpp
    src/api/v1/users/posts/last_posts.cpp
    src/api/v1/feed/feed.hpp
    src/api/v1/feed/feed.cpp
    src/api/v1/users/subscribers/subscribers.hpp
    src/api/v1/users/subscribers/subscribers.cpp
    src/api/v1/users/subscriptions/subscriptions.hpp
    src/api/v1/users/subscriptions/subscriptions.cpp
    src/models/s3_url.hpp
    src/models/s3_url.cpp
    src/models/post.hpp
    src/models/post.cpp
    src/models/pagination.hpp
    src/models/pagination.cpp
    src/models/user.hpp
    src/models/user.cpp
    src/utils/fields.hpp
    src/utils/fields.cpp
)
include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::core userver::postgresql)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)


# Unit Tests
add_executable(${PROJECT_NAME}_unittest
    src/api/v1/posts/create_post_test.cpp
    src/models/s3_url_test.cpp
)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
#add_executable(${PROJECT_NAME}_benchmark
#    src/hello_benchmark.cpp
#)
#target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver::ubench)
#add_google_benchmark_tests(${PROJECT_NAME}_benchmark)


# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple()


# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
