# Installation

## Easy way

Interface of Nature (ION) is only available for Windows. The easiest way to
install it is by downloading ION installer. The installer will undertake all
necessary actions in order to setup and configure ION on your machine.

## Compilation from source

ION is written with commercial IDE C++ Builder. It makes compilation form
source more difficult. Version XE 2 or later is required.

1. You should download or clone ION repository. To clone the repository
execute the following command:

    ```
    $ git clone https://github.com/mrkkrp/ion.git master
    ```

2. If your version of C++ Builder is different form XE 5, or if you've
installed the IDE in non-default directory, you have to edit `make.cmd` in
order to correct some pathes on top of the file.

3. Now you're ready to run `make.cmd` script. This script will compile
entire ION system and form ready-to-use application in `bar`
subdirectory. However, for normal functioning one should generate installer
and then install ION with the installer.

4. To generate installer we use Inno Setup. Thus, you should install Inno
Setup (it is free software). Use `ion.iss` file to produce ION installer.

5. Run the installer.
