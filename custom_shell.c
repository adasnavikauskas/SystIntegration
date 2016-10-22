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
#define KRED  "\x1B[31m"

#define FALSE 0
#define TRUE !FALSE

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
  "ls"
};

int (*builtin_func[]) (char **) = {
  &mshell_cd,
  &mshell_help,
  &mshell_exit,
  &mshell_pwd,
  &mshell_ifc,
  &mshell_dt,
  &mshell_ud,
  &mshell_ls
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

int mshell_ud()
{
  register struct passwd *pw;
  struct group *g;
  register uid_t uid;
  int c;
  register gid_t gid;
  char cwd[1024];
  
  getcwd(cwd, sizeof(cwd));
  gid = getgid();
  uid = geteuid();
  g = getgrgid(gid);
  pw = getpwuid (uid);
  if (pw)
        {
	 char my_cmd[64];
	 snprintf(my_cmd, 64, "ls -id %s", cwd);
	 system(my_cmd);
         printf("%u,%u,%s,%s\n", uid,gid,pw->pw_name,g->gr_name);
        }
  return 1;
}

int mshell_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}
int mshell_dt()
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  printf("%d%d%d%d%d%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec);
}

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
        snprintf(my_cmd, 64, "ifconfig %s", word);
        system(my_cmd);
        return 1;
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
 // printf("Stephen Brennan's LSH\n");
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

  return 0;
}


/**
   @brief Execute shell built-in or launch program.
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

 // return lsh_launch(args);
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

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();
    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
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
  while (token != NULL) {
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
    printf("> ");
    line = mshell_read_the_input();
    args = mshell_split_line_into_arguments(line);
    status = mshell_execute_command(args);

    free(line);
    free(args);
  } while (status);
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
