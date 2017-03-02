# llvm-env

The aim is to create an environment for development of epiphany llvm tools

- build script tools-src/build-clang-from-scratch.sh with download, build and
install clang (Debug version).
- build script proj/first-steps/build.sh will build a simple test program using
clang just built.  The script also creates the necessary files to use Eclipse
to step through the code.

### To use Eclipse

I use the llvm-env/proj folder as the Eclipse workspace, adjust the paths in the following description and in build.sh to match your desired configuration.
- In Eclipse create an empty "Makefile project" in the workspace with name first-stepsE.
- In Build Configurations ensure that there is a Debug configuration.
- Switch off automatically generating Makefile. Navigate to {Properties; C/C++ Build} - uncheck Generate Makefiles automatically.
- Select the Build directory. Navigate to {Properties; C/C++ Build} - Set build directory something like ${workspace_loc:/first-stepsE}/Debug.
- Link the source to the project in Navigate to {Properties; C/C++ General; Paths and Symbols; Source Location} - add WORKSPACE_LOC/first-steps.
- Add additional include paths and library paths as necessary. Navigate to {Properties; C/C++ General; Paths and Symbols; Includes} - Add full path to llvm-env/local/include.
- Now in a shell run the shell script llvm-env/proj/first-steps/build.sh to create the Eclipse project data.
- In Eclipse refresh the indexes and refresh the project to see new files created by build.sh.
When debugging with Eclipse use Debug/src/main/main as the application to debug and create a new environment variable LD_LIBRARY ${workspace_loc}/../local/lib.

Note the build.sh script selects Eclipse CDT4, you may need to alter this to match your version of Eclipse:

```bash
cmake $SRCDIR -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_VERSION=4.3 -DCMAKE_INSTALL_PREFIX=$INSROOTDIR -DCMAKE_BUILD_TYPE=$BUILDTYPE -DCMAKE_USER_MAKE_RULES_OVERRIDE=$SRCDIR/ClangOverrides.txt
```
