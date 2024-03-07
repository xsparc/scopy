This is a guide for installing the dependencies and setting up the environment for SCOPYv2 development. It is recommended to do these steps on a virtual machine since this has the potential to ruin the configuration of the system. The commands shown in this guide came from the [ci-for-scopy2](https://github.com/analogdevicesinc/scopy-mingw-build-deps/blob/ci-for-scopy2/docker/Dockerfile) dockerfile which can be visited and used as a reference. Most of the content of this guide is tailored fit to be executed using CMD or Windows Command Prompt which explains why there are some commands that are slightly different from the ones in the dockerfile. 

1. Install [MSYS2](https://www.msys2.org/) using the default settings. **Do not change the directory locations.**

2. **Only applicable to virtual machines.** Open **Microsoft Store** and search for **OpenCL, OpenGL, and Vulkan Compatibility Pack**. This would install the packages needed in rendering the graphics of the software. Without this, Scopy would just crash.

3. Create folder with the following directory *C:\msys64\home\docker*

4. Execute the following commands using the CMD and **not MINGW64**: 

        cd C:/msys64/home/docker/
        setx PATH C:\msys64\bin;C:\msys64\mingw64\bin;C:\msys64\usr\bin;%PATH%
        setx HOME C:\msys64\home\docker
        setx CHERE_INVOKING yes
        setx MSYSTEM MINGW64 

        C:\msys64\usr\bin\bash.exe -lc " "
        C:\msys64\usr\bin\bash.exe -lc "pacman --noconfirm -Syyu"
        C:\msys64\usr\bin\bash.exe -lc "pacman --noconfirm -Sy msys2-keyring"
        C:\msys64\usr\bin\bash.exe -lc "pacman --noconfirm -Su"

        C:\msys64\usr\bin\bash.exe -lc "pacman --noconfirm -Syuu" & C:\msys64\usr\bin\bash.exe -lc "pacman --noconfirm -Syuu" & C:\msys64\usr\bin\bash.exe -lc "pacman --noconfirm -Scc "

        C:\msys64\usr\bin\bash.exe -lc "pacman --noconfirm --needed -Sy git"
        C:\msys64\usr\bin\bash.exe -lc "git clone https://github.com/analogdevicesinc/scopy-mingw-build-deps --branch ci-for-scopy2"

        C:\msys64\usr\bin\bash.exe -lc "cd /home/docker/scopy-mingw-build-deps && ls && echo Building for x86_64 && ./init_staging.sh x86_64 OFF"

        C:\msys64\usr\bin\bash.exe -lc "cd /home/docker/scopy-mingw-build-deps && source build.sh x86_64 OFF && install_tools && install_deps && recurse_submodules"

        C:\msys64\home\docker\scopy-mingw-build-deps\is.exe /VERYSILENT /SP- /SUPPRESSMSGBOXES /NORESTART /LOG=C:\msys64\home\docker\iss.log /DIR=C:\innosetup

        C:\msys64\usr\bin\bash.exe -lc "cd /home/docker/scopy-mingw-build-deps && source build.sh x86_64 OFF && build_deps"

5. Clone **scopy** using tag **v2.0.0-alpha** to the directory: *C:\msys64\home\docker*
6. Install VS Code and install the **C/C++ Extension Pack**
7. On your VS Code, go to the toolbar on your left and locate the CMake tooltip. Use the tool.
8. In the tool, click the **Configure All Projects**. This will instruct CMake to build the scripts necessary in building the source code.
9. Go to the bottom toolbar and hit **Build All**.
10. **Only applicable to virtual machines.** Open **Microsoft Store** and search for **OpenCL, OpenGL, and Vulkan Compatibility Pack**. This would install the packages needed in rendering the graphics of the software. Without this, Scopy would just crash.
11. Copy the following dlls from the subfolders to the directory where Scopy.exe is located. **libscopy-common.dll, libscopy-core.dll, libscopy-gui.dll, libscopy-iioutil.dll, libscopy-pluginbase.dll**.
12. Launch Scopy.