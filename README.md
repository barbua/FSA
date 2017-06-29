# FSA
A fast feature selection method described in the paper:
A. Barbu, Y. She, L. Ding, G. Gramajo. Feature Selection with Annealing for Computer Vision and Big Data Learning. IEEE PAMI, 39, No. 2, 272–286, 2017
https://arxiv.org/abs/1310.2880

Build Status Travis for Linux 
[![Build Status](https://travis-ci.org/MarquiseRosier/FSA.svg?branch=master)](https://travis-ci.org/MarquiseRosier/FSA)

Build Status AppVeyor for Windows
[![Build status](https://ci.appveyor.com/api/projects/status/g3u5u382kdtfms0u?svg=true)](https://ci.appveyor.com/project/MarquiseRosier/fsa-ww759)

# GETTING STARTED

In order to get started the following things you need the following. 

-HARDWARE
 - COMPUTER WITH WINDOWS OR LINUX
 
-SOFTWARE
  - CMAKE (VERSION 2.8 OR UP)
  - PYTHON (VERSION 2.7 OR UP)
  - NUMPY (INSTALLED INTO PYTHON) 
  - A C++ COMPILER THAT SUPPORTS C++11 OR C++0X
  - Install Git 
  
 TO OBTAIN A COPY OF OUR SOFTWARE, FOLLOW THE FOLLOWING INSTRUCTIONS. 

You can either download FSA by opening navigating to the top right corner and clicking "Clone or Download"
or running 
- git clone https://github.com/barbua/FSA.git

# INSTALLING OUR SOFTWARE

Once you've obtained a copy of our software. 
If you obtained the software by hitting "Clone or Download" it will come as FSA.zip. 
Unzip this file however you can and navigate into the unzipped folder named FSA.
Otherwise, if you have cloned the repository using git clone https://github.com/barbua/FSA.git
you can go right into the FSA folder. 

The folder hierarchy should look like this when you enter FSA Folder                                                                                                                                                    
    -bin                        (folder)                                                                                     
    -include                    (folder)                                                                                     
    -src                        (folder)                                                                                     
    -cmake                      (folder)                                                                                     
    CMakeLists.txt              (file)                                                                                       
    .                                                                                                                         
    .(additional files)                                                                                                       
                                                                                                                              
 # WINDOWS
 
 Enter the bin folder in your FSA folder
 It should be empty. 

Run the following commands depending on the Command line interface you are using. 
Command prompt: cmake ..\
Power Shell: cmake ..\
Git Bash: cmake ../

This will generate solution files if you are using Visual Studios as your generator
(To learn more about generators https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html)

If you have an .sln file in the bin directory go ahead and click on it and open it up with Visual Studios. 
There you will hit Build which will create the dynamic library in your bin directory!

  
