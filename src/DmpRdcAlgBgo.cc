/*
 *  $Id: DmpRdcAlgBgo.cc, 2014-08-12 00:12:36 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 09/03/2014
 *    Yifeng WEI (weiyf@mail.ustc.edu.cn) 24/04/2014
*/

#include "DmpEvtRawBgo.h"
#include "DmpDataBuffer.h"
#include "DmpRdcAlgBT2012.h"

//-------------------------------------------------------------------
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
bool DmpRdcAlgBT2012::InitializeBgo(){
  if(fCNCTPathBgo == "NO"){
    DmpLogInfo<<"No set connector:\tBgo"<<DmpLogEndl;
    return true;
  }else{
    DmpLogInfo<<"Setting connector:\tBgo"<<DmpLogEndl;
  }
  // setup connector
  short feeID=0, channelNo=0, channelID=0, layerID=0, barID=0, sideID=0, dyID=0;
  boost::filesystem::directory_iterator end_iter;
  for(boost::filesystem::directory_iterator iter(fCNCTPathBgo);iter!=end_iter;++iter){
    if(iter->path().extension() != ".cnct") continue;
    ifstream cnctFile(iter->path().string().c_str());
    if (not cnctFile.good()){
      DmpLogError<<"\t\treading cnct file ("<<iter->path().string()<<") failed"<<DmpLogEndl;
      cnctFile.close();
      return false;
    }else{
      std::cout<<"\t\treading cnct file: "<<iter->path().string();
    }
    cnctFile>>feeID;
    cnctFile>>channelNo;
    for(short s=0;s<channelNo;++s){
      cnctFile>>channelID;
      cnctFile>>layerID;
      cnctFile>>barID;
      cnctFile>>sideID;
      cnctFile>>dyID;
      fMapBgo.insert(std::make_pair(feeID*1000+channelID,layerID*10000+barID*100+sideID*10+dyID));
    }
    cnctFile.close();
    std::cout<<" Done. ID = "<<feeID<<"\tN_channel = "<<channelNo<<std::endl;
  }
  //-------------------------------------------------------------------
  fEvtBgo = new DmpEvtRawBgo();
  if(not gDataBuffer->RegisterObject("Event/Rdc/Bgo",fEvtBgo,"DmpEvtRawBgo")){
    return false;
  }
  return true;
}

//-------------------------------------------------------------------
bool DmpRdcAlgBT2012::ProcessThisEventBgo(){
        /*
  static char data=0;
  static unsigned char data2=0;
  std::map<short,short> triggers,runModes;
  fBgoBarSet->Delete();
  DmpLogDebug<<"[Bgo] from "<<fFile.tellg();
//-------------------------------------------------------------------
  for (short feeCounts=0;feeCounts<fFEENoBgo;++feeCounts) {
    fFile.read((char*)(&data2),1);
    if (data2!=0xeb) {
      fEvtHeader->AddFeeErrorTag(DmpDetectorID::kBgo,feeCounts+1,DmpDataError::NotFind_0xeb);
      return false;
    }
    fFile.read((char*)(&data2),1);
    if (data2!=0x90) {
      fEvtHeader->AddFeeErrorTag(DmpDetectorID::kBgo,feeCounts+1,DmpDataError::NotFind_0x90);
      return false;
    }
    fFile.read((char*)(&data2),1);       // trigger
    short trigger = data2;
    fFile.read((char*)(&data2),1);       // run mode and FEE ID
    short feeID = data2%16;
    triggers.insert(std::make_pair(feeID,trigger));
    runModes.insert(std::make_pair(feeID,data2/16-fFEETypeBgo));
    fFile.read((char*)(&data),1);       // data length, 2 bytes
    fFile.read((char*)(&data2),1);
    short nBytes = data*256+data2-2-2-2;        // 2 bytes for data length, 2 bytes for 0x0000, 2 bytes for CRC
// *
// *  TODO: mode == k0Compress && data length == xxx
// *
    short nSignal=0;
    if(runModes[feeID] == DmpRunMode::kCompress){
      nSignal = nBytes/3;
      DmpLogDebug<<"\tFEE ID "<<feeID<<" signalNo = "<<nSignal<<DmpLogEndl;
      short channelID;
      for(short i=0;i<nSignal;++i){     // kCompress
        fFile.read((char*)(&channelID),1);
        fFile.read((char*)(&data),1);
        fFile.read((char*)(&data2),1);
        AppendSignalBgo(feeID*1000+channelID,(data*256+data2)&0x3fff);
      }
    }else{
      nSignal = nBytes/2;
      DmpLogDebug<<"\tFEE ID "<<feeID<<" signalNo = "<<nSignal<<DmpLogEndl;
      for(short i=0;i<nSignal;++i){     // k0Compress
        fFile.read((char*)(&data),1);
        fFile.read((char*)(&data2),1);
        AppendSignalBgo(feeID*1000+i,(data*256+data2)&0x3fff);
      }
    }
    fFile.read((char*)(&data2),1);       // 2 bytes for 0x0000
    fFile.read((char*)(&data2),1);       // must split them, 2 bytes for 0x0000
    fFile.read((char*)(&data2),1);       // 2 bytes for CRC
    fFile.read((char*)(&data2),1);       // must spplit them, 2 bytes for CRC
  }
  fEvtHeader->SetTrigger(DmpDetectorID::kBgo,triggers);
  fEvtHeader->SetRunMode(DmpDetectorID::kBgo,runModes);
//-------------------------------------------------------------------
*/
  fEvtBgo->Reset();
  short nFee = fBgoBuf[0].size();
  for(size_t i=0;i<nFee;;++i){
    fEvtBgo->SetFeeNavigator(fBgoBuf[0][i].Navigator);
    short nChannel = 0;
    if(DmpERunMode::kCompress == fBgoBuf[0][i].Navigator.RunMode){
      nChannel = fBgoBuf[0][i].Signal.size();
    }else if(DmpERunMode::k0Compress == fBgoBuf[0][i].Navigator.RunMode || DmpERunMode::kCalDAC == fBgoBuf[0][i].Navigator.RunMode){
    }
  }
  //fEvtBgo->GenerateStatus();
  fBgoBuf.erase(fBgoBuf.begin());
  return true;
}


