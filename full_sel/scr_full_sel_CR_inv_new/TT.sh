#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

full_sel_cr_inv /wk_cms2/cychuang/dbl_pre_16/TTbar/pre_sel_bpk_ntuple_ 16_full_CR_inv_new/TTbar/ 0

