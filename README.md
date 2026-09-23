# ls-clone

A lightweight, POSIX-compliant implementation of the classic `ls` system utility written from scratch in C. This project interacts directly with the Linux file system using low-level system calls to read directory streams and extract file metadata.

## Features

- **`-a` (All):** Lists all directory entries, including hidden files and folders starting with a dot (`.`).
- **`-l` (Long Listing):** Displays verbose file metadata including file types, permissions, link counts, owner/group names, sizes in bytes, and last modification times.
- **`-R` (Recursive):** Recursively traverses down subdirectories using a POSIX-compliant multi-pass structure to prevent infinite loops on `.` and `..`.

## How it Works

The utility avoids high-level wrapper functions and directly handles core UNIX primitives:
- `getopt()` for robust command-line argument and switch tracking.
- `opendir()`, `readdir()`, and `rewinddir()` for manipulating directory data streams.
- `lstat()` for inspecting exact symlinks and file details without triggering unintended link targets.
- `getpwuid()` and `getgrgid()` for translating numeric system IDs into human-readable user and group attributes.

## Compilation & Usage

Compile using any standard C compiler (like `gcc`):

```bash
gcc lsclone.c -o lsclone
```

### Examples

Run a standard directory listing:
```bash
./lsclone
```

Run a verbose, recursive, hidden-inclusive pass over a specific target folder:
```bash
./lsclone -alR /path/to/directory
```
