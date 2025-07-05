// This code was written by ChatGPT4
// Modify it for your own usage to implement features for PA1 (or completely
// rewrite it) Include the shell header file for necessary constants and
// function declarations
#include "shell.h"
#include <time.h>     // For time(), localtime(), strftime()
#include <limits.h>   // For PATH_MAX
char base_dir[PATH_MAX];



const char *builtin_commands[] = {
    "cd",    // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
    "help",  //  List all builtin commands in the shell
    "exit",  // Exits the shell
    "usage", // Provides a brief usage guide for the shell and its built-in command
    "env", // Lists all the environment variables currently set in the shell
    "setenv", // Sets or modifies an environment variable for this shell session
    "unsetenv" // Removes an environment variable from the shell
    };

/*** This is array of functions, with argument char ***/
int (*builtin_command_func[])(char **) = {
    &shell_cd,     // builtin_command_func[0]: cd 
    &shell_help,   // builtin_command_func[1]: help
    &shell_exit,   // builtin_command_func[2]: exit
    &shell_usage,  // builtin_command_func[3]: usage
    &list_env,     // builtin_command_func[4]: env
    &set_env_var,  // builtin_command_func[5]: setenv
    &unset_env_var // builtin_command_func[6]: unsetenv
};


// Function to read a command from the user input
void read_command(char **cmd) {
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize coufnt to keep track of the number of characters read
  int count = 0, i = 0;
  // Array to hold pointers to the parsed command arguments
  char *array[MAX_ARGS], *command_token;

  // Infinite loop to read characters until a newline or maximum line length is
  // reached
  for (;;) {
    // Read a single character from standard input
    int current_char = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)current_char;
    // If a newline character is encountered, break out of the loop
    if (current_char == '\n')
      break;
    // If the command exceeds the maximum length, print an error and exit
    if (count >= MAX_LINE) {
      printf("Command is too long, unable to process\n");
      exit(1);
    }
  }
  // Null-terminate the command line string
  line[count] = '\0';


  // If only the newline character was entered, return without processing
  if (count == 1)
    return;


  // Use strtok to parse the first token (word) of the command
  command_token = strtok(line, " \n");


  // Continue parsing the line into words and store them in the array
  while (command_token != NULL) {
    array[i++] = strdup(command_token);  // Duplicate the token and store it
    command_token = strtok(NULL, " \n"); // Get the next token
  }


  // Copy the parsed command and its parameters to the cmd array
  for (int j = 0; j < i; j++) {
    cmd[j] = array[j];
  }
  // Null-terminate the cmd array to mark the end of arguments
  cmd[i] = NULL;
}

// Function to display the shell prompt
void type_prompt() {
  static int first_time = 1;
  if (first_time) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    first_time = 0;
  }

  char cwd[PATH_MAX];
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  // Format current time
  char time_str[64];
  strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Y", t);

  // Get current directory
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    strcpy(cwd, "[unknown-dir]");
  }

  // Print styled prompt
  printf("🐚 >> \033[1;33m%s\033[0m >> \033[1;34m%s\033[0m >> \033[1;32mNatAgusWanSuey\033[0m\n↳ ",
       cwd, time_str);

  fflush(stdout);
}



// Helper function to figure out how many builtin commands are supported by the shell
int num_builtin_functions(void) {
  return sizeof(builtin_commands) / sizeof(char *);
};


