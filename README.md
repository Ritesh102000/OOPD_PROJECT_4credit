# OOPD_PROJECT_4credit
OOPD_PROJECT
Quest 1: Simple Shell with File Management Commands
Overview

This simple shell program provides a basic command-line interface with file management commands, including ls, mv, rm, cp, and cd. The shell allows users to navigate directories, list directory contents, move or rename files, remove files or directories, copy files or directories, and change the current directory.
Usage

To compile and run the program, use the following commands:

bash

g++ shell.cpp -o shell
./shell

Commands
1. ls - List Directory Contents

bash

ls [options]

Options:

    -r: List in reverse order
    -s: List file size
    -S: Sort by file size
    -R or --recursive: List subdirectories recursively
    --help: Display help information

2. mv - Move or Rename Files

bash

mv [options] <source> <destination>

Options:

    -f: Force move by overwriting destination file without prompt
    -i: Interactive prompt before overwrite
    --help: Display help information

3. rm - Remove Files or Directories

bash

rm [options] <file>

Options:

    -i: Prompt before every removal
    --recursive: Remove directories and their contents recursively
    --help: Display help information

4. cp - Copy Files

bash

cp [options] <source> <destination>

Options:

    -r or --recursive: Copy directories recursively
    --help: Display help information

5. cd - Change Directory

bash

cd [options] <directory>

Options:

    --help: Display help information

6. exit - Exit the Shell

bash

exit

Notes

    The shell continuously prompts for commands until the user enters exit.
    Use the --help option with any command to display help information.

Script Explanation

This Bash script creates three directories (dir1, dir2, and dir3) with specific file configurations.
1. Create dir1 with 100 Files of 1GB Each

bash

mkdir -p dir1
time for i in {1..100}; do dd if=/dev/zero of=dir1/file$i bs=1M count=1000; done

    Creates a directory named dir1.
    Uses a loop to create 100 files (file1 to file100) with a size of 1GB each.
    The time command is used to measure the time taken for the file creation.

2. Create dir2 with 10,000 Files of 10MB Each

bash

mkdir -p dir2
time for i in {1..10000}; do dd if=/dev/zero of=dir2/file$i bs=1M count=10; done

    Creates a directory named dir2.
    Uses a loop to create 10,000 files (file1 to file10000) with a size of 10MB each.
    The time command is used to measure the time taken for the file creation.

3. Create dir3 with Subdirectories and Files

bash

mkdir dir3
cd dir3
for i in {1..100}; do
    mkdir "subdir$i"
    cd "subdir$i"
    for j in {1..100}; do
        dd if=/dev/urandom of="file$j.txt" bs=10M count=1
    done
done

    Creates a directory named dir3.
    Changes into dir3 and uses a loop to create 100 subdirectories (subdir1 to subdir100).
    Changes into each subdirectory and uses another loop to create 100 files (file1.txt to file100.txt) with random content (using /dev/urandom) and a size of 10MB each.

4. Completion Message

bash

echo "Script execution complete."

    Outputs a message indicating that the script execution is complete.

This script provides a way to generate a significant amount of test data in the form of files and directories with different sizes. The time command is used to give an idea of the time taken for these operations.
Multi-threaded Implementation in File System Commands

In this implementation, we have enhanced the file system commands (ls, mv, rm, and cp) to support multi-threading, specifically in scenarios involving recursion. The number of threads created is determined intelligently based on the number of available CPU cores and the amount of workload.
Modifications in LsCommand Class

The listFilesRecursively function now utilizes multi-threading to improve performance when listing subdirectories recursively. Each subdirectory is processed concurrently using separate threads, allowing for better utilization of system resources.
Modifications in RmCommand Class

The removeDirectory function, responsible for removing directories recursively, has been enhanced to use multi-threading. Threads are created for each file or subdirectory within the directory being removed, enabling parallelized removal operations.
Modifications in CpCommand Class

The copyDirectory function has been enhanced to use multi-threading when copying directories recursively. Threads are created for each file within the source directory, enabling concurrent file copying for improved efficiency.
Multi-threading Strategy

    Threads are created intelligently based on the number of available CPU cores using std::thread::hardware_concurrency() and the amount of workload.
    Threads are joined before exiting the respective functions to ensure proper synchronization.