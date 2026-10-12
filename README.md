*This project has been created as part of the 42 curriculum by mvasquez.*

# Get Next Line

## Description

Get Next Line is a C function that reads a file descriptor one line at a time.

Unlike `read`, which returns a number of bytes, `get_next_line` assembles those
bytes into complete lines. Successive calls return successive lines until the
input is exhausted.

The project focuses on file descriptors, static variables, buffered reading
and dynamic memory management.

The implementation uses a single static string to preserve unread data between
calls. Each call returns a newly allocated line, including the newline
character when one is present.

---

## Instructions

### Requirements

- A C compiler such as `cc`, GCC or Clang.
- A POSIX environment providing `read`, `open` and `close`.
- The project header, `get_next_line.h`.

**Repository note:** the current repository contains the two source files but
does not include `get_next_line.h`. Restore the original header before using
the compilation commands below. Both source files include it.

The header must provide the standard declarations required by the sources,
the `BUFFER_SIZE` definition and the declarations of `get_next_line`,
`ft_strchr`, `ft_strlen` and `join_string`.

### Clone

```bash
git clone https://github.com/Matias171314/42-get_next_line.git
cd 42-get_next_line
```

### Compilation

This repository provides a function rather than a standalone executable.
Create a `main.c` using the example below, then compile it alongside the two
source files:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
main.c get_next_line.c get_next_line_utils.c -o gnl_test
```

The read buffer size can be changed at compilation time:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 \
main.c get_next_line.c get_next_line_utils.c -o gnl_test
```

`BUFFER_SIZE` controls the number of bytes requested in each `read` call.
It does not limit the length of the returned line.

---

## Function Interface

```c
char *get_next_line(int fd);
```

| Item | Description |
|---|---|
| `fd` | File descriptor to read from. |
| Return value | A newly allocated, null-terminated line. |
| Newline | Included in the returned string when present in the input. |
| Final line | Returned even when the file does not end with a newline. |
| `NULL` | No remaining line, or a detected error. |
| Ownership | The caller must free every returned line. |

End of file is reported by `read` returning `0`. The character `'\0'`
terminates a C string; it is not an EOF marker.

The function returns `NULL` for a negative file descriptor or a non-positive
`BUFFER_SIZE`.

---

## Usage Example

Save this example as `main.c`. It reads a file when a path is provided, or
standard input when no argument is given.

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int     fd;
    char    *line;

    if (argc > 2)
    {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return (1);
    }
    fd = STDIN_FILENO;
    if (argc == 2)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            return (1);
        }
    }
    line = get_next_line(fd);
    while (line != NULL)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    if (argc == 2)
        close(fd);
    return (0);
}
```

### Read a file

```bash
printf 'First line\nSecond line\nLast line without newline' > text.txt
./gnl_test text.txt
```

Expected output:

```text
First line
Second line
Last line without newline
```

### Read standard input

```bash
printf 'Hello\n42\n' | ./gnl_test
```

For interactive input:

```bash
./gnl_test
```

On a Unix terminal, press **Ctrl+D** on an empty input line to signal EOF.

Standard input is descriptor `0` (`STDIN_FILENO`). Descriptor `1` is standard
output.

This example stops when `get_next_line` returns `NULL`; the interface does
not distinguish EOF from a read or allocation error.

---

## Reading Model

The function separates reading, line extraction and preservation of the
remaining data.

| Stage | Function | Responsibility |
|---|---|---|
| Read | `ft_read_buffer` | Append chunks until a newline is found or `read` reaches EOF. |
| Extract | `ft_get_line` | Allocate and copy the next line, including its newline when present. |
| Preserve | `ft_get_rest` | Copy the unread suffix and free the previous saved string. |
| Coordinate | `get_next_line` | Validate inputs and update the static state between calls. |

For example, if one read produces `"Hello\n42\n"`, the first call returns
`"Hello\n"` and saves `"42\n"`. The next call can return that saved line
without reading more bytes from the descriptor.

### Static state

The local declaration:

```c
static char *save;
```

keeps its value between function calls. This preserves bytes read beyond the
end of the current line.

This implementation has one shared saved string. It is intended for sequential
reading of a single descriptor until exhaustion. It does not maintain separate
states for interleaved reads from multiple descriptors and is not thread-safe.

### Utility functions

| Function | Role |
|---|---|
| `ft_strchr` | Check whether a saved string contains a newline. |
| `ft_strlen` | Calculate string length, treating `NULL` as an empty string. |
| `join_string` | Allocate a combined string and free the previous saved string after a successful join. |

The helper `ft_strchr` is specialized for newline detection and returns a
boolean-style value. It does not have the standard `strchr` interface.

---

## Memory Management

The function dynamically allocates its read buffer, returned lines and saved
data.

- The temporary read buffer is freed after reading.
- A successfully replaced saved string is freed during joining or remainder
  extraction.
- Returned lines belong to the caller and must be freed after use.
- Reading until `NULL` allows normal EOF cleanup to complete.

Stopping before EOF may leave unread data in the static string. The current
interface does not provide a dedicated reset or cleanup function.

Allocation-failure paths require additional review; this README does not claim
that the implementation is leak-free under every failure condition.

---

## Project Structure

| File | Purpose |
|---|---|
| `get_next_line.c` | Main function and reading, extraction and remainder helpers. |
| `get_next_line_utils.c` | Newline detection, string length and string joining. |
| `README.md` | Project overview, usage and implementation notes. |
| `get_next_line.h` | Required header; currently absent from the repository. |

No Makefile, test driver or multiple-descriptor bonus implementation is
included in the current repository.

---

## Testing

After restoring the header and compiling the example, useful checks include:

| Case | Expected behavior |
|---|---|
| Empty file | Return `NULL` immediately. |
| A single newline | Return `"\n"`, then `NULL`. |
| Consecutive newlines | Return each empty line separately. |
| Line longer than `BUFFER_SIZE` | Assemble the line across multiple reads. |
| Multiple lines in one read | Preserve the remaining lines for later calls. |
| Final line without a newline | Return the remaining text before `NULL`. |
| Invalid descriptor | Return `NULL` when the read fails. |
| Standard input | Read lines from descriptor `0`. |

Repeat the same inputs with `BUFFER_SIZE` values such as `1`, `4`, `42`
and `1024`. The returned text should remain the same.

### Norminette

```bash
norminette get_next_line.c get_next_line_utils.c get_next_line.h
```

### Valgrind

```bash
valgrind --leak-check=full --show-leak-kinds=all ./gnl_test text.txt
```

These are suggested validation commands, not a claim that all checks have
already passed on the current repository.

---

## Resources

Useful manual pages for the concepts used in this project:

```bash
man 2 read
man 2 open
man 2 close
man 3 malloc
man 3 free
```

Other references include the Get Next Line subject and the 42 Norm.

---

## Use of AI

During development, AI tools were used as a learning aid to explain:

- file descriptors and how the operating system manages open files;
- the difference between local, global and static variables in C;
- heap allocation and memory-leak prevention.

As described in the original README, AI was used to understand these concepts
without directly generating the final implementation.

AI assistance was also used to revise this documentation, correct the usage
examples and describe the implementation from the source files.

---

## Author

**mvasquez** · [Matias171314](https://github.com/Matias171314)
