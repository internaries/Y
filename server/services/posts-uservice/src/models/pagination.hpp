#pragma once

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <optional>
#include <string>
#include <userver/formats/json/value_builder.hpp>
#include <userver/storages/postgres/postgres_fwd.hpp>

#include "utils/fields.hpp"

namespace posts_uservice::models {
struct PaginationRequest {
  boost::uuids::uuid user_id;
  int size;
  std::optional<std::chrono::system_clock::time_point> page_token;

  PaginationRequest(const std::string& user_id_argument, const std::string& size_argument,
                    const std::string& page_argument)
      : user_id(utils::ParseUUIDArgument(user_id_argument)),
        size(utils::ParsePaginationSizeArgument(size_argument)),
        page_token(utils::ParsePaginationArgument(page_argument)) {}

  userver::storages::postgres::ResultSet GetPostsFromDb(const userver::storages::postgres::ClusterPtr& db,
                                                        const std::string& table, const std::string& search_id);
};

userver::formats::json::ValueBuilder PostsToPaginationJson(const userver::storages::postgres::ResultSet& db_posts);
}  // namespace posts_uservice::models