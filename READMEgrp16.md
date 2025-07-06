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

 
## 🌿 Sustainability and 🌍 Inclusivity Considerations

As part of SUTD’s and the Engineering Accreditation Board’s initiatives, this shell project incorporates key features and thoughtful design choices to support both **sustainability** and **inclusivity**.

---

### ♻️ Sustainability Features

#### ✅ 1. Lightweight Design & Efficient Execution
- All built-in commands are executed **without creating child processes**, which saves CPU cycles and avoids unnecessary system calls.
- The shell uses **non-busy loops**, remaining idle when not in use and minimizing energy consumption.
- History and `.cseshellrc` parsing are performed using simple and efficient file I/O, reducing system resource usage.

#### 🛠 2. Configurable for Low Resource Use
- By keeping the design minimal and modular, the shell performs well even on older or low-powered hardware.
- Future enhancement: support for environment variables like `POWER_MODE=low` to further reduce polling or background activity.

---

### 🌍 Inclusivity Features

#### 🎨 1. Prompt Color Customization
- Users can personalize their shell prompt using the `PROMPT_COLOR` environment variable.
- This helps accommodate individual **visual preferences**, including support for **colorblind-friendly setups** (e.g., using distinct, readable colors).

#### 🧾 2. Clear, Friendly Messaging
- The `usage` command provides simple, jargon-free descriptions of built-in commands.
- Error messages are designed to be informative and beginner-friendly.
  - For example: “Command not found” or “Missing argument: usage <command>”

#### 📜 3. Session History Feature
- The `history` built-in command allows users to view previously typed commands, reducing repetitive typing and supporting users with memory or learning difficulties.

---

### Summary

These features reflects how our shell is both resource-efficient and user-friendly for a wide range of users. The implementation balances functionality with simplicity, ensuring compatibility with diverse environments and needs.




