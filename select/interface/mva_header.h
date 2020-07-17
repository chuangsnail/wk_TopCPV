#ifndef MVA_HEADER_H
#define MVA_HEADER_H

#include <string>
#include "string.h"

//#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/p05_0-2/TMVAClassification_MLP.class.C"
//#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/p05_all/TMVAClassification_MLP.class.C"

//#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/a05_all/TMVAClassification_MLP.class.C"
//#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/a05_all/TMVAClassification_BDTG.class.C"
//#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/a05_all/TMVAClassification_BDT.class.C"

//#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/a04_all/TMVAClassification_MLP.class.C"

/*
#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/p05_all/TMVAClassification_MLP.class.C"
#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/p04_all/TMVAClassification_MLP.class.C"


#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/a04_all/TMVAClassification_BDT.class.C"
#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/a04_all/TMVAClassification_BDTG.class.C"
*/

//string default_training_name = "a05_MLP";

#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/t13/TMVAClassification_MLP.class.C"
string default_training_name = "t13_MLP";
/*
#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/t13/TMVAClassification_BDTG.class.C"
string default_training_name = "t13_BDTG";
#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/t13/TMVAClassification_BDT.class.C"
string default_training_name = "t13_BDT";
*/


//----- NOTE -----//
//	in addition to modify this file
//	
//	remember to modify the MLPReader/BDTReader/BDTGReader
//	in TopCPViolation/select/src/Sel_sep.cc
//
//	and also modify the variables reading setting
//	in TopCPViolation/select/src/MVA_var.cc

#endif //MVA_HEADER_H
