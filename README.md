#Adas Navikauskas C13496402
##Systems Integration
#C13496402@mydit.ie <----- Student email Address

###Custom Shell made entirely in C.

Date Started: 11th October
Date Finished 1st Draf: 18th October

##Introduction

Welcome to the Custom Shell made by Adas Navikauskas.
The purpose of this project is to gain knowledge and experience
in developing and using my own custom shell writen in C.
It has multiple features built into it using C libraries of the
equivalent functions.

These functions have been abbreviated for ease of use.
E.g running ifc is shorthand for ifconfig eth1
This is done by having built_in strings and error checking
to see if the input is in the string list.

The main features of this shell include:

- "help"
- "cd"
- "exit"
- "ifc" -> ifconfig with default of eth1
- "ifc <argument>" ifconfig with different output
        E.G ifc lo
- "ud" -> UserID,GroupID,username,groupname,inode of home directory
- "dt" -> date and time in a specific format

##Installation
Installation is simple and quick.
- This is the most recommended way to install this custom shell and is done following the steps bellow:
	- Enter: cd /
		- This will bring you to your root directory
	- Then enter: git clone "https://github.com/adasnavikauskas/SystIntegration.git"
	- This will clone into your root direcotry.
	- This repository should contain a file named MyCustomShell which is the script for the custom shell
	- To update, go to the repo folder and enter the command: git pull

**1.1 - Creating Users**
- To create a new user, enter the following commands:
	- useradd -m username
		- e.g. useradd -m "new user"
		- Flag -m will create and set a home directory for the use and named as the user.
	- passwd <user> #will create a password for the user
		- e.g. passwd newuser
		- This will create a password for that user.
- To switch user, enter the command: su "user"

**1.2 - User Shell Configuration**
- This step will change the default shell to custom shell of a user.
	- Enter command: usermod -s /SystIntegration/MyCustomShell "user"	
		- e.g. usermod -s /SystemsIntegration/MyCustomShell newuser
		- This will make "newuser" use the shell customshell

