#!/bin/bash
psql -U postgres -tc "SELECT 1 FROM pg_database WHERE datname = '${POSTGRES_DB}'" | grep -q 1 || psql -U  ${POSTGRES_USER}  -c "CREATE DATABASE ${POSTGRES_DB}"
psql -U postgres -d ${POSTGRES_DB} -a -f ./db_setup.sql
psql -d ${POSTGRES_DB} -U ${POSTGRES_USER} -c "\COPY ${MENS_TABLE} from '${MENS_CSV_TABLE}' with csv"
psql -d ${POSTGRES_DB} -U ${POSTGRES_USER} -c "\COPY ${WOMENS_TABLE} from '${WOMENS_CSV_TABLE}' with csv"