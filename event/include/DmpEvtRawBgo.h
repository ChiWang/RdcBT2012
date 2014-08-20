/*
 *  $Id: DmpEvtRawBgo.h, 2014-08-18 11:24:44 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 24/04/2014
*/

#ifndef DmpEvtRawBgo_H
#define DmpEvtRawBgo_H

#include <vector>
#include "DmpFeeNavig.h"

//-------------------------------------------------------------------
class DmpEvtRawBgo : public TObject{
/*
 *  DmpEvtRawBgo
 *
 *      this class is used to save output of Raw for Bgo
 *
 *   one object mean one Bgo event
 *
 */
public:
  DmpEvtRawBgo();
  ~DmpEvtRawBgo();
  void  Reset();
  void  SetFeeNavigator(const DmpFeeNavig &s){fFeeNavig.push_back(s);}
  void  AppendSignal(const short &l,const short &b,const short &s,const short &d,const short &v);
  void  GenerateStatus();

  bool  IsGoodEvent()const{return fIsGood;}
  short GetTrigger()const{return fTrigger;}
  short GetRunMode()const{return fRunMode;}
  V_DmpFeeNavig GetFeeNavigator()const{return fFeeNavig;}
  short GetSignal(const short &l,const short &b,const short &s,const short &d)const;

private:
  short ConstructGID(const short &l,const short &b,const short &s,const short &d)const;

private:
  short fTrigger;
  short fRunMode;
  bool  fIsGood;
  V_DmpFeeNavig  fFeeNavig;
  std::vector<short>    fGlobalID;
  /*
   *    short: bit 15~0
   *
   *    layer(0~13):    bits 14,13,12,11
   *        = (fGlobalID >> 11) & 0x000f
   *    bar(0~23):      bits 10,9,8,7,6
   *         = (fGlobalID >> 6) & 0x001f
   *    side(0,1):      bits 4
   *        = (fGlobal >> 4) & 0x0001
   *    dynode(2,5,8):  bits 3,2,1,0
   *        = fGlobal & 0x000f
   */
  std::vector<short>    fADC;

  ClassDef(DmpEvtRawBgo,1)
};

#endif


