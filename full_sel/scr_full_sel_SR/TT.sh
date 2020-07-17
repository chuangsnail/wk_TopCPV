#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src && eval `scramv1 runtime -sh`

full_sel_sr /wk_cms2/cychuang/pre_sel_file_2016legacy/TTbar/pre_sel_bpk_ntuple_ 16_full_SR/TTbar/ 0
#full_sel_sr /wk_cms2/cychuang/pre_sel_file_2016legacy/TTbar/pre_sel_bpk_ntuple_1 test_/TTbar/ 0 test

