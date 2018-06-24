# EOS-Beginner

(OS- Ubuntu 18.04 LTS)

EOS dApp dev steps:

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

Referred : https://developers.eos.io/eosio-nodeos/docs/build-validation , https://infinitexlabs.com/first-steps-in-eos-blockchain-development/ 

