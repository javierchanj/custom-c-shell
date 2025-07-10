// This code was written by ChatGPT4
// Modify it for your own usage to implement features for PA1 (or completely
// rewrite it) Include the shell header file for necessary constants and
// function declarations
#include "shell.h"

#define MAX_LINE_LENGTH 1024
#define HISTORY_FILE ".cseshell_history"
#include <time.h>     // For time(), localtime(), strftime()
#include <limits.h>   // For PATH_MAX
char base_dir[PATH_MAX];
void append_to_history(const char *raw_line);



const char *builtin_commands[] = {
    "cd",    // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
    "help",  //  List all builtin commands in the shell
    "exit",  // Exits the shell
    "usage", // Provides a brief usage guide for the shell and its built-in command
    "env", // Lists all the environment variables currently set in the shell
    "setenv", // Sets or modifies an environment variable for this shell session
    "unsetenv", // Removes an environment variable from the shell
    "history"
    };

/*** This is array of functions, with argument char ***/
int (*builtin_command_func[])(char **) = {
    &shell_cd,     // builtin_command_func[0]: cd 
    &shell_help,   // builtin_command_func[1]: help
    &shell_exit,   // builtin_command_func[2]: exit
    &shell_usage,  // builtin_command_func[3]: usage
    &list_env,     // builtin_command_func[4]: env
    &set_env_var,  // builtin_command_func[5]: setenv
    &unset_env_var, // builtin_command_func[6]: unsetenv
    &shell_history
};


// Function to read a command from the user input
void read_command(char **cmd) {
  // Define a character array to store the command line input
  char line[MAX_LINE];
  char raw_line[MAX_LINE];  // to store the full input line
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
  strncpy(raw_line, line, MAX_LINE);  // Save a copy before strtok modifies it



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
  append_to_history(raw_line);  // Save command to .cseshell_history

}

// Function to display the shell prompt
void type_prompt() {
  static int first_time = 1;
  if (first_time) {
#ifdef _WIN32
    system("cls"); // Windows only
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
  getcwd(base_dir, sizeof(base_dir));  

  char *cmd[MAX_ARGS];  
  int child_status;
  pid_t pid;

  const char *filePath = ".cseshellrc"; // Specify the path to your text file

  // opening the .rc file using fopen 
  FILE *file = fopen(".cseshellrc", "r"); //args are fopen(filename, "read"/"write")
  if (file == NULL){
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  // While loop to incorporate path of .cseshellrc file begins here
  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file))
  {
    // Remove the newline at end of line
    line[strcspn(line, "\r\n")] = '\0';  

        // If line in .cseshell starts with varialbe PATH, rest of line interpret as PATH environemnt variable (so setenv)
        // Set the PATH environment variable of your shell process to the specified value in .cseshellrc 

        // First we need to check if it's PATH. 

    if (strncmp (line, "PATH=", 5) == 0){ // Means is PATH 
      // Since it's PATH, set the env variable of shell to .cseshellrc
      setenv("PATH", line + 5, 1); // Replacing existing path 
      continue; 
    } 

    // Tokenize the current line from .cseshellrc into command and arguments, storing them in rc_cmd
    char *rc_cmd[MAX_ARGS];
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        rc_cmd[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    rc_cmd[i] = NULL; // Mark end

    // Skip this if no command found
    if (rc_cmd[0] == NULL) continue;
                
    printf("[.rc] running: %s\n", rc_cmd[0]);
    fflush(stdout);            
    pid_t pid = fork();

    if (pid < 0){
        // If fork() returns -1, an error occurred
      perror("fork failed\n");
      continue;
    }

    else if (pid == 0){
      // Child process
      execvp(rc_cmd[0], rc_cmd);
      fprintf(stderr, "cseshell: command not found: %s\n", rc_cmd[0]);
      _exit(1); 
    }

    else{
      // Parent process
      int status;
      waitpid(pid, &status, 0); // Wait for child process to finish
      }
      for (int j = 0; rc_cmd[j] != NULL; j++) {
      free(rc_cmd[j]);
    }

    } 

  fclose(file); // Close the file

  // Command for opening file 
  // FILE *hist = fopen(HISTORY_FILE, "r");
  // if (hist != NULL) {
  //   char hist_line[MAX_LINE_LENGTH];
  //   printf("Previous Commands (History):\n");
  //   while (fgets(hist_line, sizeof(hist_line), hist)) {
  //     printf("  %s", hist_line);  // Already has newline
  //   }
  //   fclose(hist);
  //   printf("\n");
  // }

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


//compare cmd[0] against builtin_commands[], and call builtin_command_func[i](cmd) if found.
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
  
    pid = fork(); 

      // If error
      if (pid < 0){
        continue; // We don't want to exit. 
      }
      
      // In child process
    else if (pid == 0){
        // Formulate the full path to [PROJECT_DIR]/bin/cmd
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/bin/%s", base_dir, cmd[0]);

        // Try executing from bin/
        execv(full_path, cmd);

        // If that fails, try searching with PATH
        execvp(cmd[0], cmd);

        // If still fails, print error and exit
        fprintf(stderr, "cseshell: command not found: %s\n", cmd[0]);
        _exit(1);
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


  return EXIT_SUCCESS;
}

// ========================
// Built-in Command Handlers
// ========================
// shell_cd - change current working directory
// Usage: cd [directory]
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

// (4) usage function (hardcoded to give same output as shown on web)
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


// (5) env (showing the whole list of env)
extern char **environ;
int list_env(char **args) {
  for (char **env = environ; *env != NULL; env++) {
    printf("%s\n", *env);
  }
  return 0;
}

// (6) set env (add)
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


// (7) unset env (del)
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


// Shell history command
int shell_history(char **args) {
  FILE *file = fopen(HISTORY_FILE, "r");
  if (file == NULL) {
    printf("No history available.\n");
      return 1;
  }

  char *lines[100];  // store up to 100 history entries
  int count = 0;
  char buffer[MAX_LINE_LENGTH];

  while (fgets(buffer, sizeof(buffer), file)) {
    // Trim leading whitespace
    char *line = buffer;
    while (*line == ' ' || *line == '\t') line++;

    // Skip if line is empty or just newline
    if (line[0] == '\n' || line[0] == '\0') continue;

    lines[count++] = strdup(line);
    if (count >= 100) break;
  }
  fclose(file);

  // Show only last 5 entries
  int start = count > 5 ? count - 5 : 0;
  for (int i = start; i < count; i++) {
    printf("%s", lines[i]);  // line already has \n
    free(lines[i]);
  }
  return 0;
}


// Function to append commands to history
void append_to_history(const char *raw_line) {
    FILE *file = fopen(HISTORY_FILE, "a");
    if (file != NULL) {
        fprintf(file, "%s\n", raw_line);
        fclose(file);
    }
}

