# Minishell

`Minishell` is a simplified UNIX shell implemented in C. This group project replicates the behavior of a real shell, including parsing commands, handling pipes, redirections, environment variables, and launching executables.

The goal is to understand how a shell works under the hood, practice process and signal management, and build a robust, interactive command-line interface.

## Project Goal

- Recreate basic behavior of a Bash-like shell
- Support built-in commands such as `cd`, `echo`, `env`, `exit`, `export`, `pwd`, and `unset`
- Handle input/output redirections (`>`, `>>`, `<`, `<<`)
- Execute external binaries using `execve` and the `PATH`
- Support command pipelines (`|`) and multiple file descriptors
- Implement proper signal handling (`Ctrl+C`, `Ctrl+\`)
- Manage environment variables dynamically

## Learning Outcomes

- Understanding the lifecycle of a **UNIX process** (forking, executing, waiting)
- Working with **pipes** and **file descriptors**
- Handling **signals** (`SIGINT`, `SIGQUIT`) properly in both parent and child processes
- Implementing a **command parser** with **lexing and tokenization**
- Dynamically managing **environment variables**
- Managing memory carefully and debugging leaks
- Building a **REPL** (read-eval-print loop) for an interactive program

## Project Challenges

- Parsing complex shell input including quotes, variable expansion, and redirections
- Handling **heredocs** with correct behavior (including signal handling inside heredoc input)
- Executing commands in the correct order, especially in **pipelines**
- Replicating Bash-like behavior while respecting 42's restricted function list
- Avoiding memory leaks and properly freeing everything on each cycle
- Ensuring **signal behavior** matches expectations (e.g., interrupting only foreground processes)

## Example Commands

minishell$ echo Hello World
minishell$ export USER=batuhan
minishell$ ls -l | grep minishell
