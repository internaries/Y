#pragma once

#include <chrono>
#include <stdexcept>
#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace posts_uservice {



void AppendLastPosts(userver::components::ComponentList& component_list);

}  // namespace posts_uservice
