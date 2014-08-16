/*
 *  $Id: DmpEvtRawBgo.cc, 2014-08-07 16:14:09 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 24/04/2014
*/

#include "DmpEvtRawBgo.h"
#include "DmpEFeeFlags.h"

//-------------------------------------------------------------------
DmpEvtRawBgo::DmpEvtRawBgo()
 :fTrigger(-1),fRunMode(-1),fIsGood(true)
{
}

//-------------------------------------------------------------------
DmpEvtRawBgo::~DmpEvtRawBgo(){
}

//-------------------------------------------------------------------
void DmpEvtRawBgo::Reset(){
// *
// *  TODO: check... will release heap of all sub-map??
// *
  fSignal.clear();
  fFeeNavig.clear();
  fIsGood = true;
}

//-------------------------------------------------------------------
void DmpEvtRawBgo::AppendSignal(const short &l,const short &b,const short &s,const short &d,const short &v){
// *
// *  TODO: check this is the only l,b,s,d
// *
  fSignal[l][b][s][d] = v;
}

//-------------------------------------------------------------------
M_RawSignalBgoLayer DmpEvtRawBgo::GetSignal(const short &l)const{
  M_RawSignalBgoLayer tmp;
  if(ExistLayer(l)){
    tmp = fSignal.at(l);
  }
  return tmp;
}

//-------------------------------------------------------------------
M_RawSignalBgoBar DmpEvtRawBgo::GetSignal(const short &l,const short &b)const{
  M_RawSignalBgoBar tmp;
  if(ExistBar(l,b)){
    tmp = fSignal.at(l).at(b);
  }
  return tmp;
}

//-------------------------------------------------------------------
M_RawSignalBgoPMT DmpEvtRawBgo::GetSignal(const short &l,const short &b,const short &s)const{
  M_RawSignalBgoPMT tmp;
  if(ExistPMT(l,b,s)){
    tmp = fSignal.at(l).at(b).at(s);
  }
  return tmp;
}

//-------------------------------------------------------------------
short DmpEvtRawBgo::GetSignal(const short &l,const short &b,const short &s,const short &d)const{
  if(ExistPMT(l,b,s)){
    if(fSignal.at(l).at(b).at(s).find(d) != fSignal.at(l).at(b).at(s).end()){
      return fSignal.at(l).at(b).at(s).at(d);
    }
  }
  return -1;
}

//-------------------------------------------------------------------
bool DmpEvtRawBgo::ExistLayer(const short &l)const{
  if(fSignal.find(l) == fSignal.end()){
    return false;
  }
  return true;
}

//-------------------------------------------------------------------
bool DmpEvtRawBgo::ExistBar(const short &l,const short &b)const{
  if(not ExistLayer(l)){
    return false;
  }
  if(fSignal.at(l).find(b) == fSignal.at(l).end()){
    return false;
  }
  return true;
}

//-------------------------------------------------------------------
bool DmpEvtRawBgo::ExistPMT(const short &l,const short &b,const short &s)const{
  if(not ExistBar(l,b)){
    return false;
  }
  if(fSignal.at(l).at(b).find(s) == fSignal.at(l).at(b).end()){
    return false;
  }
  return true;
}

//-------------------------------------------------------------------
void DmpEvtRawBgo::GenerateStatus(){
  for(size_t i=0;i<fFeeNavig.size();++i){
    if(false ==  fFeeNavig[i].CRCFlag){
      fIsGood = false;
      break;
    }
    if(DmpETriggerFlag::kCheckWrong == fFeeNavig[i].TriggerFlag){
      fIsGood = false;
      break;
    }
    if(DmpEPackageFlag::kGood != fFeeNavig[i].PackageFlag){
      fIsGood = false;
      break;
    }
  }
//-------------------------------------------------------------------
  fTrigger = fFeeNavig[0].Trigger;
  for(short i=1;i<fFeeNavig.size();++i){
    if(fTrigger != fFeeNavig[i].Trigger){
      fTrigger = -1;
      fIsGood = false;
    }
  }
//-------------------------------------------------------------------
  short lastTrigger = fTrigger;
  if(-1 != lastTrigger && -1 != fTrigger){
    short t = lastTrigger-fTrigger;
    if(t != -1 || t != 0xff){ // TODO: check ??
      //fIsGood = false;        // trigger skip, but event not wrong
// *
// *  TODO: cout warning
// *
      //DmpLogError<<" trigger not continuous:\t"<<t<<DmpLogEndl;
    }
  }
//-------------------------------------------------------------------
  fRunMode = fFeeNavig[0].RunMode;
  for(size_t i=1;i<fFeeNavig.size();++i){
    if(fRunMode != fFeeNavig[i].RunMode){
      fRunMode = -1;
      //fIsGood = false;  // yes! Some good event run mode not match, too
    }
  }
}

//-------------------------------------------------------------------

