#!/bin/bash
psql -U postgres -tc "SELECT 1 FROM pg_database WHERE datname = '${POSTGRES_DB}'" | grep -q 1 || psql -U  ${POSTGRES_USER}  -c "CREATE DATABASE ${POSTGRES_DB}"
psql -U postgres -d ${POSTGRES_DB} -a -f ./db_setup.sql
psql -d ${POSTGRES_DB} -U ${POSTGRES_USER} -c "\COPY ${STR_TABLE}(exercise, reps, work_id, week_id) from '${STR_CSV_TABLE}' with csv"
psql -d ${POSTGRES_DB} -U ${POSTGRES_USER} -c "\COPY ${ENDU_TABLE}(exercise, reps, superset, work_id, week_id) from '${ENDU_CSV_TABLE}' with csv"