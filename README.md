
# Minishell 42

## Overview

Minishell is a simplified Unix shell inspired by the 42 school curriculum project. The aim is to create a basic shell capable of handling command executions, redirections, and a few built-in commands.

## Features

1. **Command Execution**: Execute basic commands entered by the user.
2. **Redirections**: Support for output (`>`, `>>`) and input (`<`) redirections.
3. **Pipes**: Execute commands in a pipeline using the `|` operator.
4. **Built-in Commands**: Implement commonly used built-in commands like `cd`, `exit`, etc.
5. **Signal Handling**: Gracefully handle signals such as `SIGINT` and `SIGQUIT`.
6. **Environment Variables**: Support for setting, reading, and using environment variables.

## How to Build and Run

```bash
# To compile the minishell
make

# To run the minishell
./minishell
```

## Usage

After launching the minishell, you can enter commands just like you would in any Unix shell:

```bash
minishell# ls -l
minishell# echo Hello World!
minishell# cat file.txt | grep "search_term"
```
