#include "feed.hpp"

#include <fmt/format.h>
#include <userver/components/component_context.hpp>

#include <userver/server/handlers/http_handler_base.hpp>

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
    return "I getted feed";
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendGetFeed(userver::components::ComponentList& component_list) { component_list.Append<GetFeed>(); }

}  // namespace posts_uservice
