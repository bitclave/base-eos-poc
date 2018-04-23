
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/crypto.h>
#include <boost/algorithm/string.hpp>


using namespace eosio;
using namespace std;

typedef uint128_t key_n_t;

class hello : public eosio::contract {
    public:
        using contract::contract;

        hello( account_name self )  
        : contract(self)
        , _keyval(_self, _self)
        , counter(28)
        {}

        int counter;

        // @abi table
        struct keyval {
            uint64_t id;
            key_n_t key_n;
            string key_s;
            string val;

            uint64_t primary_key() const { return id; }
            key_n_t get_key_n() const { return key_n; }

            EOSLIB_SERIALIZE( keyval, (id)(key_n)(key_s)(val) )
        };


        string u64_to_string(name value)const {
            static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";

            string str(13,'.');

            uint64_t tmp = value;
            for( uint32_t i = 0; i <= 12; ++i ) {
                char c = charmap[tmp & (i == 0 ? 0x0f : 0x1f)];
                str[12-i] = c;
                tmp >>= (i == 0 ? 4 : 5);
            }

            boost::algorithm::trim_right_if( str, []( char c ){ return c == '.'; } );
            return str;
        }
        
        static constexpr uint64_t string_to_u64( const char* str ) {

            uint32_t len = 0;
            while( str[len] ) ++len;

            uint64_t value = 0;

            for( uint32_t i = 0; i <= 12; ++i ) {
                uint64_t c = 0;
                if( i < len && i <= 12 ) c = uint64_t(char_to_symbol( str[i] ));

                if( i < 12 ) {
                    c &= 0x1f;
                    c <<= 64-5*(i+1);
                }
                else {
                    c &= 0x0f;
                }

                value |= c;
            }

            return value;
        }

        static const int maxl = 25;
        static const int maxb = 128;

        static constexpr uint128_t string_to_u128( const char* str ) {
 
            uint32_t len = 0;
            while( str[len] ) ++len;
            
            uint128_t value = 0;
            
            for( uint32_t i = 0; i <= maxl; ++i ) {
                uint128_t c = 0;
                if( i < len && i <= maxl ) c = uint64_t(char_to_symbol( str[i] ));
                
                if( i < maxl ) {
                    c &= 0x1f;
                    c <<= maxb-5*(i+1);
                }
                else {
                    c &= 0x07;
                }
                
                value |= c;
            }
            
            return value;
        }

        static string u128_to_string(uint128_t  value) {
            static const char* charmap = ".12345abcdefghijklmnopqrstuvwxyz";
            
            string str(maxl+1,'.');
            
            uint128_t tmp = value;
            for( uint32_t i = 0; i <= maxl; ++i ) {
                char c = charmap[tmp & (i == 0 ? 0x07 : 0x1f)];
                str[maxl-i] = c;
                tmp >>= (i == 0 ? 3 : 5);
            }
            
            boost::trim_right_if( str, []( char c ){ return c == '.'; } );
            return str;
        }

        /// @abi action 
        void addprm( name user, string key, string val, uint64_t id ) {
            print( "add key val for user by primary idx, user = ", user, ": ", getCounter());

            eosio::multi_index<N(keyval), keyval
            , indexed_by< N(by_key_n),   const_mem_fun<keyval, key_n_t, &keyval::get_key_n> >
            > tbl(_self, user);
            
            auto primary_idx = tbl.find(id);
            if (primary_idx!=tbl.end())
            {
                print("key already found = ", key.c_str(), "id= ", primary_idx->id);
                
                tbl.modify( primary_idx, user, [&]( auto& row) {
                    // row.key_n = string_to_u64(key.c_str());
                    row.key_n = string_to_u128(key.c_str());
                    row.key_s = key;
                    row.val = val;
                });

            }
            // get here when need to insert
            else
            {
                print("adding new key  ", key.c_str());
                tbl.emplace( user, [&]( auto& row) {
                    row.id = incCounter();
                    // row.key_n = string_to_u64(key.c_str());
                    row.key_n = string_to_u128(key.c_str());
                    row.key_s = key;
                    row.val = val;
                });
            }
        }

        /// @abi action 
        void addsec( name user, string key, string val ) {
            print( "add key val for user by secondary idx (key), user= ", user, "\n");

            eosio::multi_index<N(keyval), keyval
            , indexed_by< N(by_key_n),   const_mem_fun<keyval, key_n_t, &keyval::get_key_n> >
            > tbl(_self, user);
            // > tbl(_self, _self /*user*/);

            
            auto keyn_idx = tbl.get_index<N(by_key_n)>();
            // auto it = keyn_idx.find(string_to_u64(key.c_str()));
            auto it = keyn_idx.find(string_to_u128(key.c_str()));
            if (it != keyn_idx.end())
            {
                print("key already found = ", key.c_str(), " id= ", it->id);
                auto pkidx = tbl.find(it->id);
                
                tbl.modify( pkidx, user, [&]( auto& row) {
                    row.val = val;
                });
            }
            else 
            {
                print("adding new key  ", key.c_str());
                tbl.emplace( user, [&]( auto& row) {
                    row.id = incCounter();
                    // row.key_n = string_to_u64(key.c_str());
                    row.key_n = string_to_u128(key.c_str());
                    row.key_s = key;
                    row.val = val;
                });
            }
        }

        /// @abi action 
        void remove( name user, uint64_t id ) {
            print("got remove cmd: ", user, "id=  ", id);
            auto it = db_find_i64(_self, /*_self*/user, N(keyval), id);
            if (it >= 0) {
                db_remove_i64(it);
            }
        }

    private:

        multi_index<N(keyval), keyval> _keyval;

        singleton<N(hello3), N(g_counter), N(hello3), uint64_t>  g_counter;

        uint64_t incCounter() {
            uint64_t next = g_counter.exists() ? g_counter.get()+1 : 0;
            g_counter.set(next);
            return next;
        }

        uint64_t getCounter() {
            uint64_t cur = g_counter.exists() ? g_counter.get() : 0;
            return cur;
        }

        
        
};

EOSIO_ABI( hello, (remove)(addprm)(addsec) )    