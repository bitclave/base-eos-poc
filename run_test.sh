#/bin/bash
docker-compose up -d

docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd wallet create
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd wallet import 5Ji8rDYiuqTAzocRPgz6EdT9szM7ootSBeVDxNm6p2rsUCRn1Yh
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd wallet import 5JmEDUXG88AnikGzov8MZVESaN1zBscYbpiqJrS2SFUedqfUL8c
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd create account eosio hello3 EOS5g1it16cQG9aXZoHdTdG8abk2bA6Mk1ojEnw7bhJWvC28a81gJ EOS5LSwDqyYt9a65ebYATJ2gqXhSxfaAi8Evpn9tz1w81tcD5N7Vy
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd set contract hello3 /mnt/contracts/hello
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd get table hello3 tester keyval
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd create account eosio user EOS5g1it16cQG9aXZoHdTdG8abk2bA6Mk1ojEnw7bhJWvC28a81gJ EOS5g1it16cQG9aXZoHdTdG8abk2bA6Mk1ojEnw7bhJWvC28a81gJ
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd create account eosio tester EOS5g1it16cQG9aXZoHdTdG8abk2bA6Mk1ojEnw7bhJWvC28a81gJ EOS5g1it16cQG9aXZoHdTdG8abk2bA6Mk1ojEnw7bhJWvC28a81gJ
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd push action hello3 addsec '["tester", "tester", "value"]' -p tester
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd push action hello3 addsec '["tester", "123456789012345678901234a", "25 characters aaaa"]' -p tester
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd push action hello3 addsec '["tester", "123456789012345678901234b", "25 characters bbbb"]' -p tester
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd push action hello3 addsec '["user", "123456789012345678901234b", "25 characters bbbb"]' -p user
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd get table hello3 tester keyval
docker-compose exec keosd /opt/eosio/bin/cleos -H nodeosd get table hello3 user keyval

docker-compose down


