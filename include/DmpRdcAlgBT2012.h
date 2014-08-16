/*
 *  $Id: DmpRdcAlgBT2012.h, 2014-08-15 23:57:33 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 27/05/2014
*/

#ifndef DmpRdcAlgBT2012_H
#define DmpRdcAlgBT2012_H

#include <boost/filesystem.hpp>     // path
#include <fstream>

#include "DmpFeeNavig.h"
#include "DmpVAlg.h"

class DmpEvtRawBgo;
class DmpEvtHeader;

//-------------------------------------------------------------------
struct _HeaderNavig{
  _HeaderNavig():DataLength(0xffff){
    PacketID[0] = 0x00;
    PacketID[1] = 0xff;
    for(size_t i=0;i<8;++i){
      Time[i] = 0x00;
    }
  }
  _HeaderNavig(const _HeaderNavig &r){
    PacketID[0] = r.PacketID[0];
    PacketID[1] = r.PacketID[1];
    for(size_t i=0;i<8;++i){
      Time[i] = r.Time[i];
    }
  }
  char      PacketID[2];    // 2 bytes, just use packageID[1], since it's the same as Fees' trigger
  unsigned short     DataLength;
  char      Time[8];
};

//-------------------------------------------------------------------
struct _FeeData{
  _FeeData(){}
  _FeeData(const _FeeData &r){
    Navigator = r.Navigator;
    short n=r.Signal.size();
    for(short i=0;i<n;++i){
      Signal.push_back(r.Signal[i]);
    }
  }
  _FeeData(char *data,const short &bytes,const short &feeID,const short &runMode,const short &trigger,const unsigned char &pkgFlag=0xff,const short &trgFlag=1,const bool &crc=true):Navigator(feeID,runMode,trigger,pkgFlag,trgFlag,crc){
    for(short i=0;i<bytes;++i){
      Signal.push_back(data[i]);
    }
  }
  ~_FeeData(){}
  DmpFeeNavig  Navigator;
  std::vector<char> Signal;
};

//-------------------------------------------------------------------
class DmpRdcAlgBT2012 : public DmpVAlg{
/*
 *  DmpRdcAlgBT2012
 *
 */
public:
  DmpRdcAlgBT2012();
  ~DmpRdcAlgBT2012();

  void Set(const std::string&,const std::string&);
  bool Initialize();
  bool ProcessThisEvent();
  bool Finalize();

public:
  std::string GetInputFileName()const{return fInDataName.filename().string();}
  std::string GetInputPath()const{return fInDataName.stem().string();}

private:    // for all
  enum EFeeNumber{
    kFeeNoPsd = 1,
    kFeeNoBgo = 6,
    kFeeNoNud = 1,
    KTotalFeeNo = kFeeNoPsd+kFeeNoBgo+kFeeNoNud
  };
  enum EFeeType{
   kFeeTypeBgo = 0x10,
   kFeeTypePsd = 0x20,
   kFeeTypeNud = 0x30,
  };

  bool ReadDataIntoDataBuffer();
  bool fOneEventReady;
  /* 
   *    1. return true:
   *        1.1     read one 0xe2250813 into data buffer
   *        1.2     throw one 0xe2250813 into Exception()
   *    2. retrun false:
   *        reach the end of input file stream
   *
   */
  bool CheckE2250813DataLength(const int &nBytes);
  bool CheckEb90DataLength(const int &nBytes);

  void Exception(const int &b);     // throw whole data of e2250813 into fOutError
  boost::filesystem::path   fInDataName;    // input data name
  std::ifstream     fFile;          // in data stream
  std::ofstream     fOutError;      // save error datas into Error_fInDataName
  long              fMaxPkgNo;      // read how many 0xe2250813?
  long              fCurrentPkgID;  // current package id
  std::vector<_HeaderNavig> fHeaderBuf;
  std::vector<std::vector<_FeeData> >    fBgoBuf;
  //std::vector<_FeeData*>    fPsdBuf;
  //std::vector<_FeeData*>    fNudBuf;

private:
  DmpEvtHeader      *fEvtHeader;    // save me
  char              *fTmpTime;      //
  bool ProcessThisEventHeader();    // convert event header
  void PrintTime()const;

private:    // Bgo
  std::string       fCNCTPathBgo;   // connector path
  std::map<short,short> fMapBgo;        // map of Bgo connector

  DmpEvtRawBgo      *fEvtBgo;       // Bgo outdata
  bool InitializeBgo();
  bool ProcessThisEventBgo();

private:    // Psd

private:    // Nud

};

#endif


