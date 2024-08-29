#include "get_post.hpp"

#include "models/post.hpp"
#include "models/s3_url.hpp"

#include <fmt/format.h>
#include <boost/lexical_cast.hpp>
#include <userver/components/component_context.hpp>

#include <userver/server/handlers/http_handler_base.hpp>

#include "userver/storages/secdist/exceptions.hpp"
#include "utils/errors.hpp"
#include "utils/fields.hpp"

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace posts_uservice {

namespace {
class GetPost final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-get-post";

  GetPost(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                 userver::server::request::RequestContext&) const override {
    auto post_id_as_text = request.GetPathArg("postId");
    boost::uuids::uuid post_id = utils::ParseUUIDArgument(post_id_as_text, "postId");

    auto res = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                    "SELECT id ,description, media_url, author_id, author_name, "
                                    "author_avatar_url, created_at from posts WHERE id = $1",
                                    post_id);
    //change to throw
    if (res.IsEmpty()) {
      throw errors::NotFoundException("user", "User with this id not found");
    }
    auto parsed = res.AsSingleRow<models::PostResponse>(userver::storages::postgres::kRowTag);

    userver::formats::json::ValueBuilder builder(parsed);

    auto& response = request.GetHttpResponse();
    response.SetStatus(userver::server::http::HttpStatus::kOk);
    return ToPrettyString(builder.ExtractValue());
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendGetPost(userver::components::ComponentList& component_list) { component_list.Append<GetPost>(); }

}  // namespace posts_uservice
