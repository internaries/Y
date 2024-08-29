#include "feed.hpp"

#include <fmt/format.h>
#include <userver/components/component_context.hpp>

#include <userver/formats/serialize/common_containers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include "models/pagination.hpp"
#include "models/post.hpp"
#include "userver/storages/secdist/exceptions.hpp"

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace posts_uservice {

namespace {

class GetFeed final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-get-feed";

  GetFeed(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                 userver::server::request::RequestContext&) const override {
    const std::string& user_id_argument = request.GetHeader("System-Design-User-Id");
    const std::string& size_argument = request.GetArg("size");
    const std::string& page_argument = request.GetArg("page");

    models::PaginationRequest pagination_request(user_id_argument, size_argument, page_argument);
    auto res = pagination_request.GetPostsFromDb(pg_cluster_, "feed", "owner_id");

    auto posts = res.AsSetOf<models::PostResponse>(userver::storages::postgres::kRowTag);

    userver::formats::json::ValueBuilder response;
    response["posts"] = posts;
    if (!posts.IsEmpty()) {
      response["nextPage"] = response["posts"][posts.Size() - 1]["createdAt"];
    }
    return userver::formats::json::ToPrettyString(response.ExtractValue());
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendGetFeed(userver::components::ComponentList& component_list) { component_list.Append<GetFeed>(); }

}  // namespace posts_uservice
