# CSE 314: Operating Systems Sessional

This repository contains offline assignments for **CSE 314: Operating Systems Sessional**. The work covers shell scripting, xv6 system calls, xv6 scheduling, and inter-process communication using threads and synchronization primitives.

## Contents

| Offline | Topic | Description |
| --- | --- | --- |
| `Offline_01` | Shell Scripting | Organizes submitted code files by language and student ID, then optionally compiles/runs them against test cases. |
| `Offline_02` | xv6 System Calls | Adds custom xv6 system calls for tracing, syscall history, and stopping QEMU execution. |
| `Offline_03` | xv6 Scheduling | Implements a two-level MLFQ scheduler with lottery scheduling, round-robin scheduling, priority boosting, and process info system calls. |
| `Offline_04` | IPC Assignment | Simulates a concurrent report submission workflow using POSIX threads, semaphores, and reader-writer synchronization. |

## Repository Structure

```text
CSE_314-main/
├── README.md
├── Offline_01/
│   ├── CSE 314 Jan 2023 - Shell Scripting Assignment.pdf
│   └── 1905038/
│       └── organize.sh
├── Offline_02/
│   ├── Offline-2-spec.pdf
│   └── 1905038.patch
├── Offline_03/
│   ├── _CSE-314--assignment-03.pdf
│   └── 1905038.patch
└── Offline_04/
    ├── IPC Offline.pdf
    └── 1905038/
        └── 1905038.cpp
```

## Offline Summaries

### Offline 01: Shell Scripting

The script `organize.sh` processes zipped student submissions. It extracts each submission, searches recursively for a C, Java, or Python source file, organizes files into language-specific folders, and can execute submissions against provided test cases.

### Offline 02: xv6 System Calls

The patch adds new xv6 system calls:

- `trace`: traces a selected system call for a process.
- `history`: reports syscall usage statistics.
- `stop`: signals QEMU to halt.

### Offline 03: xv6 Scheduling

The patch modifies xv6 scheduling behavior by adding:

- A two-level Multilevel Feedback Queue.
- Lottery scheduling in the top queue.
- Round-robin scheduling in the lower queue.
- Priority boosting.
- `settickets` and `getpinfo` system calls.

### Offline 04: IPC Assignment

The C++ program models a science project report submission system.Here, Students use print stations, group leaders use binding stations, and library staff read from a shared entry book while group leaders write to it. The implementation uses `pthread`, POSIX semaphores, and a reader-writer lock.

## Notes

- xv6-related submissions are provided as `.patch` files and should be applied to the corresponding xv6 source tree.
- Assignment specifications are included as PDF files inside each offline folder.
