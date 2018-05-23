#/bin/bash
docker volume create --name=nodeos-data-volume
docker volume create --name=keosd-data-volume

docker-compose up -d

alias cleos='docker-compose exec keosd /opt/eosio/bin/cleos -u http://nodeosd:8888 --wallet-url http://localhost:8888'

cleos wallet create
cleos wallet keys
cleos wallet import 5KL5gTRpwyReTcKbY1qwUWiMBrKaz83iMFvyxaBaMcezWUNMWhi
cleos wallet import 5JK6WvvxrDuyAsAUVhe2oypVjXtRE4kLfSQGrGH8EhGYtRU4LLN
cleos create account eosio hello3 EOS5fsDMbSdBXEgWrUycajXU75gPZ3qkKeffrHcZp2rPdYrMfLyZ2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos set contract hello3 /mnt/contracts/hello
cleos get table hello3 tester keyval
cleos create account eosio user EOS5fsDMbSdBXEgWrUycajXU75gPZ3qkKeffrHcZp2rPdYrMfLyZ2 EOS5fsDMbSdBXEgWrUycajXU75gPZ3qkKeffrHcZp2rPdYrMfLyZ2
cleos create account eosio tester EOS5fsDMbSdBXEgWrUycajXU75gPZ3qkKeffrHcZp2rPdYrMfLyZ2 EOS5fsDMbSdBXEgWrUycajXU75gPZ3qkKeffrHcZp2rPdYrMfLyZ2
cleos push action hello3 addsec '["tester", "tester", "value"]' -p tester
cleos push action hello3 addsec '["tester", "123456789012345678901234a", "25 characters aaaa"]' -p tester
cleos push action hello3 addsec '["tester", "123456789012345678901234b", "25 characters bbbb"]' -p tester
cleos push action hello3 addsec '["user", "123456789012345678901234b", "25 characters bbbb"]' -p user
cleos get table hello3 tester keyval
cleos get table hello3 user keyval

docker-compose down
docker volume rm nodeos-data-volume
docker volume rm keosd-data-volume



