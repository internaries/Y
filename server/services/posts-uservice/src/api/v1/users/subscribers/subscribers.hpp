#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace posts_uservice {

void AppendGetSubscribers(userver::components::ComponentList& component_list);

}  // namespace posts_uservice
