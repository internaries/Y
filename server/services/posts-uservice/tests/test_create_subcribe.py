# Start via `make test-debug` or `make test-release`

import pytest

from testsuite.databases import pgsql

from utils.error_strings import *

# No test for absent id

FIRST_GUY = '6cca3ced-9615-4e2c-bae4-580c86d78710'
SECOND_GUY = '46775165-7542-49d9-a3b2-a7878980a2bc'

@pytest.mark.pgsql('db', files=['testCreateSubsrcibe.sql'])
async def test_create_subcribe_with_incorrect_id(service_client):
    response = await service_client.post(
        f'/api/v1/users/kek/subscribe',
        headers={'System-Design-User-Id': '123'},
        )
    assert response.status == 400
    assert VALIDATION_ERROR_START in response.text

@pytest.mark.pgsql('db', files=['testCreateSubsrcibe.sql'])
async def test_create_subcribe_with_myself(service_client):
    response = await service_client.post(
        f'/api/v1/users/{FIRST_GUY}/subscribe',
        headers={'System-Design-User-Id': FIRST_GUY}
    )
    assert response.status == 400
    assert VALIDATION_ERROR_START in response.text

@pytest.mark.pgsql('db', files=['testCreateSubsrcibe.sql'])
async def test_create_subcribe_twice(service_client):
    response = await service_client.post(
        f'/api/v1/users/{FIRST_GUY}/subscribe',
        headers={'System-Design-User-Id': SECOND_GUY}
    )
    assert response.status == 200
    assert response.text == "Already subscribed"
@pytest.mark.pgsql('db', files=['testCreateSubsrcibe.sql'])
async def test_create_subcribe_good(service_client):
    response = await service_client.post(
        f'/api/v1/users/{SECOND_GUY}/subscribe',
        headers={'System-Design-User-Id': FIRST_GUY}
    )
    assert response.status == 200
    assert response.text == "Subcribed"





