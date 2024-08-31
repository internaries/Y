# Start via `make test-debug` or `make test-release`

import pytest

from testsuite.databases import pgsql

from utils.error_strings import *


@pytest.mark.pgsql('db', files=['testGetSubscriptionsOrSubscribers.sql'])
async def test_get_subscriptions_with_bad_id(service_client):
    response = await service_client.get(
        '/api/v1/users/123/subscriptions',
        headers={'System-Design-User-Id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'})
    assert response.status == 400
    assert VALIDATION_ERROR_START in response.text


@pytest.mark.pgsql('db', files=['testGetSubscriptionsOrSubscribers.sql'])
async def test_get_subscriptions_with_doesnt_exist_user(service_client):
    response = await service_client.get(
        '/api/v1/users/c25a7e41-8a27-48ca-bbaf-dc27a000b9bc/subscriptions',
        headers={'System-Design-User-Id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'})
    assert response.status == 404
    assert NOT_FOUND_ERROR_START in response.text


@pytest.mark.pgsql('db', files=['testGetSubscriptionsOrSubscribers.sql'])
async def test_get_mine_subscriptions(service_client):
    response = await service_client.get(
        '/api/v1/users/c25a7e41-8a27-48ca-bbaf-dc27a000b9ba/subscriptions',
        headers={'System-Design-User-Id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'})
    assert response.status == 200

    expected_data = {'users':
                     [{'id': '267d54d2-8956-4045-9885-1bf238fc90ce'}]}

    assert expected_data == response.json()


@pytest.mark.pgsql('db', files=['testGetSubscriptionsOrSubscribers.sql'])
async def test_get_not_mine_subscriptions(service_client):
    response = await service_client.get(
        '/api/v1/users/1e755517-eea9-414a-acb1-7a84600898bd/subscriptions',
        headers={'System-Design-User-Id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'})
    assert response.status == 200

    expected_data = {'users':
                     [{'id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'},
                      {'id': '267d54d2-8956-4045-9885-1bf238fc90ce'}]}

    assert expected_data == response.json()

    response = await service_client.get(
        '/api/v1/users/267d54d2-8956-4045-9885-1bf238fc90ce/subscriptions',
        headers={'System-Design-User-Id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'})
    assert response.status == 200

    expected_data = {'users':
                     [{'id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'},
                      {'id': 'eacea82c-b498-4545-8db2-04895f194962'}]}

    assert expected_data == response.json()

    response = await service_client.get(
        '/api/v1/users/eacea82c-b498-4545-8db2-04895f194962/subscriptions',
        headers={'System-Design-User-Id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'})
    assert response.status == 200

    expected_data = {'users':
                     [{'id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'}]}

    assert expected_data == response.json()

    response = await service_client.get(
        '/api/v1/users/ab573c05-d6c8-4924-aa1a-3f2fd1cea121/subscriptions',
        headers={'System-Design-User-Id': 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'})
    assert response.status == 200

    expected_data = {'users': []}

    assert expected_data == response.json()
