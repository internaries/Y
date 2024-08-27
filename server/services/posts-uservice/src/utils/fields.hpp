#pragma once

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <chrono>

namespace posts_uservice::utils {

#define POSTS_USERVICE_PAGINATION_MIN_PAGE_SIZE 0 
#define POSTS_USERVICE_PAGINATION_DEFAULT_PAGE_SIZE 10
#define POSTS_USERVICE_PAGINATION_MAX_PAGE_SIZE 100

boost::uuids::uuid ParseUUIDArgument(const std::string& uuid_argument, const std::string& field_name = "uuid");
int ParsePaginationSizeArgument(const std::string& size_argument);
std::chrono::system_clock::time_point ParsePaginationArgument(const std::string& page_argument);
}