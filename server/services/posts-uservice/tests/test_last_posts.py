import pytest

from testsuite.databases import pgsql

from utils.error_strings import VALIDATION_ERROR_START


async def get_last_posts(service_client, user_id, size='', page=''):
    return await service_client.get(f'api/v1/users/{user_id}/posts', params={'size': size, 'page': page})


def validate_posts(posts, excpectedName, expectedSize=10, expected_count=0):
    assert len(posts) == expectedSize

    count = expected_count

    for post in posts:
        newCount = int(posts[0]['text'])

        assert newCount - 1 == count
        assert post['authorName'] == excpectedName


@pytest.mark.pgsql('db', files=['testLastPosts.sql'])
async def test_default(service_client):
    response = await get_last_posts(service_client, 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba')
    assert response.status == 200

    data = response.json()
    validate_posts(data['posts'], 'user1')


@pytest.mark.pgsql('db', files=['testLastPosts.sql'])
async def test_different_size(service_client):
    response = await get_last_posts(service_client, 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 5)
    assert response.status == 200

    data = response.json()
    validate_posts(data['posts'], 'user1', 5)


@pytest.mark.pgsql('db', files=['testLastPosts.sql'])
async def test_pagination(service_client):
    response = await get_last_posts(service_client, 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 3)
    assert response.status == 200

    data = response.json()
    posts = data['posts']

    validate_posts(posts, 'user1', 3)

    lastCount = int(posts[len(posts) - 1]['text'])

    print(data['nextPage'])
    response = await get_last_posts(service_client, 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 3, data['nextPage'])
    assert response.status == 200

    data = response.json()
    posts = data['posts']

    validate_posts(posts, 'user1', 3, lastCount)


@pytest.mark.pgsql('db')
async def test_bad_id(service_client):
    response = await get_last_posts(service_client, 'bla-bla', 3)
    assert response.status == 400
    assert VALIDATION_ERROR_START in response.text


@pytest.mark.pgsql('db')
async def test_bad_page(service_client):
    response = await get_last_posts(service_client, 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', None, "bla-bla")
    assert response.status == 400
    assert VALIDATION_ERROR_START in response.text
