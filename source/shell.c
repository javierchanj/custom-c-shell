// This code was written by ChatGPT4
// Modify it for your own usage to implement features for PA1 (or completely
// rewrite it) Include the shell header file for necessary constants and
// function declarations
#include "shell.h"
#define MAX_LINE_LENGTH 1024

// Function to read a command from the user input
void read_command(char **cmd) {
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize count to keep track of the number of characters read
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
  // Use a static variable to check if this is the first call to the function
  static int first_time = 1;
  if (first_time) {
    // Clear the screen on the first call
#ifdef _WIN32
    system("cls"); // Windows command to clear screen
#endif
    first_time = 0;
  }
  fflush(stdout); // Flush the output buffer
  printf("$$ ");  // Print the shell prompt
}

// The main function where the shell's execution begins
int main(void) {
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS];  
  int child_status;
  pid_t pid;

  const char *filePath = ".cseshellrc"; // Specify the path to your text file

  // opening the .rc file using fopen 
  FILE *file = fopen(".cseshellrc", "r"); //args are filename, "read"/"write"
  if (file == NULL){
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  // While loop to incorporate path of .cseshellrc file begins here
  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file))
  {
    line[strcspn(line, "\r\n")] = '\0';  // Remove the newline at end of line

        // If line in .cseshell starts with varialbe PATH, rest of line interpret as PATH environemnt variable (so setenv)
        // Set the PATH environment variable of your shell process to the specified value in .cseshellrc 

        // First we need to check if it's PATH. 

    if (strncmp (line, "PATH=", 5) == 0){ // Means is PATH 
      // Since it's PATH, set the env variable of shell to .cseshellrc
      setenv("PATH", line + 5, 1); // Replacing existing path 
      continue; 
    } 

    char *rc_cmd[MAX_ARGS];
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        rc_cmd[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    rc_cmd[i] = NULL;

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

  // Infinite for loop, calling type_prompt and read_command 
  for(;;)
  {
    type_prompt();     // Display the prompt
    read_command(cmd); // Read a command from the user

    // If the command is empty skip execution 
    if(cmd[0] == NULL){
      continue; 
    }

    // If the command is "exit", break out of the loop to terminate the shell
    if (strcmp(cmd[0], "exit") == 0){
      break;
    }
    
    // Creating Child Process using Fork 
  
    pid = fork(); 

      // If error
      if (pid < 0){
        continue; // We don't want to exit. 
      }
      
      // In child process
      else if (pid == 0){
        // Arguments array
        // Execute command
        execvp(cmd[0],cmd);

        // If execvp returns, it must have failed
        perror("execvp failed");
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
