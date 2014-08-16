/*
 *  $Id: DmpEventRawLinkDef.h, 2014-08-07 16:30:35 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 07/08/2014
*/

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedtypedef;
#pragma link C++ class DmpEvtHeader+;
#pragma link C++ class DmpFeeNavig+;
#pragma link C++ typedef V_DmpFeeNavig;

#pragma link C++ typedef M_RawSignalBgoPMT;
#pragma link C++ typedef M_RawSignalBgoBar;
#pragma link C++ typedef M_RawSignalBgoLayer;
#pragma link C++ typedef M_RawSignalBgo;
#pragma link C++ class DmpEvtRawBgo+;

/*
#pragma link C++ typedef M_RawSignalPsdPMT;
#pragma link C++ typedef M_RawSignalPsdStrip;
#pragma link C++ typedef M_RawSignalPsdLayer;
#pragma link C++ typedef M_RawSignalPsd;
#pragma link C++ class DmpEvtRawPsd+;

#pragma link C++ class DmpEvtRawNud+;
*/

#endif

