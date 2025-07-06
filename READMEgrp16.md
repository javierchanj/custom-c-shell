# 📁 CSESHELL — PA1 for 50.005 Computer System Engineering

## 🛠 How to Compile and Run
Ensure you're using a **Linux** environment with `gcc` and `make` installed.  

## 📥 Cloning the Repository

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

## 📌 Built-in Functions Supported

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

### 🔧 Implementation Highlights

- Built-in command names are stored in a `const char *builtin_commands[]` array.
- Corresponding handler functions follow a consistent prototype: `int handler(char **args);`
- An array of function pointers maps commands to their handlers for clean, dynamic dispatch:
  ```c
  int (*builtin_command_func[])(char **) = {
      &shell_cd, &shell_help, &shell_exit, &shell_usage,
      &list_env, &set_env_var, &unset_env_var
  };

## ✨ Additional Features Supported

In addition to the required functionalities, our shell implementation includes the following extra features:

---

### 🎨 1. Decorated Prompt

The shell prompt is enhanced to display useful real-time information:

```bash
🐚 >> /current/working/directory >> Day Mon DD HH:MM:SS YYYY >> username
↳
```
### 🧾 2. `History` Built-in Command (Custom Feature)

The `history` command is a custom built-in feature that displays the most recent commands entered by the user during the current and past shell sessions.

#### ✅ Features:
- Stores each command entered into a file named `.cseshell_history` located in the shell’s working directory.
- When `history` is typed, the shell:
  - Reads the history file
  - Displays the **last 5 non-empty commands**
- File is **persistent across sessions**, so you can see your last used commands even after restarting the shell.

#### 📝 Example:

```bash
🐚 >> history
cd ..
env
setenv PROMPT_COLOR blue
ls
usage
```

 
# Sustainability Considerations 
Our shell and daemon programs are optimized for long-term maintainability, resource efficiency, and low-overhead operation:

## 1. Modular Architecture
Built-in commands and system programs are structured as isolated components (e.g., cd, usage, dspawn, dcheck) to allow easy future expansion without major rewrites.

## 2.Low Resource Daemon Design

Daemon processes created via dspawn are terminated after a fixed cycle.

File descriptors are properly closed and redirected to /dev/null, minimizing leakage and preserving system integrity.

## 3. Garbage Collection Support

Shell uses proper memory cleanup (free()) between commands, preventing memory leaks in long-running sessions.

## 4.Makefile Optimization

The build system compiles only necessary components and supports make clean for footprint reduction.

 # Inclusivity Considerations
Our implementation is built to support a diverse set of users and contributors, emphasizing accessibility, adaptability, and clarity

## 1.Customizable Shell via .cseshellrc

Supports user-defined environment setup and startup commands, enabling users to personalize the shell according to their workflows and hardware.

## 2.Internationalisation-Friendly Input Parsing

Input tokenization tolerates spacing quirks and non-alphanumeric characters, making command use more forgiving across cultures and keyboard layouts.

## 3. Clear  Error Messaging

Errors such as "command not found" or failed directory changes (cd) are returned with clear, non-technical language.

## 4. Accessible Documentation

Source files include comment blocks explaining functionality for novice programmers.

help and usage commands provide inline guidance without external documentation, increasing accessibility for first-time shell users.

## 5. Functionality Collaboration Support

Shell code is split into well-defined headers and source files to support contributions from multiple team members.

Built-in functions are matched with array-dispatched handlers, encouraging clean and consistent naming for teams practicing C together.


