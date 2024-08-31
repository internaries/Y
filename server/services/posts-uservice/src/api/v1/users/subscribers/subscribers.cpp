#include "subscribers.hpp"

#include <fmt/format.h>
#include <boost/uuid/uuid.hpp>
#include <userver/components/component_context.hpp>

#include <userver/formats/serialize/common_containers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include "models/user.hpp"
#include "userver/storages/secdist/exceptions.hpp"
#include "utils/errors.hpp"
#include "utils/fields.hpp"

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/cluster_types.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/io/row_types.hpp>
#include <userver/storages/postgres/postgres_fwd.hpp>

namespace posts_uservice {

namespace {

class GetSubscribers final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-get-subscribers";

  GetSubscribers(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                 userver::server::request::RequestContext&) const override {
    // can be useful for hide subscriptions for someone
    const std::string& user_authorized_id_argument = request.GetHeader("System-Design-User-Id");
    const std::string& user_id_argument = request.GetPathArg("userId");

    const auto user_authorized_id = utils::ParseUUIDArgument(user_id_argument);
    const auto user_id = utils::ParseUUIDArgument(user_id_argument);

    auto user_exists = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                            "SELECT name FROM users WHERE id = $1", user_id);

    if (user_exists.IsEmpty()) {
      throw errors::NotFoundException("user", "User with this id not found");
    }

    auto res = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                    "SELECT folower_id FROM follows WHERE folowee_id = $1", user_id);

    auto users = res.AsSetOf<models::UserResponse>(userver::storages::postgres::kRowTag);

    userver::formats::json::ValueBuilder response;
    response["users"] = users;

    return userver::formats::json::ToPrettyString(response.ExtractValue());
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendGetSubscribers(userver::components::ComponentList& component_list) {
  component_list.Append<GetSubscribers>();
}

}  // namespace posts_uservice