// The main function where the shell's execution begins
int main(void) {
  // Define an array to hold the command and its arguments

  
  char *cmd[MAX_ARGS];
  int child_status;
  pid_t pid;

  if (getcwd(base_dir, sizeof(base_dir)) == NULL) {
    perror("getcwd");
    exit(1);
  }

  // Infinite for loop, calling type_prompt and read_command 
  for(;;)
  {
    type_prompt();     // Display the prompt
    read_command(cmd); // Read a command from the user

    // If the command is empty skip execution 
    if(cmd[0] == NULL){
      continue; 
    }

    // // If the command is "exit", break out of the loop to terminate the shell
    // if (strcmp(cmd[0], "exit") == 0){
    //   break;
    // }

    int handled = 0;
    for (int command_index = 0; command_index < num_builtin_functions(); command_index++) {
      if (strcmp(cmd[0], builtin_commands[command_index]) == 0) {
        (*builtin_command_func[command_index])(cmd);
        handled = 1;
        break;
      }
    }

    if (handled) {
      for (int i = 0; cmd[i] != NULL; i++) {
        free(cmd[i]);
      }
      continue;  // Skip fork()
    }
    
    // Creating Child Process using Fork 
    pid_t pid = fork(); 

      // If error
      if (pid < 0){
        continue; // We don't want to exit. 
      }
      
      // In child process
      else if (pid == 0){
        // Formulate the full path of the command to be executed
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/bin/%s", base_dir, cmd[0]);

        execv(full_path, cmd);

        // If execv returns, command failed
        fprintf(stderr, "cseshell: command not found: %s\n", cmd[0]);
        _exit(1);  // Use _exit in child after fork
      }
      // In Parent process 
      else{
        int status, child_exit_status;

        waitpid(pid, &status, WUNTRACED);
        // if child terminates properly,
        if (WIFEXITED(status)){
            child_exit_status = WEXITSTATUS(status);
        }
        // checks child_exit_status and do something about it exit[0] is success exit[1] is failure
        if (child_exit_status == 0){
          printf("successful child exit\n");
        }
        else{
          printf("unsuccessful child exit\n");
        }
      }

    // Free the allocated memory for the command arguments before exiting
    for (int i = 0; cmd[i] != NULL; i++)
    {
      free(cmd[i]);
    }

  }
}


// For each builtin functions to work
// (1) cd functions
int shell_cd(char **args) {
  if (args[1] == NULL) { // directory not provided
    chdir(getenv("HOME")); 
  } else {
    if (chdir(args[1]) != 0) { // directory provided 
      perror("cd");
    }
  }
  return 1;
}

// (2) help function
int shell_help(char **args) {
  printf("Available built-ins:\n");
  for (int i = 0; i < num_builtin_functions(); i++) { // loop through built-in commands
    printf("  %s\n", builtin_commands[i]);
  }
  return 0;
}

// (3) exit function
int shell_exit(char **args) {
  exit(0);  // Shell terminates 
}

// (4) usage function
int shell_usage(char **args) {
  if (args[1] == NULL) { // check for command 
    printf("Command not given. Type usage <command>.\n");
    return 1;
  }

  if (strcmp(args[1], "cd") == 0) {
    printf("Type: cd directory_name to change the current working directory of the shell\n");
  } else if (strcmp(args[1], "help") == 0) {
      printf("Type: help for supported commands\n");
  } else if (strcmp(args[1], "exit") == 0) {
      printf("Type: exit to terminate the shell gracefully\n");
  } else if (strcmp(args[1], "usage") == 0) {
      printf("Type: usage <cd/help/exit>\n");
  } else if (strcmp(args[1], "env") == 0) {
      printf("Type: env to list all registered env variables\n");
  } else if (strcmp(args[1], "setenv") == 0) {
      printf("Type: setenv ENV=VALUE to set a new env variable\n");
  } else if (strcmp(args[1], "unsetenv") == 0) {
      printf("Type: unsetenv ENV to remove this env from the list of env variables\n");
  } else { // not available
      printf("The command you gave: %s, is not part of CSEshell's builtin command\n", args[1]);
  }
  return 0;
}


// (5) env 
extern char **environ;
int list_env(char **args) {
  for (char **env = environ; *env != NULL; env++) {
    printf("%s\n", *env);
  }
  return 0;
}

// (6) set env 
int set_env_var(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "setenv: missing KEY=VALUE\n");
    return 1;
  }

  // split args[1] with equal
  char *equal_sign = strchr(args[1], '=');
  if (equal_sign == NULL) {
    fprintf(stderr, "setenv: use format KEY=VALUE\n");
    return 1;
  }

  *equal_sign = '\0';  // temporarily split the string
  char *key = args[1];
  char *value = equal_sign + 1;

  if (setenv(key, value, 1) != 0) {
    perror("setenv");
    return 1;
  }
  return 0;
}


// (7) unset env
int unset_env_var(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "unsetenv: missing variable name\n");
    return 1;
  }
  if (unsetenv(args[1]) != 0) {
    perror("unsetenv");
    return 1;
  }
  return 0;
}

