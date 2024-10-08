# Start via `make test-debug` or `make test-release`

import pytest

from testsuite.databases import pgsql

from utils.error_strings import *


async def test_bad_id(service_client):
    response = await service_client.get('/api/v1/posts/123')
    assert response.status == 400
    assert VALIDATION_ERROR_START in response.text


async def test_absence_id(service_client):
    response = await service_client.get('/api/v1/posts/dd1870ab-fb0d-4b79-a890-ce62a091b77b')
    assert response.status == 404
    assert NOT_FOUND_ERROR_START in response.text


@pytest.mark.pgsql('db', files=['testGetPost.sql'])
async def test_good_data(service_client):
    response = await service_client.get('/api/v1/posts/749e5dc8-4c3b-4158-95c1-dc2ff9aab17b')

    expected_data = {
        "id": "749e5dc8-4c3b-4158-95c1-dc2ff9aab17b",
        "description": "Hehe silly",
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
        "description": "Just settin up my YYYYYY",
        "mediaUrl": "",
        "authorId": "6cca3ced-9615-4e2c-bae4-580c86d78710",
        "authorName": "cool",
        "authorAvatarUrl": "",
        "createdAt": "2024-08-17T18:29:16.307175+00:00"
    }

    assert response.status == 200
    assert response.json() == expected_data
