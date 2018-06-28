#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>
using namespace eosio;
using std::string;

class Users : public eosio::contract {
      using contract::contract;

    public:
      
        Users(account_name self):contract(self) {}

        /// @abi action 
        void adduser(const account_name account, string& username) {
            require_auth(account);
            userIndex users(_self,_self);

            auto itr = users.find(account);

            eosio_assert(itr == users.end(), "Account already exists");

            users.emplace(account, [&](auto& user){
                user.account_name = account;
                user.username = username;
                user.user_tokens = 10;
            });
            
        }

        /// @abi action 
        void updateuser(const account_name account, string& username, uint64_t usertokens) {
            require_auth(account);
            userIndex users(_self,_self);

            auto itr = users.find(account);

            eosio_assert(itr != users.end(), "Account does not exist");

            users.modify(itr, account, [&](auto& user){
                user.username = username;
                user.user_tokens = usertokens;
            });            

        }

        /// @abi action 
        void getuser(const account_name account) {
     
            userIndex users(_self,_self);

            auto itr = users.find(account);
            eosio_assert(itr != users.end(), "Account does not exist");

            auto currentuser = users.get(account);

            print("username is : ", currentuser.username.c_str(), "User Tokens as : ", currentuser.user_tokens);

        }

    private:

        //@abi table user i64
        struct user {
            uint64_t account_name;
            string username;
            uint64_t user_tokens = 10;

            uint64_t primary_key() const { return account_name; }

            EOSLIB_SERIALIZE(user, (account_name)(username)(user_tokens))
        };

        typedef multi_index<N(user), user> userIndex;

};

 EOSIO_ABI(Users, (adduser)(updateuser)(getuser))
