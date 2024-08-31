#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "api/v1/feed/feed.hpp"
#include "api/v1/posts/create_post.hpp"
#include "api/v1/posts/get_post.hpp"
#include "api/v1/users/posts/last_posts.hpp"
#include "api/v1/users/subscribers/subscribers.hpp"
#include "api/v1/users/subscriptions/subscriptions.hpp"

int main(int argc, char* argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::components::Postgres>("postgres-db-1")
                            .Append<userver::clients::dns::Component>()
                            .Append<userver::server::handlers::TestsControl>();

  posts_uservice::AppendCreatePost(component_list);
  posts_uservice::AppendGetPost(component_list);
  posts_uservice::AppendLastPosts(component_list);
  posts_uservice::AppendGetFeed(component_list);
  posts_uservice::AppendGetSubscribers(component_list);
  posts_uservice::AppendGetSubscriptions(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
