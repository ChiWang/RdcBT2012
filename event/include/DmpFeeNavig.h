/*
 *  $Id: DmpFeeNavig.h, 2014-08-10 20:57:03 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 07/08/2014
*/

#ifndef DmpFeeNavig_H
#define DmpFeeNavig_H

#include "TObject.h"

//-------------------------------------------------------------------
class DmpFeeNavig : public TObject{
/*
 *  navigator of Fee, no ADC counts in this class
 *
 */
public:
  DmpFeeNavig():FeeID(-1),RunMode(-1),Trigger(-1),PackageFlag(0xff),TriggerFlag(-1),CRCFlag(true){}
  DmpFeeNavig(const DmpFeeNavig &r){
    FeeID = r.FeeID;
    RunMode = r.RunMode;
    Trigger = r.Trigger;
    PackageFlag = r.PackageFlag;
    TriggerFlag = r.TriggerFlag;
    CRCFlag = r.CRCFlag;
  }
  DmpFeeNavig& operator=(const DmpFeeNavig &r){
    FeeID = r.FeeID;
    RunMode = r.RunMode;
    Trigger = r.Trigger;
    PackageFlag = r.PackageFlag;
    TriggerFlag = r.TriggerFlag;
    CRCFlag = r.CRCFlag;
  }
  DmpFeeNavig(const short &feeID,const short &runMode,const short &trigger,const unsigned char &pkgFlag,const short &trgFlag,const bool &crc):FeeID(feeID),RunMode(runMode),Trigger(trigger),PackageFlag(pkgFlag),TriggerFlag(trgFlag),CRCFlag(crc){}

  short FeeID;
  short RunMode;
  short Trigger;
  unsigned char PackageFlag;
  short TriggerFlag;
  bool CRCFlag;

  ClassDef(DmpFeeNavig,1)
};

typedef std::vector<DmpFeeNavig>    V_DmpFeeNavig;

#endif

