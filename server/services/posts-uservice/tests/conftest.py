import pathlib

import pytest

from testsuite.databases.pgsql import discover

pytest_plugins = ['pytest_userver.plugins.core',
                  'pytest_userver.plugins.postgresql']


@pytest.fixture(scope='session')
def service_source_dir():
    """Path to root directory service."""
    return pathlib.Path(__file__).parent.parent


@pytest.fixture(scope='session')
def initial_data_path(service_source_dir):
    """Path for find files with data"""
    return [
        service_source_dir / '../../databases/core-pgsql/data',
    ]


@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
    """Create schemas databases for tests"""
    databases = discover.find_schemas(
        'posts_uservice',  # service name that goes to the DB connection
        [service_source_dir.joinpath('../../databases/core-pgsql/schemas')],
    )
    return pgsql_local_create(list(databases.values()))
