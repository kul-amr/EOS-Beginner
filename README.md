# EOS-Beginner

(OS- Ubuntu 18.04 LTS)

EOS dApp dev steps:

Setup : 

1. Clone the EOS git repo 

  	git clone https://github.com/EOSIO/eos.git --recursive

2. Navigate to the eos folder and execute eosio_build.sh
  
  	sh eosio_build.sh darwin
  
  (While executing this, I faced below errors
  ./eosio_build.sh: 33: ./eosio_build.sh: Bad substitution
  ./eosio_build.sh: 43: ./eosio_build.sh: Syntax error: "(" unexpected
  
  The reason was default shell (/bin/sh) under Ubuntu was pointing to dash, not bash. You can also check that with command 
  
  	readlink -f $(which sh)
  
  When I executed this, it had shown  "/bin/dash" . So instead, I used below command to execute  eosio_build.sh
  
  bash eosio_build.sh darwin
  
  The output for above command populates the system info like OS, CPU, Disk etc. The execution will only start if 
  20GB storage is available else the script will exit with below output 
  
  	You must have at least 20GB of available storage to install EOSIO.
	Exiting now.

3. Now, say if you have the required storage and supported OS, the script will execute and will see if all the dependencies 
are installed else will ask for permission to install those. It took some time to install those dependencies in my case.

4. Then it started building verious objects from the installed dependencies. This step also took some time (well-to be honest- a lot of time). Total time (installing dependencies and building) required was around 2-2.5hrs.

So, now after EOSIO has been successfully built, we should have some programs like cleos, keosd, nodeos, eosio-launcher etc  in the /eos/programs directory. (We will use these in future while building our app-more info at https://developers.eos.io/eosio-nodeos/docs/overview-1 )

5. Now, to validate the build, we can run test cases provided by EOS.
To execute the test cases, we need to start the  "Mongo Daemon" - the host process for the database. (basically start the MongoDB process and run it in the background). 

	/eos$ ~/opt/mongodb/bin/mongod -f ~/opt/mongodb/mongod.conf &
	[1] <will display some number>
	/eos$ cd build/
	/eos/build$ make test
	Running tests...
	.
	.
	.
	100% tests passed, 0 tests failed out of 36

	Total Test time (real) = 1282.57 sec

(At this moment, in my case, it is executing 36 test cases but I think this number will keep on changing)

6. After successful tests execution, I installed all the executables with below command inside /build directory

	/build$ sudo make install

This was fairly quick.

Now, to launch node :

1. started single node blockchain with command

	nodeos -e -p eosio --plugin eosio::wallet_api_plugin --plugin eosio::chain_api_plugin --plugin 	eosio::history_api_plugin --contracts-console

This started generating blocks 

So here nodeos is the main command to start and you can use config options with that. Here , 
-e ==> enables block production
-p ==> provide ID of the block producer which would be controlled by this node
--plugin ==> to enable various plugins 

Now ,to validate the environment , lets try and run one example contract - eosio.token,  provided by EOS. For this, we need to create a wallet (which can be done by plugin command line option and that is why we have added eosio::wallet_api_plugin 
while starting our test node).

	:~$ cd ~/eos/build/programs/cleos/
	/eos/build/programs/cleos$ cleos wallet create -n <wallet-name>
	"/usr/local/bin/keosd" launched
	Creating wallet: <wallet-name>
	Save password to use in the future to unlock this wallet.
	Without password imported keys will not be retrievable.
	"the-password-string"

After creating the wallet, import the eos private key which you can find in /.local/share/eosio/nodeos/config.ini 
under tag "signature-provider" which is tuple of [public key, WIF private key] . 

Then create a public -private keypair with below commandwhich would be used as owner key. Also create one more key to use as active key for account creation.

	/eos/build/programs/cleos$     cleos create key
	Private key: <private-key>
	Public key: <public-key>

You can see the created wallet, keys etc with below commands :

	/eos/build/programs/cleos$ cleos wallet list
	Wallets:
	[
	  "<wallet-name> *"
	]

	/eos/build/programs/cleos$ cleos wallet keys
	[
	  "<publick-key>"
	]

	/eos/build/programs/cleos$ cleos wallet private_keys -n <wallet-name>
	password: [[
	    "<publick-key>",
	    "<private-key>"
	  ]
	]

Import the owner key and active key to wallet : 

	/eos/build/programs/cleos$ cleos wallet import -n <wallet-name>  <private-key>
	imported private key for: <publick-key>

Now, to create account 

	/eos/build/programs/cleos$ cleos create account eosio token <publick-owner-key> <publick-active-key>
	executed transaction: 4703f600865488eda6bb82fba43e41a56764e4399dc53659d36c7bb61cd9e1e4  200 bytes  334 us
	#         eosio <= eosio::newaccount            {"creator":"eosio","name":"token","owner":{"threshold":1,"keys":...
	warning: transaction executed locally, but may not be confirmed by the network yet

Check if the account is created properly with command :

	/eos/build/programs/cleos$ cleos get account token --json
	{
	  "account_name": "token",
	  "head_block_num": 20168,
	  "head_block_time": "2018-06-28T12:42:03.000",
	.
	.
	
upload the contract to blockchain now 

	/eos/build$ cleos set contract token ./contracts/eosio.token/ ./contracts/eosio.token/eosio.token.wast ./contracts/eosio.token/eosio.token.abi
	Reading WAST/WASM from ./contracts/eosio.token/eosio.token.wast...
	Assembling WASM...
	Publishing contract...
	
Create the token with

	/eos/build$ cleos push action token create '{"issuer":"token","maximum_supply":"1000000.0000 AMRTKN"}' -p token

Issue the tokens with

	/eos/build$ cleos push action token issue '{"to":"token","quantity":"1000.0000 AMRTKN","memo":""}' -p token

Check the balance to see if the tokens are issued 

	/eos/build$ cleos get table token token accounts
	{
	  "rows": [{
	      "balance": "1000.0000 AMRTKN"
	    }
	  ],
	  "more": false
	}
	
Now we can try to transfer some amount 

	/eos/build$ cleos push action token transfer '{"from":"token","to":"eosio","quantity":"10.0000 AMRTKN", "memo":"testing my first transfer"}' -p token
	executed transaction: 5d656c0aa739d57e1d351c5a72f75c478607b0a7959fc22ba8065bfb70eaf6d6  152 bytes  1195 us
	#         token <= token::transfer              {"from":"token","to":"eosio","quantity":"10.0000 AMRTKN","memo":"testing my first transfer"}
	#         eosio <= token::transfer              {"from":"token","to":"eosio","quantity":"10.0000 AMRTKN","memo":"testing my first transfer"}
	warning: transaction executed locally, but may not be confirmed by the network yet
	
check if the amount is deducted from token account 

	/eos/build$ cleos get table token token accounts
	{
	  "rows": [{
	      "balance": "990.0000 AMRTKN"
	    }
	  ],
	  "more": false
	}

We can see that before transfer the amount in token account was 1000 and now 990. The amount in eosio account now is 10.

	/eos/build$ cleos get table token eosio accounts
	{
	  "rows": [{
	      "balance": "10.0000 AMRTKN"
	    }
	  ],
	  "more": false
	}







Referred : https://developers.eos.io/eosio-nodeos/docs/build-validation , https://infinitexlabs.com/first-steps-in-eos-blockchain-development/ , https://github.com/EOSIO/eos/wiki/Tutorial-eosio-token-Contract

