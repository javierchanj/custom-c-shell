# 📁 CSESHELL — PA1 for 50.005 Computer System Engineering

## How to Compile and Run
Ensure you're using a **Linux** environment with `gcc` and `make` installed.  

## Cloning the Repository

To get started, clone the repository from GitHub:

```bash
git clone https://github.com/50005-computer-system-engineering/programming-assignment-1-cl02-grp16.git
cd programming-assignment-1-cl02-grp16
```

To build the shell and all system programs, run:

```bash
make
```

Start the shell by running:
```bash
./cseshell
```

## Built-in Functions Supported

The following 7 built-in commands are implemented in the shell. These commands are handled internally without forking a new process, using a table of function pointers to map each command to its corresponding handler.

| Command     | Description                                                                 |
|-------------|-----------------------------------------------------------------------------|
| `cd`        | Changes the current working directory. If no path is provided, it defaults to the home directory. After changing directories, all commands remain functional. |
| `help`      | Lists all available built-in commands supported by the shell.               |
| `exit`      | Exits the shell cleanly.                                                    |
| `usage`     | Prints a brief usage description of each built-in command. Also prints a friendly message if the user inputs an unknown command. |
| `env`       | Lists all environment variables currently available to the shell process, including inherited system variables and any set using `setenv`. |
| `setenv`    | Sets or modifies an environment variable. Usage: `setenv KEY VALUE`. No output is printed if successful. |
| `unsetenv`  | Removes the specified environment variable. Usage: `unsetenv KEY`. If the variable does not exist, it silently succeeds. |

---

### Implementation Highlights

- Built-in command names are stored in a `const char *builtin_commands[]` array.
- Corresponding handler functions follow a consistent prototype: `int handler(char **args);`
- An array of function pointers maps commands to their handlers for clean, dynamic dispatch:
  ```c
  int (*builtin_command_func[])(char **) = {
      &shell_cd, &shell_help, &shell_exit, &shell_usage,
      &list_env, &set_env_var, &unset_env_var
  };

## Additional Features Supported

In addition to the required functionalities, our shell implementation includes the following extra features:

---

### 1. Decorated Prompt

The shell prompt is enhanced to display useful real-time information:

```bash
🐚 >> /current/working/directory >> Day Mon DD HH:MM:SS YYYY >> username
↳
```
###  2. `History` Built-in Command (Custom Feature)

The `history` command is a custom built-in feature that displays the most recent commands entered by the user during the current and past shell sessions.

#### Features:
- Stores each command entered into a file named `.cseshell_history` located in the shell’s working directory.
- When `history` is typed, the shell:
  - Reads the history file
  - Displays the **last 5 non-empty commands**
- File is **persistent across sessions**, so you can see your last used commands even after restarting the shell.

#### Example:

```bash
🐚 >> history
cd ..
env
setenv PROMPT_COLOR blue
ls
usage
```

 
## Sustainability and Inclusivity

As part of SUTD and the Engineering Accreditation Board's initiatives, this project incorporates elements that promote both sustainability and inclusivity. The goal is to create a lightweight, efficient, and user-friendly shell that can be adapted for diverse users and systems.

---

### Sustainability Consideration

**💡 Lightweight Command Execution & Idle Resource Awareness**

- The shell is designed to be **lightweight**, avoiding unnecessary CPU usage:
  - No busy loops — it sleeps and waits efficiently for user input.
  - Built-in commands are executed **without forking**, saving process overhead.
- Uses simple and efficient file I/O for `.cseshellrc` and history management to minimize system resource use.

** Future Suggestions:**
- Implement built-in system resource feedback (e.g., `usage` could show memory/CPU stats).
- Introduce environment variable options to control how aggressively the shell refreshes or logs data (e.g., low-power mode).

---

### Inclusivity Consideration

** Customizable Interface for Diverse Users**

- Users can set environment variables like `PROMPT_COLOR` and `THEME` to personalize their shell experience based on visual preference (e.g., light/dark mode, high contrast).
- Prompts are decorated with contextual information to improve orientation (e.g., path, time, user), which supports clarity for beginners.

** Inclusive Feedback and Messaging**
- All built-in commands return helpful and friendly error messages.
- `usage` provides explanations for each command, reducing the need for technical background.

** Future Suggestions:**
- Add multilingual support or translation files for help/usage output.
- Provide text scaling or accessibility toggles via environment variables for visually impaired users.

---

We believe that small design choices like customizability, clarity in error messaging, and efficient execution contribute significantly toward a sustainable and inclusive development mindset.



