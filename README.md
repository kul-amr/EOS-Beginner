# EOS-Beginner

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
