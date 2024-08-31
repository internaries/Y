#include "subscriptions.hpp"

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

class GetSubscriptions final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-get-subscriptions";

  GetSubscriptions(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                 userver::server::request::RequestContext&) const override {
    // can be useful for hide subscriptions for someone
    const std::string& user_authorized_id_argument = request.GetHeader("System-Design-User-Id");
    const std::string& user_id_argument = request.GetPathArg("userId");

    auto user_authorized_id = utils::ParseUUIDArgument(user_id_argument);
    auto user_id = utils::ParseUUIDArgument(user_id_argument);
    
    return "something";
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendGetSubscriptions(userver::components::ComponentList& component_list) { component_list.Append<GetSubscriptions>(); }

}  // namespace posts_uservice
