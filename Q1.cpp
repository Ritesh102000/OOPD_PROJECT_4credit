#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <filesystem>

using namespace std;

namespace fs = filesystem;

class LsCommand {
public:
    void execute(const vector<string>& args) {
        bool reverseOrder = false;
        bool listSize = false;
        bool sortBySize = false;

        // Parse command-line options
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "-r") {
                reverseOrder = true;
            } else if (args[i] == "-s") {
                listSize = true;
            } else if (args[i] == "-S") {
                sortBySize = true;
            } else if (args[i] == "--help") {
                displayLsHelp();
                return;
            }
        }

        DIR* dir;
        struct dirent* entry;
        vector<string> files;

        // Open and read the directory
        if ((dir = opendir(".")) != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                files.push_back(entry->d_name);
            }
            closedir(dir);

            // Apply options
            if (reverseOrder) {
                reverse(files.begin(), files.end());
            }

            if (sortBySize) {
                sort(files.begin(), files.end(), [](const string& a, const string& b) {
                    struct stat fileStatA, fileStatB;
                    stat(a.c_str(), &fileStatA);
                    stat(b.c_str(), &fileStatB);
                    return fileStatA.st_size > fileStatB.st_size;
                });
            }

            // Print the result
            for (const string& file : files) {
                if (listSize) {
                    struct stat fileStat;
                    stat(file.c_str(), &fileStat);
                    cout << fileStat.st_size << "\t";
                }
                cout << file << endl;
            }
        } else {
            perror("ls");
        }
    }

private:
    // Function to display help information for ls command
    void displayLsHelp() {
        cout << "ls: List directory contents" << endl;
        cout << "Usage: ls [options]" << endl;
        cout << "Options:" << endl;
        cout << "  -r\tList in reverse order" << endl;
        cout << "  -s\tList file size" << endl;
        cout << "  -S\tSort by file size" << endl;
        cout << "  --help\tDisplay help information" << endl;
    }
};

class MvCommand {
public:
    void execute(const vector<string>& args) {
        bool forceOverwrite = false;
        bool interactivePrompt = false;

        // Parse command-line options
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "-f") {
                forceOverwrite = true;
            } else if (args[i] == "-i") {
                interactivePrompt = true;
            } else if (args[i] == "--help") {
                displayMvHelp();
                return;
            }
        }

        // Check for the correct number of arguments
        if (args.size() < 3) {
            cerr << "mv: missing source or destination file" << endl;
            return;
        }

        const char* source = args[1].c_str();
        const char* destination = args[2].c_str();

        // Check if the destination file exists and if interactive prompt is enabled
        if (interactivePrompt && access(destination, F_OK) == 0) {
            char response;
            cout << "mv: overwrite '" << destination << "'? (y/n): ";
            cin >> response;

            if (response != 'y') {
                return;
            }
        }

        // Perform the move operation
        if (rename(source, destination) != 0) {
            if (forceOverwrite) {
                // If force overwrite is enabled, remove the destination file and try again
                remove(destination);
                if (rename(source, destination) != 0) {
                    perror("mv");
                }
            } else {
                perror("mv");
            }
        }
    }

private:
    // Function to display help information for mv command
    void displayMvHelp() {
        cout << "mv: Move or rename files" << endl;
        cout << "Usage: mv [options] <source> <destination>" << endl;
        cout << "Options:" << endl;
        cout << "  -f\tForce move by overwriting destination file without prompt" << endl;
        cout << "  -i\tInteractive prompt before overwrite" << endl;
        cout << "  --help\tDisplay help information" << endl;
    }
};

class RmCommand {
public:
    void execute(const vector<string>& args) {
        bool interactivePrompt = false;

        // Parse command-line options
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "-i") {
                interactivePrompt = true;
            } else if (args[i] == "--help") {
                displayRmHelp();
                return;
            }
        }

        // Check for the correct number of arguments
        if (args.size() < 2) {
            cerr << "rm: missing file operand" << endl;
            return;
        }

        const char* file = args[1].c_str();

        // Check if interactive prompt is enabled
        if (interactivePrompt) {
            char response;
            cout << "rm: remove '" << file << "'? (y/n): ";
            cin >> response;

            if (response != 'y') {
                return;
            }
        }

        // Perform the remove operation
        if (remove(file) != 0) {
            perror("rm");
        }
    }

