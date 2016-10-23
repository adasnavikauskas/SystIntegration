/*
Name: 		Adas Navikauskas
Student Number:	C13496402
Email:		C13496402@mydit.ie
Module:		Systems Integration
Assignment:	Custom Shell (in variation of C and system calls)
*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdint.h>
#define KRED  "\x1B[31m"

#define FALSE 0
#define TRUE !FALSE
/**
	Global Comment:
	1)	if (args[1] == NULL) = checking for second argument
		of array, a.k.a if it exists

	2)	Tab Completion was attempted but discontinuted
		as caused major bug in code

		-see *mshell_read_the_input(void) as it works on its own
			with readline library

	3)	Line history (pressing up with arrow key) not
		integrated as i'd need to change program considerably

	4)	Line editing (ctr d) not implemented but heavily considered
**/
/*
  Function Declarations for builtin shell commands:
	
 */
int mshell_cd(char **args);
int mshell_help(char **args);
int mshell_exit(char **args);
int mshell_pwd(char **args);
int mshell_ifc(char **args);
int mshell_dt();
int mshell_ud();
int mshell_ls(char **args);
int mshell_mkdir(char **args);
int mshell_cl();
int mshell_touch(char **args);
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *all_builtin_strings[] = {
	"cd",
	"help",
	"exit",
	"pw",
	"ifc",
	"dt",
	"ud",
	"ls",
	"mkdir",
	"cl",
	"tch"
};

int (*builtin_func[]) (char **) = {
	&mshell_cd,
	&mshell_help,
	&mshell_exit,
	&mshell_pwd,
	&mshell_ifc,
	&mshell_dt,
	&mshell_ud,
	&mshell_ls,
	&mshell_mkdir,
	&mshell_cl,
	&mshell_touch
};

