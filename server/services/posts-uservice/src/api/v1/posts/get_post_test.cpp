#include "get_post.hpp"

#include <userver/utest/utest.hpp>

UTEST(url,S3) {

    posts_uservice::S3_url data{"bucket","key"};
    EXPECT_EQ(posts_uservice::ToLink(data),"https://storage.yandexcloud.net/" + data.bucket + "/" + data.key);

}