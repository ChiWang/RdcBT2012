/*
 *  $Id: DmpRdcAlgBT2012.cc, 2014-08-13 10:57:04 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 27/05/2014
*/

#include <stdlib.h>     // getenv()
#include "DmpEvtHeader.h"
#include "DmpDataBuffer.h"
#include "DmpRdcAlgBT2012.h"

//-------------------------------------------------------------------
DmpRdcAlgBT2012::DmpRdcAlgBT2012()
 :DmpVAlg("Rdc/BT2012"),fInDataName("NO"),fMaxPkgNo(-1),fCurrentPkgID(0),fEvtHeader(0),
  fFeeNoBgo(6),fCNCTPathBgo("NO"),fEvtBgo(0),
  fFeeNoPsd(1),fCNCTPathPsd("NO"),
  fFeeNoNud(1),fCNCTPathNud("NO"),
  fFeeNoStk(1),fCNCTPathStk("NO")
{
  OptMap.insert(std::make_pair("BinaryFile",    0));
  OptMap.insert(std::make_pair("Bgo/Connector", 1));
  OptMap.insert(std::make_pair("Psd/Connector", 4));
  OptMap.insert(std::make_pair("Stk/Connector", 5));
  OptMap.insert(std::make_pair("Nud/Connector", 7));
  OptMap.insert(std::make_pair("PackageNumber", 8));
}

//-------------------------------------------------------------------
DmpRdcAlgBT2012::~DmpRdcAlgBT2012(){
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
void DmpRdcAlgBT2012::Set(const std::string &type, const std::string &argv){
  if(OptMap.find(type) == OptMap.end()){
    DmpLogError<<"No argument type: "<<type<<DmpLogEndl;
  }
// *
// *  TODO: if release, use DMPSWSYS
// *
  std::string prefix = (std::string)getenv("DMPSWWORK")+"/share/Connector/";
  //std::string prefix = (std::string)getenv("DMPSWSYS")+"/share/Connector/";
  switch (OptMap[type]){
    case 0: // BinaryFile
    {
      fInDataName = argv;
      break;
    }
    case 1: // Connector/Bgo
    {
      fCNCTPathBgo = prefix+argv;
      break;
    }
    case 4:
    {// Psd/Connector
      fCNCTPathPsd = prefix+argv;
      break;
    }
    case 5:
    {// Stk/Connector
      fCNCTPathStk = prefix+argv;
      break;
    }
    case 7:
    {// Nud/Connector
      fCNCTPathNud = prefix+argv;
      break;
    }
    case 8: // PackageNumber
    {
      fMaxPkgNo = boost::lexical_cast<long>(argv);
      break;
    }
  }
}

//-------------------------------------------------------------------
bool DmpRdcAlgBT2012::Initialize(){
  fFile.open(fInDataName.c_str(),std::ios::in|std::ios::binary);
  if(not fFile.good()){
    DmpLogError<<"Open "<<fInDataName<<" failed"<<DmpLogEndl;
    return false;
  }else{
    std::string name = "Error_"+fInDataName.filename().string();
    DmpLogInfo<<"Reading "<<fInDataName.string()<<"\tError data in "<<name<<DmpLogEndl;
    fOutError.open(name.c_str(),std::ios::out|std::ios::binary);
  }
  fEvtHeader = new DmpEvtHeader();
  if(not gDataBuffer->RegisterObject("Event/Rdc/EventHeader",fEvtHeader,"DmpEvtHeader")){
    return false;
  }
  bool bgo = InitializeBgo();
  return bgo;
}

//-------------------------------------------------------------------
#include "DmpCore.h"
bool DmpRdcAlgBT2012::ProcessThisEvent(){
  fOneEventReady = false;
  // TODO: inster Stk buffer at here
  while(not fOneEventReady){
    if(not ReadDataIntoDataBuffer()){
      gCore->TerminateRun();
      return false;
    }
  }
  bool header = ProcessThisEventHeader();
  bool bgo = ProcessThisEventBgo();
std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<"), in "<<__PRETTY_FUNCTION__<<std::endl;
  return (header && bgo);
  //return true;
}

//-------------------------------------------------------------------
bool DmpRdcAlgBT2012::Finalize(){
  fFile.close();
  fOutError.close();
  return true;
}

//-------------------------------------------------------------------
bool DmpRdcAlgBT2012::ProcessThisEventHeader(){
  fEvtHeader->Reset();
  fEvtHeader->SetPackageID((short)(unsigned char)fHeaderBuf[0].PacketID[1]);
  fEvtHeader->SetTime(&fHeaderBuf[0].Time[2]);
  fHeaderBuf.erase(fHeaderBuf.begin());
  return true;
}

//-------------------------------------------------------------------
void DmpRdcAlgBT2012::PrintTime()const{
  std::cout<<"  Time:";
  for(size_t i=2;i<8;++i){
    std::cout<<std::hex<<"  "<<(short)(unsigned char)fHeaderBuf[fHeaderBuf.size()-1].Time[i];
  }
  std::cout<<std::dec<<std::endl;
}

