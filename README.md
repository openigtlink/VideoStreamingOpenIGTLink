Video Streaming with OpenIGTLink and OpenH264 Library
=======================

This project is a demonstration of video streaming using OpenIGTLink protocal [OpenIGTLink Web Page](http://openigtlink.org/) and OpenH264 library [OpenH264 Web Page](http://www.openh264.org/). 
The project is consists of two sub-projects, a VideoStreamServer and a VideoStreamReceiver, the former reads a video file of YUV format and compresses the file into bit stream, while the latter receives the bit stream and decompresses the bit stream back into YUV file. The communication between them is achieved via the OpenIGTLink protocal.

Build Instruction
-----------------

## Linux / Mac OS X
First, obtain the source code from the repository using Git. To simply download the code, run the following command from a terminal:

    $ git clone https://github.com/leochan2009/VideoStreamingOpenIGTLink.git

Then configure using CMake. The library requires CMake version higher than 2.8.

    $ mkdir VideoStreamingOpenIGTLink-build
    $ cd VideoStreamingOpenIGTLink-build
    $ cmake ../VideoStreamingOpenIGTLink
    $ make

If all went OK you will have the executable and the library:
* ../VideoStreamingOpenIGTLink-build/VideoStreamReceiver/VideoStreamReceiver.exec
* ../VideoStreamingOpenIGTLink-build/VideoStreamServer/VideoStreamServer.exec

You may install the library into your disk (optional). The default target directory is /usr/local, but you can configure it from the CMake configuration screen. To install the files, run

    $ make install

You might need super user access.

## Windows
* Download the source code from Git repository.
  * URL of repository: https://github.com/leochan2009/VideoStreamingOpenIGTLink.git
* Run CMake
  * Where is the source code: C:\Devel\VideoStreamingOpenIGTLink
  * Where to build the binaries: C:\Devel\VideoStreamingOpenIGTLink-build
  * Click "Configure" and select your compiler (usually just click "OK")
  * Message: "Build directory does not exit, should I create it?" - click "OK"
  * Click "Configure"
  * Click "OK" to close CMake
* Start Visual C and compile the project (C:\Devel\VideoStreamingOpenIGTLink-build\OpenIGTLink.sln)
If all went OK you will have the executable and the library:
* C:\Devel\VideoStreamingOpenIGTLink-build\VideoStreamReceiver\bin\debug\VideoStreamReceiver.exe
* C:\Devel\VideoStreamingOpenIGTLink-build\VideoStreamServer\bin\debug\VideoStreamServer.exe

Demonstration Instruction
-------------------------
 Prepare the data in the format of YUV420, which is currently the only supported format.
 Open A terminal or command window, run the VideoStreamServer.exec first, which take four auguments, an example in the mac terminal will be:

    $  ./VideoStreamServer 18944 ../OpenH264/res/CiscoVT2people_320x192_12fps.yuv 320 192
  
 Then run the VideoStreamReceiver.exec in another terminal window, an example will be:

    $  ./VideoStreamReceiver localhost  18944 10 100
  
The decoded output image with name "outputDecodedVideo.yuv" will be in the same directory as the VideoStreamReceiver.exec
To view the decodedvideo, you could download a YUV player from this repository [YUV Player](https://github.com/IENT/YUView.git)

License
-------
The code is distributed as open source under [the new BSD liccense](http://www.opensource.org/licenses/bsd-license.php).

Acknowledgement
-------
* The project is based on the work of OpenIGTLink and OpenH264 Communities.
* Documentation is based on the README.md file from OpenIGTLink.
* We also use The video display from the Institut für Nachrichtentechnik from RWTH Aachen University

