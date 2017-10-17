# ftrest/ftrestd #

## Description ##

This client/server program was created for managing files and directories
on remote server. For command and file transport uses HTTP protocol, only
with 3 types of request(GET, PUT, DELETE). This message is aditionaly 
specified if request if for file or folder. If any error occurs server 
sends error message with http code according to standard and error message
is written to stderr.

## Running program ##

### ftrest - client ###
Client is started with :

	ftrest COMMAND REMOTE-PATH [LOCAL-PATH]
  
##### command #####
  * **del** - delete file specified in **REMOTE-PATH** from server
  * **get** - copy file from **REMOTE-PATH** into local directory 
  or to specified place **LOCAL-PATH**, if set
  * **put** - copy file from **LOCAL-PATH** into directory **REMOTE-PATH** 
  * **lst** - list content of remote directory
  * **mkd** - create a directory in folder specified by **REMOTE-PATH** on server site
  * **rmd** - delete a directory specified by **REMOTE-PATH** 
  
##### REMOTE-PATH #####
The path to file or folder in server.

##### LOCAL-PATH #####
Path to local file. Necassary for command **put**.

### ftrestd - server ###

Server is started with :

    ftrestd [-r ROOT-FOLDER] [-p PORT]
    
##### -r ROOT-FOLDER #####
Specify server root folder, where user folders are stored. Default values is current directory.

##### -p PORT #####
Setting listening port for server. Default value is 6677.

