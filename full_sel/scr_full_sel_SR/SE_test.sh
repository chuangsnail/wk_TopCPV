#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src && eval `scramv1 runtime -sh`

full_sel_sr /wk_cms2/cychuang/dbl_pre_sel_file_2016legacy/SingleElectron/Run2016B/pre_sel_bpk_ntuple_ 16_full_SR/SingleElectron/Run2016B_test/ 1

