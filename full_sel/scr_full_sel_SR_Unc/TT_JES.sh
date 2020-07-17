#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

full_sel_sr_unc /wk_cms2/cychuang/UncPreSel_NS/TTbar/pre_sel_bpk_ntuple_ TTbar/ 0 JES_up/
full_sel_sr_unc /wk_cms2/cychuang/UncPreSel_NS/TTbar/pre_sel_bpk_ntuple_ TTbar/ 0 JES_down/

