/*
 *  $Id: DmpEFeeFlags.h, 2014-08-10 22:10:53 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 10/08/2014
*/

#ifndef DmpEFeeFlags_H
#define DmpEFeeFlags_H

//-------------------------------------------------------------------
namespace DmpEPackageFlag{
  enum Type{
    kPowerOk = 0x80,        // bit 7 = 1, power not resetted. VA locked then power reset
    kHThresholdOk = 0x40,   // bit 6 = 1, high threshold ok
    kLThresholdOk = 0x20,   // bit 5 = 1, low threshold ok
    kTAOk = 0x10,           // bit 4 = 1, TA ok
    kADC976Ok = 0x08,       // bit 3 = 1, ADC 976 ok
    kGood = 0xff            // bit[0~2] = 111. reserved
  };
}

//-------------------------------------------------------------------
namespace DmpERunMode{
  enum Type{
    k0Compress = 0,
    kCompress = 1,
    kCalDAC = 2,
    kUnknow = 3
  };
}

//-------------------------------------------------------------------
namespace DmpETriggerFlag{
  enum Type{
    kNoReceiveTriggerCheck = 0,       // did not receive check signal
    kCheckRight = 1,    // received check signal, and checked right
    kCheckWrong = 2,    // received check signal, and checked, but wrong
    kUnknow = 3         // unknow
  };
}

#endif

