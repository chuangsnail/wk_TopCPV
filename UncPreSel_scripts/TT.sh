#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

UncPreSel /wk_cms2/cychuang/dbl_pre_16/TTbar/pre_sel_bpk_ntuple_ TTbar/ 0

