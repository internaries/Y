#include "pagination.hpp"
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/result_set.hpp>
#include "models/post.hpp"

namespace posts_uservice::models {
userver::storages::postgres::ResultSet PaginationRequest::GetPostsFromDb(
    const userver::storages::postgres::ClusterPtr& db, const std::string& table, const std::string& search_id) {
  if (!page_token.has_value()) {
    return db->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                       fmt::format("SELECT {} from {} WHERE {} = $1 "
                                   "ORDER BY created_at DESC "
                                   "LIMIT $2 ",
                                   models::kPostResponseFields, table, search_id),
                       user_id, size);

  } else {
    return db->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                       fmt::format("SELECT {} from {} WHERE {} = $1 "
                                   "AND created_at < $3 "
                                   "ORDER BY created_at DESC "
                                   "LIMIT $2 ",
                                   models::kPostResponseFields, table, search_id),
                       user_id, size, page_token.value());
  }
}

}  // namespace posts_uservice::models