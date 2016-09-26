/*=========================================================================
 
 Program:   OpenIGTLink
 Language:  C++
 
 Copyright (c) Insight Software Consortium. All rights reserved.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.
 
 =========================================================================*/

#include <fstream>
#include <climits>
#include <cstring>
#include <stdlib.h>
#include "api/svc/codec_api.h"
#include "api/svc/codec_app_def.h"
#include "utils/FileInputStream.h"
#include "api/sha1.c"

#include "igtlOSUtil.h"
#include "igtlMessageHeader.h"
#include "igtlVideoMessage.h"
#include "igtlServerSocket.h"
#include "igtlMultiThreader.h"

#include "H264Decoder.h"


int ReceiveVideoData(igtl::ClientSocket::Pointer& socket, igtl::MessageHeader::Pointer& header, ISVCDecoder* decoder_, const char* outputFileName);

int main(int argc, char* argv[])
{
  //------------------------------------------------------------
  // Parse Arguments
  
  if (argc != 5) // check number of arguments
  {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <hostname> <port> <fps>"    << std::endl;
    std::cerr << "    <hostname> : IP or host name"                    << std::endl;
    std::cerr << "    <port>     : Port # (18944 in default)"   << std::endl;
    std::cerr << "    <fps>      : Frequency (fps) to send frame" << std::endl;
    std::cerr << "    <frameNum>      : Number of frame to be received" << std::endl;
    exit(0);
  }
  
  char*  hostname = argv[1];
  int    port     = atoi(argv[2]);
  double fps      = atof(argv[3]);
  int frameNum      = atoi(argv[4]);
  int    interval = (int) (1000.0 / fps);
  
  ISVCDecoder* decoder_;
  WelsCreateDecoder (&decoder_);
  SDecodingParam decParam;
  memset (&decParam, 0, sizeof (SDecodingParam));
  decParam.uiTargetDqLayer = UCHAR_MAX;
  decParam.eEcActiveIdc = ERROR_CON_SLICE_COPY;
  decParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_DEFAULT;
  decoder_->Initialize (&decParam);

  //------------------------------------------------------------
  // Establish Connection
  
  igtl::ClientSocket::Pointer socket;
  socket = igtl::ClientSocket::New();
  int r = socket->ConnectToServer(hostname, port);
  
  if (r != 0)
  {
    std::cerr << "Cannot connect to the server." << std::endl;
    exit(0);
  }
  
  //------------------------------------------------------------
  // Ask the server to start pushing tracking data
  std::cerr << "Sending STT_VIDEO message....." << std::endl;
  igtl::StartVideoDataMessage::Pointer startVideoMsg;
  startVideoMsg = igtl::StartVideoDataMessage::New();
  startVideoMsg->SetDeviceName("Video Client");
  startVideoMsg->SetTimeInterval(interval);
  startVideoMsg->SetUseCompress(interval);
  startVideoMsg->Pack();
  socket->Send(startVideoMsg->GetPackPointer(), startVideoMsg->GetPackSize());
  int loop = 0;
  std::string outputFileName = "outputDecodedVideo.yuv";
  while (1 && loop < frameNum)
  {
    //------------------------------------------------------------
    // Wait for a reply
    igtl::MessageHeader::Pointer headerMsg;
    headerMsg = igtl::MessageHeader::New();
    headerMsg->InitPack();
    int rs = socket->Receive(headerMsg->GetPackPointer(), headerMsg->GetPackSize());
    if (rs == 0)
    {
      std::cerr << "Connection closed." << std::endl;
      socket->CloseSocket();
      exit(0);
    }
    if (rs != headerMsg->GetPackSize())
    {
      std::cerr << "Message size information and actual data size don't match." << std::endl;
      socket->CloseSocket();
      exit(0);
    }
    
    headerMsg->Unpack();
    if (strcmp(headerMsg->GetDeviceName(), "Video") == 0)
    {
      ReceiveVideoData(socket, headerMsg, decoder_, outputFileName.c_str());
      if (++loop >= frameNum) // if received user define frame number
      {
        //------------------------------------------------------------
        // Ask the server to stop pushing tracking data
        std::cerr << "Sending STP_VIDEO message....." << std::endl;
        igtl::StopVideoMessage::Pointer stopVideoMsg;
        stopVideoMsg = igtl::StopVideoMessage::New();
        stopVideoMsg->SetDeviceName("TDataClient");
        stopVideoMsg->Pack();
        socket->Send(stopVideoMsg->GetPackPointer(), stopVideoMsg->GetPackSize());
        break;
      }
    }
    else
    {
      std::cerr << "Receiving : " << headerMsg->GetDeviceType() << std::endl;
      socket->Skip(headerMsg->GetBodySizeToRead(), 0);
    }
  }
  WelsDestroyDecoder(decoder_);
}


int ReceiveVideoData(igtl::ClientSocket::Pointer& socket, igtl::MessageHeader::Pointer& header, ISVCDecoder* decoder_, const char* outputFileName)
{
  std::cerr << "Receiving Video data type." << std::endl;
  
  //------------------------------------------------------------
  // Allocate Video Message Class
  
  igtl::VideoMessage::Pointer videoMsg;
  videoMsg = igtl::VideoMessage::New();
  videoMsg->SetMessageHeader(header);
  videoMsg->AllocatePack(header->GetBodySizeToRead());
  
  // Receive body from the socket
  socket->Receive(videoMsg->GetPackBodyPointer(), videoMsg->GetPackBodySize());
  
  // Deserialize the transform data
  // If you want to skip CRC check, call Unpack() without argument.
  videoMsg->Unpack();
  
  if (igtl::MessageHeader::UNPACK_BODY)
  {
    unsigned char* data[3];
    memset (data, 0, sizeof (data));
    int32_t iWidth = videoMsg->GetWidth(), iHeight = videoMsg->GetHeight(), streamLength = videoMsg->GetPackBodySize()- IGTL_VIDEO_HEADER_SIZE;
    H264DecodeInstance(decoder_, videoMsg->GetPackFragmentPointer(2), outputFileName, iWidth, iHeight, streamLength, NULL);
    return 1;
  }
  return 0;
}
