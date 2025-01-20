# Minishell: A Minimal Shell Implementation in C

## Overview

Welcome to **Minishell**, a project aimed at developing a simplified, yet robust command-line shell written in C. The goal of this project is to provide an interactive shell that mimics the behavior of Bash, handling pipelines, redirections, built-in commands, and signal management. This project was completed as part of a rigorous programming curriculum, adhering to strict coding standards and memory management requirements.

Minishell showcases my ability to write clean, efficient, and reliable code while managing complexity in a real-world scenario. It is a testament to my understanding of system-level programming, including process control, memory allocation, and signal handling.

---

## Features

- **Interactive Command-Line Interface:**
  - Displays a prompt while waiting for user input.
  - Supports a functional command history using `readline` and `add_history`.
- **Command Execution:**
  - Executes binaries located via the `PATH` environment variable or specified as absolute/relative paths.
- **Redirection:**
  - Input (`<`) and output (`>`).
  - Append redirection (`>>`) and here-document (`<<`) with custom delimiters.
- **Pipelines:**
  - Allows chaining of commands using the `|` operator.
- **Environment Variable Expansion:**
  - Handles `$VARIABLE` expansion and supports `$?` to display the exit status of the last command.
- **Built-in Commands:**
  - Includes `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
- **Signal Handling:**
  - Manages `CTRL-C`, `CTRL-D`, and `CTRL-\` like Bash in interactive mode.
- **Memory Management:**
  - Ensures no memory leaks, except for those explicitly allowed by `readline`.

---

## Technical Highlights

1. **Adherence to Strict Coding Standards:**
   - The project strictly complies with the provided Norm coding standard, ensuring readability and maintainability of the codebase.
   - Comprehensive use of `-Wall`, `-Wextra`, and `-Werror` flags to ensure error-free compilation.

2. **Robust Memory Management:**
   - Dynamically allocated memory is properly freed to prevent memory leaks.
   - Extensive testing was conducted using tools like `valgrind` to ensure reliability.

3. **Error Handling:**
   - Implements thorough checks to prevent segmentation faults, double frees, and other undefined behaviors.

4. **Makefile:**
   - Provides rules for compilation (`all`), cleaning (`clean`, `fclean`), and rebuilding (`re`).
   - Includes a `bonus` target for additional functionality.

5. **Testing and Debugging:**
   - Created test scripts to validate all functionalities, including edge cases.
   - Handled user input errors gracefully to prevent unexpected crashes.

---

## Challenges and Solutions

### 1. **Signal Management:**
   - **Challenge:** Implementing signal handling while avoiding excessive use of global variables.
   - **Solution:** Used a minimal global variable setup to store signal information and carefully managed signal behavior to ensure responsiveness during interactive sessions.

### 2. **Pipeline and Redirection:**
   - **Challenge:** Managing multiple processes and file descriptors for complex command pipelines and redirections.
   - **Solution:** Used `pipe`, `dup2`, and careful process management to ensure seamless chaining of commands without deadlocks or resource leaks.

### 3. **Environment Variable Expansion:**
   - **Challenge:** Expanding variables dynamically while preserving quotes and avoiding unintended behavior.
   - **Solution:** Developed a custom parser to handle variable expansions within quotes while adhering to shell behavior.

### 4. **Adhering to the Norm:**
   - **Challenge:** Writing concise and efficient code while following stringent coding rules (e.g., maximum line length, limited functions per file).
   - **Solution:** Refactored the codebase into modular functions and files, improving both organization and readability.

---

## Why This Project Matters

This project reflects my:
- Proficiency in **C programming** and low-level system calls.
- Ability to deliver a complex software system while meeting strict requirements.
- Attention to detail in coding standards, error handling, and resource management.
- Capacity to work effectively under constraints, ensuring both functionality and performance.

Minishell is more than a coding exercise; it is a demonstration of my dedication to quality and my ability to tackle challenging technical problems.

---

## How to Use

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd minishell
   ```

2. Build the project:
   ```bash
   make
   ```

3. Run the shell:
   ```bash
   ./minishell
   ```

4. Clean up build files:
   ```bash
   make fclean
   ```

---

## Future Improvements

- **Error Messages:** Provide more descriptive error outputs for complex scenarios.
- **Advanced Features:** Add support for advanced Bash functionalities like job control and advanced scripting syntax.
- **Cross-Platform Compatibility:** Enhance portability to support more environments beyond UNIX-like systems.

---

## Conclusion

Minishell is a fully functional project that demonstrates my technical expertise and problem-solving abilities. It is built with attention to detail, robustness, and a strong emphasis on learning best practices in software engineering. I invite you to explore the codebase to see these principles in action.

