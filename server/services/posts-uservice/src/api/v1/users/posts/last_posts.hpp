#pragma once

#include <chrono>
#include <stdexcept>
#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace posts_uservice {

#define POSTS_USERVICE_LAST_POSTS_DEFAULT_PAGE_SIZE 10

int ParseSizeArgument(const std::string& size_argument);
std::chrono::system_clock::time_point ParsePageArgument(const std::string& page_argument);

void AppendLastPosts(userver::components::ComponentList& component_list);

}  // namespace posts_uservice
