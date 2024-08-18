# Start via `make test-debug` or `make test-release`

import pytest

from testsuite.databases import pgsql


@pytest.mark.pgsql('y-testing')
async def test_badId(service_client):
    response = await service_client.get('/api/v1/posts/123')
    assert response.status == 400
    assert response.text == 'Bad Id'

@pytest.mark.pgsql('y-testing')
async def test_absenceId(service_client):
    response = await service_client.get('/api/v1/posts/dd1870ab-fb0d-4b79-a890-ce62a091b77b')
    assert response.status == 404
    assert response.text == 'Id does not exist'

@pytest.mark.pgsql('y-testing')
async def test_goodData(service_client):
    response = await service_client.get('/api/v1/posts/749e5dc8-4c3b-4158-95c1-dc2ff9aab17b')

    expected_data = {
        "id": "749e5dc8-4c3b-4158-95c1-dc2ff9aab17b",
        "text": "Hehe silly",
        "mediaUrl": "https://storage.yandexcloud.net/y-media/cat_cute_blabla.jpg",
        "authorId": "46775165-7542-49d9-a3b2-a7878980a2bc",
        "authorName": "kittycat",
        "authorAvatarUrl": "https://storage.yandexcloud.net/y-media/cat_cute_blabla.jpg",
        "createdAt": "2024-08-17T18:50:20.711473+00:00"
    }


    assert response.status == 200
    assert response.json() == expected_data


    response = await service_client.get('/api/v1/posts/dd1870ab-fb0d-4b79-a890-ce62a091b77a')

    expected_data = {
        "id": "dd1870ab-fb0d-4b79-a890-ce62a091b77a",
        "text": "Just settin up my YYYYYY",
        "mediaUrl": "",
        "authorId": "6cca3ced-9615-4e2c-bae4-580c86d78710",
        "authorName": "cool",
        "authorAvatarUrl": "",
        "createdAt": "2024-08-17T18:29:16.307175+00:00"
    }


    assert response.status == 200
    assert response.json() == expected_data

