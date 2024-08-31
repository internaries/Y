#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace posts_uservice {

void AppendGetSubscriptions(userver::components::ComponentList& component_list);

}  // namespace posts_uservice
