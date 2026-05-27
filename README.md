# mysh - A Unix Shell in C

A lightweight Unix shell written in C from scratch, implementing core shell features including piping, I/O redirection, background execution, and built-in commands.

---

## Features

- **Command execution** - Run any program with arguments
- **Piping** - Chain commands with `|`
- **I/O Redirection** - `>`, `>>`, and `<` support
- **Background processes** - Run commands with `&`
- **Built-in commands** - `cd`, `exit`, `help`, `history`
- **Custom prompt** - Shows `user@host:cwd$`
- **Signal handling** - Ctrl+C does not kill the shell

---

## Build

```bash
make
```

## Run

```bash
./mysh
```

## Clean

```bash
make clean
```

---

## Usage Examples

```bash
# Basic command
ls -la

# Piping
ls | grep .c

# Output redirection
echo hello > file.txt

# Append
echo world >> file.txt

# Input redirection
cat < file.txt

# Background process
sleep 5 &

# Switch to previous directory
cd -

# View command history
history
```

---

## Built-in Commands

| Command | Description |
|--------|-------------|
| `cd [dir]` | Change directory, defaults to HOME |
| `cd -` | Switch to previous directory |
| `history` | Show command history |
| `help` | Show help message |
| `exit [code]` | Exit the shell with optional code |

---

## Project Structure

```
simple-shell/
├── include/
│   └── shell.h        # Shared headers and declarations
├── src/
│   ├── main.c         # Entry point and main loop
│   ├── shell.c        # Core execution and piping logic
│   ├── builtins.c     # Built-in command implementations
│   └── parser.c       # Input parsing and tokenization
└── Makefile
```

---

## Concepts Covered

- `fork`, `execvp`, `waitpid` for process management
- `pipe`, `dup2` for piping between processes
- `open`, `dup2` for I/O redirection
- `signal` for Ctrl+C handling
- Memory management with `strdup` and `free`

---

## License

MIT
```