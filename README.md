# Microshell Exam Question

## Description

### Allowed Functions
- `malloc`, `free`, `write`, `close`, `fork`, `waitpid`, `signal`, `kill`, `exit`, `chdir`, `execve`, `dup`, `dup2`, `pipe`, `strcmp`, `strncmp`

### Program Requirements
- The program should execute shell commands based on the arguments provided.
- Executable paths will be absolute or relative; no path construction from `PATH` is allowed.
- The program must support `|` (pipe) and `;` (command separator).
- A `|` should never be immediately followed or preceded by another `|` or `;`.
- The built-in `cd` command must be implemented, accepting only a single path argument.
    - If given incorrect arguments, print `error: cd: bad arguments` to `STDERR` followed by `\n`.
    - If `cd` fails, print `error: cd: cannot change directory to path_to_change` to `STDERR` followed by `\n`, replacing `path_to_change` with the argument.
    - `cd` should not be immediately followed or preceded by `|`.
- No support is needed for wildcards (`*`, `~`, etc.).
- No environment variable management (`$VAR`, etc.).
- If a system call (except `execve` and `chdir`) fails, print `error: fatal` to `STDERR` followed by `\n` and exit immediately.
- If `execve` fails, print `error: cannot execute executable_that_failed` to `STDERR` followed by `\n`, replacing `executable_that_failed` with the failed executable's path.
- The program should handle more than hundreds of `|`, even with a file descriptor limit of fewer than 30.

## Example Usage
```sh
$> ./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
microshell
i love my microshell
$>
```

```sh
$> ./microshell
```

## Hints
- Pass the environment variables to `execve`.
- Avoid leaking file descriptors.

