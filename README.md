# MiniGit-for-DSA-class

Steps on how to Build and Run the Project on VS code.

1. Install CMake

Download and install from: https://cmake.org/download/

2. Open a terminal in the root of the project folder (where CMakeLists.txt is located).

3. Create a build directory.

command: mkdir build

4. Change directory to the build folder created.
 
commnd: cd build 

5. Generate build files with CMake:

command: cmake ..

6. Build the project:

command: cmake --build .

7. Run the program:

Navigate to the folder where MiniGitProject.exe was created (usually inside build/Debug or build/).

Run the executable:

command: ./MiniGitProject.exe
