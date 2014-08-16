/*
 *  $Id: DmpEvtHeader.h, 2014-08-08 21:59:44 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 13/12/2013
*/

#ifndef DmpEvtHeader_H
#define DmpEvtHeader_H

#include "TObject.h"

class DmpEvtHeader : public TObject{
/*
 * DmpEvtHeader
 *
*/
public:
  DmpEvtHeader(){Reset();}
  ~DmpEvtHeader(){}

  void  Reset(){fPackageID=-1;fSecond=0;fMillisecond=-1;}                // invoke it at the begin of Rdc::ProcessThisEvent
  void  SetPackageID(short v) {fPackageID = v;}     // trigger of satellite
  void  SetTime(char *time){
    for(size_t i=0;i<4;++i){        // 4 bytes second
      fSecond = fSecond*256 + (short)(unsigned char)time[i];
    }
    fMillisecond = (short)(unsigned char)time[4]*256 + (short)(unsigned char)time[5];          // 2 bytes millisecond
  }
  short GetPackageID()const {return fPackageID;}
  int   GetSecond()const {return fSecond;}
  short GetMillisecond()const {return fMillisecond;}

private:
  short     fPackageID;         // trigger from satellite
  int       fSecond;            // time second
  short     fMillisecond;       // time millisecond

  ClassDef(DmpEvtHeader,1)
};

#endif