private:
    // Function to display help information for rm command
    void displayRmHelp() {
        cout << "rm: Remove files or directories" << endl;
        cout << "Usage: rm [options] <file>" << endl;
        cout << "Options:" << endl;
        cout << "  -i\tPrompt before every removal" << endl;
        cout << "  --help\tDisplay help information" << endl;
    }
};

class CpCommand {
public:
    void execute(const std::vector<std::string>& args) {
        // Parse command-line options
        bool recursiveCopy = false;
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "--help") {
                displayCpHelp();
                return;
            } else if (args[i] == "-r") {
                recursiveCopy = true;
            }
        }

        // Check for the correct number of arguments
        if (args.size() < 3) {
            std::cerr << "cp: missing source or destination file" << std::endl;
            return;
        }

        const std::string& source = args[1];
        const std::string& destination = args[2];

        // Check if the source is a directory
        if (fs::is_directory(source)) {
            copyDirectory(source, destination, recursiveCopy);
        } else {
            copyFile(source, destination);
        }
    }

private:
    // Function to display help information for cp command
    void displayCpHelp() {
        std::cout << "cp: Copy files" << std::endl;
        std::cout << "Usage: cp [options] <source> <destination>" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -r\tCopy directories recursively" << std::endl;
        std::cout << "  --help\tDisplay help information" << std::endl;
    }

    // Function to copy a file
    void copyFile(const std::string& source, const std::string& destination) {
        std::ifstream sourceFile(source, std::ios::binary);
        std::ofstream destFile(destination, std::ios::binary);

        destFile << sourceFile.rdbuf();
    }

    // Function to copy a directory
    void copyDirectory(const std::string& source, const std::string& destination, bool recursive) {
        // Create the destination directory if it doesn't exist
        fs::create_directories(destination);

        // Iterate over each file in the source directory and copy it to the destination
        for (const auto& entry : fs::directory_iterator(source)) {
            const std::string& sourceFile = entry.path();
            const std::string& destFile = fs::path(destination) / entry.path().filename();

            // Recursively copy directories if the option is enabled
            if (fs::is_directory(sourceFile) && recursive) {
                copyDirectory(sourceFile, destFile, true);
            } else {
                copyFile(sourceFile, destFile);
            }
        }
    }
};

class CdCommand {
public:
    void execute(const vector<string>& args) {
        // Parse command-line options
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "--help") {
                displayCdHelp();
                return;
            }
        }

        // Check if there is an argument provided
        if (args.size() > 1) {
            if (chdir(args[1].c_str()) != 0) {
                perror("cd");
            }
        } else {
            cerr << "cd: missing argument" << endl;
        }
    }

private:
    // Function to display help information for cd command
    void displayCdHelp() {
        cout << "cd: Change directory" << endl;
        cout << "Usage: cd [options] <directory>" << endl;
        cout << "Options:" << endl;
        cout << "  --help\tDisplay help information" << endl;
    }
};

class Shell {
public:
    void run() {
        string input;
        while (true) {
            cout << "> ";
            getline(cin, input);

            // Tokenize the input into a vector of strings
            istringstream iss(input);
            vector<string> args{
                istream_iterator<string>{iss},
                istream_iterator<string>{}
            };

            // Check if the 'exit' command is given to exit the shell
            if (!args.empty() && args[0] == "exit") {
                break;
            }

            // Check if there is any valid command to execute
            if (!args.empty()) {
                const char* command = args[0].c_str();

                // Execute the corresponding command based on the input
                if (strcmp(command, "ls") == 0) {
                    LsCommand lsCommand;
                    lsCommand.execute(args);
                } else if (strcmp(command, "mv") == 0) {
                    MvCommand mvCommand;
                    mvCommand.execute(args);
                } else if (strcmp(command, "rm") == 0) {
                    RmCommand rmCommand;
                    rmCommand.execute(args);
                } else if (strcmp(command, "cp") == 0) {
                    CpCommand cpCommand;
                    cpCommand.execute(args);
                } else if (strcmp(command, "cd") == 0) {
                    CdCommand cdCommand;
                    cdCommand.execute(args);
                } else {
                    cerr << "Command not recognized: " << command << endl;
                }
            }
        }
    }
};

int main() {
    Shell shell;
    shell.run();

    return 0;
}