int mshell_number_of_functions() {
  return sizeof(all_builtin_strings) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int mshell_cd(char **args)
{
 	if (args[1] == NULL) {
    		fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  	} 
	else {
    		if (chdir(args[1]) != 0) {
      			perror("lsh");
    	}
  }
  return 1;
}

/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int mshell_help(char **args)
{
  	int i;
 
  	printf("Type program names and arguments, and hit enter.\n");
  	printf("The following are built in:\n");

  	for (i = 0; i < mshell_number_of_functions(); i++) {
    		printf("  %s\n", all_builtin_strings[i]);
  	}

  	printf("Use the man command for information on other programs.\n");
  	return 1;
}
/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int mshell_exit(char **args)
{
	printf("you will now exit the shell\n");
	sleep(2);
  	return 0;
}

/**
   @brief Builtin command: print current working directory.
   @param args List of args are ignored due to specification.
   @return Always returns 1, to continue executing.
 */
int mshell_pwd(char **args)
{
   	if (args[1] != NULL) {
    		char cwd[1024];
    		getcwd(cwd, sizeof(cwd));
    		printf("Current Working dir: %s\n", cwd);
   	}
   	else {
    		char cwd[1024];
    		getcwd(cwd, sizeof(cwd));
    		printf("Current Working dir: %s\n", cwd);
   	}
}
/**
   @brief Builtin command: print ifconfig eth1 by defualt.
   @param args List of args.  print ifconfig <node>.
   @return Always returns 1, to continue executing.
 */
int mshell_ifc(char **args)
{
  	char *word;
  	char real;
  	if (args[1] == NULL) {
  		system("ifconfig eth1");
  	}
  	else
  	{
   	     	char *word = args[1];
   	     	char my_cmd[64];
		//store full command into a buffer
		snprintf(my_cmd, 64, "ifconfig %s", word);
		//execute buffer
    	   	system(my_cmd);
        	return 1;
  }
        return 1;

}
/**
   @brief Builtin command: print date and time in format to spec.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */

int mshell_dt()
{
  	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);

  	printf("%d%d%d%d%d%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec);
}


/**
   @brief Builtin command: print userid,gid,username,groupname and inode of current working directory.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int mshell_ud()
{
	//declared pointers
  	register struct passwd *pw;
  	struct group *g;
  	register uid_t uid;
  	int c;
  	register gid_t gid;
  	char cwd[1024];
  	struct stat sb;
	//get user info
  	getcwd(cwd, sizeof(cwd));
  	gid = getgid();
  	uid = geteuid();
  	g = getgrgid(gid);
  	pw = getpwuid (uid);
  	char my_cmd[1024];

  	if (pw)
       	{
        	printf("%u,%u,%s,%s,", uid,gid,pw->pw_name,g->gr_name);
		snprintf(my_cmd, 1024, "ls -id %s | awk '{print $1}'", cwd);
		printf("\n");
		printf("UserID: %u\n",uid);
		printf("GroupID: %u\n",gid);
        	printf("User Name: %s\n",pw->pw_name);
		printf("Group Name: %s\n",g->gr_name);
		printf("iNode of current directory: ");
	
		system(my_cmd);
		printf("\n");
	}
  	return 1;
}
/**
   @brief Builtin command: print listing of files and directories.
   @param args List of args.  Not examined as it is ignored implicitly.
   @return Always returns 1, to continue executing.
 */
int mshell_ls(char **args)
{
	char *word;
	if (args[1] == NULL) {
		system("ls");
		return 1;
	}
	else{
		char *word = args[1];
	        char my_cmd[64];
        	snprintf(my_cmd, 64, "ls %s", word);
        	system(my_cmd);
		return 1;
	}
}



/**
   @brief Builtin command: create new directory.
   @param args List of arg: name of directory.
   @return Always returns 1, to continue executing.
 */
int mshell_mkdir(char **args)
{
	struct stat st = {0};
	if (args[1] == NULL) {
		printf("Please add the name of the directory you want to create\n");
		return 1;
	}

	else {
		//check if it doesn't already exist
		if(stat(args[1], &st) == -1)
		{
			mkdir(args[1], 0700);
			return 1;
		}
		else
		{
			printf("dir already exists please try a different name\n");
			return 1;
		}
	}
}
/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int mshell_touch(char **args)
{
	struct stat st = {0};
	char *word;
        if (args[1] == NULL) {
                printf("Please add the name of the file you want to create\n");
                return 1;
        }

        else {
                if(stat(args[1], &st) == -1)
                {
                        char *word = args[1];
        	        char my_cmd[64];
	                snprintf(my_cmd, 64, "touch %s", word);
                	system(my_cmd);
                	return 1;

                        return 1;
                }
                else
                {
                        printf("file already exists please try a different name\n");
                        return 1;
                }
        }
}
int mshell_cl()
{
	system("tput clear");
	return 1;
}





/*
	Handling of 3 core principles of a shell
	1) Read the line
	2) Parse the line into tokens
	3) Execute the line (tokens)
*/


/**
   @brief Execute shell built-in 
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int mshell_execute_command(char **args)
{
  	int i;

  	if (args[0] == NULL) {
    		// An empty command was entered.
    		return 1;
  	}

  	for (i = 0; i < mshell_number_of_functions(); i++) {
    		if (strcmp(args[0], all_builtin_strings[i]) == 0) {
      			return (*builtin_func[i])(args);
     		}
  	}

}

#define mshell_read_input_bufsize 1024
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */

char *mshell_read_the_input(void)
{
  	int bufsize = mshell_read_input_bufsize;
  	int position = 0;
  	char *buffer = malloc(sizeof(char) * bufsize);
  	int c;
  	char *inpt;

  	if (!buffer) {
    		fprintf(stderr, "lsh: allocation error\n");
    		exit(EXIT_FAILURE);
  	}

  	while (1) {
    	// Read a character
    		c = getchar();
		/***	attempted to integrate tab completion
			it works, but bug in code
			goes into loop and does't execute command
	    inpt = readline("Enter text: ");
	    add_history(inpt);
	    c = (int) (long) inpt;
		**/
    	// If we hit EOF, replace it with a null character and return.
    		if (c == EOF || c == '\n') {
      			buffer[position] = '\0';
      			return buffer;
    		} 
		else 
		{
      			buffer[position] = c;
    		}
    		position++;

    	// If we have exceeded the buffer, reallocate.
    		if (position >= bufsize) {
      			bufsize += mshell_read_input_bufsize;
      			buffer = realloc(buffer, bufsize);
      			if (!buffer) {
        			fprintf(stderr, "lsh: allocation error\n");
        			exit(EXIT_FAILURE);
      			}
    		}
  	}
}

#define token_buffersize 64
#define token_delimiter " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **mshell_split_line_into_arguments(char *line)
{
  	int bufsize = token_buffersize, position = 0;
  	char **tokens = malloc(bufsize * sizeof(char*));
  	char *token;

  	if (!tokens) {
    		fprintf(stderr, "lsh: allocation error\n");
    		exit(EXIT_FAILURE);
  	}

  	token = strtok(line, token_delimiter);
  	while (token != NULL) 
	{
    		tokens[position] = token;
    		position++;

    		if (position >= bufsize) {
     			bufsize += token_buffersize;
      			tokens = realloc(tokens, bufsize * sizeof(char*));
      			if (!tokens) {
        			fprintf(stderr, "lsh: allocation error\n");
        			exit(EXIT_FAILURE);
      			}
    		}

    		token = strtok(NULL, token_delimiter);
  	}

  	tokens[position] = NULL;
  	return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void mshell_loop(void)
{
  	char *line;
  	char **args;
  	int status;

  	do {
    		printf(">> ");
    		line = mshell_read_the_input();
    		args = mshell_split_line_into_arguments(line);
    			status = mshell_execute_command(args);

    		free(line);
    		free(args);
  	} 
	while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  //made the terminal for the user with the color red
  	printf(KRED "Welcome to the Custom Shell made by Adas.");
  	printf("\nFor a list of available options type help\n");
  	mshell_loop();
  //reset colours back to normal for other users
  	system("tput sgr0");
  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

