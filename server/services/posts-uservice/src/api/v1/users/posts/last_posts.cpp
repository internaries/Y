#include "last_posts.hpp"

#include <fmt/format.h>
#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <stdexcept>
#include <string>
#include <userver/components/component_context.hpp>

#include <userver/formats/json/serialize.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include "models/pagination.hpp"
#include "models/post.hpp"
#include "userver/storages/secdist/exceptions.hpp"
#include "utils/errors.hpp"
#include "utils/fields.hpp"

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/io/row_types.hpp>
#include <userver/storages/postgres/result_set.hpp>
#include <userver/utils/datetime.hpp>

namespace posts_uservice {

namespace {

class LastPosts final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-last-posts";

  LastPosts(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                 userver::server::request::RequestContext&) const override {
    const std::string& user_id_argument = request.GetPathArg("userId");
    const std::string& size_argument = request.GetArg("size");
    const std::string& page_argument = request.GetArg("page");

    models::PaginationRequest pagination_request(user_id_argument, size_argument, page_argument);
    auto res = pagination_request.GetPostsFromDb(pg_cluster_, "posts", "author_id");

    auto response = models::PostsToPaginationJson(res);
    return userver::formats::json::ToPrettyString(response.ExtractValue());
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendLastPosts(userver::components::ComponentList& component_list) { component_list.Append<LastPosts>(); }

}  // namespace posts_uservice
