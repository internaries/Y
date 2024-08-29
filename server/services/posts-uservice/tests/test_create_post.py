# Start via `make test-debug` or `make test-release`

import pytest

from testsuite.databases import pgsql

VALIDATION_ERROR_START = 'Failed to validate field'



@pytest.mark.pgsql('db', files=['mockData.sql'])
async def test_create_post_with_bad_id(service_client):
    response = await service_client.post(
        '/api/v1/posts',
        headers = {'System-Design-User-Id': '123'},
        params = {'description': "my first ytweet"})
    assert response.status == 400
    assert response.text == 'Failed to validate field uuid:\n bad lexical cast: source type value could not be interpreted as target\n'

@pytest.mark.pgsql('db', files=['mockData.sql'])
async def test_create_post(service_client):
    description = 'my first ytweet'
    author_id = '46775165-7542-49d9-a3b2-a7878980a2bc'
    expected_data = {'authorId': author_id,
                     'authorName': 'kittycat',
                     'authorAvatarUrl':
                       'https://storage.yandexcloud.net/y-media/cat_cure_blabla.jpg',
                     'description': description}

    response_post = await service_client.post(
        '/api/v1/posts',
        headers = {'System-Design-User-Id': author_id},
        params = {'description': description})
    assert response_post.status == 200

    response_post_json = response_post.json()
    print('\n\n\n')
    print(response_post_json)
    print('\n\n\n')
    for k, v in expected_data.items():
        assert response_post_json[k] == v

    #get-post request

    response_get = await service_client.get(
        '/api/v1/posts/' + response_post_json['id']
    )

    assert response_get.status == 200

    assert response_post_json == response_get.json()
    
    

    


# @pytest.mark.pgsql('db', files=['mockData.sql'])
# async def test_create_with_bad_id(service_client):
#     response = await service_client.post(
#         '/api/v1/posts',
#         headers = {'System-Design-User-Id': '123'},
#         params = {'description': "my first ytweet"})
#     assert response.status == 400
