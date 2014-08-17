/*
 *  $Id: ReadDataIntoDataBuffer.cc, 2014-08-17 21:10:58 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 08/08/2014
*/

#include "DmpRdcAlgBT2012.h"
#include "DmpEDetectorID.h"
#include "DmpCore.h"

bool DmpRdcAlgBT2012::ReadDataIntoDataBuffer(){
  if(fFile.eof()){
    DmpLogInfo<<"Reach the end of "<<fInDataName<<DmpLogEndl;
    return false;
  }
  unsigned int scientificHeader = 0;         // 4 bytes 0xe225 0813
  fFile.read((char*)(&scientificHeader),4);
  while(0xe2250813 != htobe32(scientificHeader)){
    fFile.seekg((int)fFile.tellg()-3,std::ios::beg);
    fFile.read((char*)(&scientificHeader),4);
  }
  ++fCurrentPkgID;
  if(fCurrentPkgID == fMaxPkgNo){
    gCore->TerminateRun();
  }
  int endOfLastHeader = fFile.tellg();
  //-------------------------------------------------------------------
  _HeaderNavig newEvt;
  fFile.read(newEvt.PacketID,2);
  fFile.read((char*)(&newEvt.DataLength),2);
  fFile.read(newEvt.Time,8);
  DmpLogDebug<<std::dec<<"\tpackageID = "<<(short)(unsigned char)newEvt.PacketID[1]<<"\t length = "<<htobe16(newEvt.DataLength)<<DmpLogEndl;
  if(CheckE2250813DataLength(htobe16(newEvt.DataLength))){    // will find next 0xe2250813 as expected
    fHeaderBuf.push_back(newEvt);
    std::vector<_FeeData>  aBgoEvt;
    fBgoBuf.push_back(aBgoEvt);
    for(short i=0;i<fTotalFeeNo;++i){
      unsigned short feeHeader = 0;
      fFile.read((char*)(&feeHeader),2);
      if(0xeb90 == htobe16(feeHeader)){
//-------------------------------------------------------------------
        char feeTrigger;
        fFile.read(&feeTrigger,1);
        char runMode_feeID;
        fFile.read(&runMode_feeID,1);
        short feeID = runMode_feeID&0x3f;   // &0011 1111
        short runMode = runMode_feeID>>6;
        short detectorID = (runMode_feeID>>4)&0x03;
        unsigned short dataLength;        // 2 bytes
        fFile.read((char*)(&dataLength),2);
        dataLength= htobe16(dataLength);
        if(CheckEb90DataLength(dataLength)){
          char data[dataLength-2];  // NOTE: 2 bytes of data length itself
          fFile.read(data,dataLength-2);
          _FeeData newFee(data,dataLength-2,feeID,runMode,(short)(unsigned char)feeTrigger);
          if(DmpEDetectorID::kBgo == detectorID){
            DmpLogInfo<<" Fee type [Bgo] "<<std::hex<<feeID<<" mode "<<runMode<<std::dec<<DmpLogEndl;
            fBgoBuf[fBgoBuf.size()-1].push_back(newFee);
          }else if(DmpEDetectorID::kPsd == detectorID){
            DmpLogInfo<<" Fee type [Psd] "<<std::hex<<feeID<<" mode "<<runMode<<std::dec<<DmpLogEndl;
            //fPsdBuf[fPsdBuf.size()-1].push_back(newFee);
          }else if(DmpEDetectorID::kNud == detectorID){
            DmpLogInfo<<" Fee type [Nud] "<<std::hex<<feeID<<" mode "<<runMode<<std::dec<<DmpLogEndl;
            //fNudBuf[fNudBuf.size()-1].push_back(newFee);
          }else if(DmpEDetectorID::kStk == detectorID){
            DmpLogInfo<<" Fee type [Stk] "<<std::hex<<feeID<<" mode "<<runMode<<std::dec<<DmpLogEndl;
          }else{
            DmpLogWarning<<" Fee type error.. "<<(short)(runMode_feeID&0x30)<<DmpLogEndl;
          }
        }else{
          DmpLogWarning<<"data length error [0xeb90].\ti = "<<i<<" Fee 0x"<<(short)(unsigned char)runMode_feeID;
          PrintTime();
          Exception(endOfLastHeader);
          fHeaderBuf.erase(fHeaderBuf.end());
          fBgoBuf.erase(fBgoBuf.end());
          return true;
        }
//-------------------------------------------------------------------
      }else{
        DmpLogWarning<<"not find 0xeb90 \t";
        PrintTime();
        Exception(endOfLastHeader);
        fHeaderBuf.erase(fHeaderBuf.end());
        fBgoBuf.erase(fBgoBuf.end());
        return true;
      }
    }
// *
// *  TODO: if add stk, will not fOneEventReady at here
// *
    fOneEventReady = true; // this event done
  }else{
    DmpLogWarning<<"data length error [0xe2250813].\t";
    std::cout<<"  Time:";
    for(size_t i=2;i<8;++i){
      std::cout<<std::hex<<"  "<<(short)(unsigned char)newEvt.Time[i];
    }
    std::cout<<std::dec<<std::endl;
    Exception(endOfLastHeader);
  }
  return true;
}

//-------------------------------------------------------------------
bool DmpRdcAlgBT2012::CheckE2250813DataLength(const int &n){
  int skipPoint = fFile.tellg();
  fFile.seekg(skipPoint+n+1-8,std::ios::beg);   // time: 8 bytes. need 1
  unsigned int scientificHeader = 0;            // 4 bytes 0xe225 0813
  fFile.read((char*)(&scientificHeader),4);
  if(0xe2250813 != htobe32(scientificHeader)){
    return false;
  }
  fFile.seekg(skipPoint,std::ios::beg);
  return true;
}

//-------------------------------------------------------------------
bool DmpRdcAlgBT2012::CheckEb90DataLength(const int &n){
  int skipPoint = fFile.tellg();
  fFile.seekg(skipPoint+n-2,std::ios::beg);   // NOTE:  n include 2 bytes which is data length
  unsigned short header = 0;         // 4 bytes 0xe225 0813
  fFile.read((char*)(&header),2);
  if(0xeb90 != htobe16(header) && 0xe225 != htobe16(header)){
  /*
   *    the last Fee will read 0xe225
   */
    return false;
  }
  fFile.seekg(skipPoint,std::ios::beg);
  return true;
}

//-------------------------------------------------------------------
void DmpRdcAlgBT2012::Exception(const int &endOfLastE2250813){
  fFile.seekg(endOfLastE2250813,std::ios::beg);
  unsigned int scientificHeader = 0;         // 4 bytes 0xe225 0813
  fFile.read((char*)(&scientificHeader),4);
  while(0xe2250813 != htobe32(scientificHeader)){
    fFile.seekg((int)fFile.tellg()-3,std::ios::beg);
    fFile.read((char*)(&scientificHeader),4);
  }
  int nBytes = (int)fFile.tellg() - endOfLastE2250813;
  char *errorData = new char[nBytes];
  fFile.seekg(endOfLastE2250813-4,std::ios::beg);
  fFile.read(errorData,nBytes);
  fOutError.write(errorData,nBytes);
  delete[] errorData;
}

