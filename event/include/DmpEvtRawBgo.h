/*
 *  $Id: DmpEvtRawBgo.h, 2014-08-07 23:09:11 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 24/04/2014
*/

#ifndef DmpEvtRawBgo_H
#define DmpEvtRawBgo_H

#include <map>
#include "DmpFeeNavig.h"

//-------------------------------------------------------------------
typedef std::map<short,short>                 M_RawSignalBgoPMT;       // key is dynode id{2,5,8}, value is ADC count
typedef std::map<short,M_RawSignalBgoPMT>     M_RawSignalBgoBar;       // key is side(0,1),
typedef std::map<short,M_RawSignalBgoBar>     M_RawSignalBgoLayer;     // key is bar id
typedef std::map<short,M_RawSignalBgoLayer>   M_RawSignalBgo;          // key is layer id

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
  void  AppendSignal(const short &l,const short &b,const short &s,const short &d,const short &v);
  M_RawSignalBgo      GetSignal()const{return fSignal;}
  M_RawSignalBgoLayer GetSignal(const short &l)const;
  M_RawSignalBgoBar   GetSignal(const short &l,const short &b)const;
  M_RawSignalBgoPMT   GetSignal(const short &l,const short &b,const short &s)const;
  short               GetSignal(const short &l,const short &b,const short &s,const short &d)const;

public:
  void  SetFeeNavigator(const DmpFeeNavig &s){fFeeNavig.push_back(s);}
  V_DmpFeeNavig GetFeeNavigator()const{return fFeeNavig;}

public:
  void  GenerateStatus();
  bool  IsGoodEvent()const{return fIsGood;}
  short GetTrigger()const{return fTrigger;}
  short GetRunMode()const{return fRunMode;}

private:
  bool ExistLayer(const short &l)const;
  bool ExistBar(const short &l,const short &b)const;
  bool ExistPMT(const short &l,const short &b,const short &s)const;

private:
  short fTrigger;
  short fRunMode;
  bool  fIsGood;
  M_RawSignalBgo    fSignal;        // fSignal[layer][bar][side][dynode]
  V_DmpFeeNavig     fFeeNavig;

  ClassDef(DmpEvtRawBgo,1)
};

#endif


