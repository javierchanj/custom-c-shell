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

The following built-in commands are implemented directly in the shell:

| Command     | Description                                                                 |
|-------------|-----------------------------------------------------------------------------|
| `cd`        | Change the current working directory                                        |
| `help`      | Display help information about available built-in commands                 |
| `exit`      | Exit the shell                                                              |
| `usage`     | Display system usage statistics (CPU time, memory usage, etc.)             |
| `env`       | List all current environment variables                                      |
| `setenv`    | Set or update an environment variable (e.g., `setenv VAR value`)           |
| `unsetenv`  | Remove an environment variable from the environment                        |

These commands are recognized and executed by the shell itself without invoking external programs.


 
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


