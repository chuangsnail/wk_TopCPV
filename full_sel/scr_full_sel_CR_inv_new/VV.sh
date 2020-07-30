#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

full_sel_cr_inv /wk_cms2/cychuang/dbl_pre_16/VV/WW_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_ 16_full_CR_inv_new/VV/WW_TuneCUETP8M1_13TeV-pythia8/ 0
full_sel_cr_inv /wk_cms2/cychuang/dbl_pre_16/VV/WZ_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_ 16_full_CR_inv_new/VV/WZ_TuneCUETP8M1_13TeV-pythia8/ 0
full_sel_cr_inv /wk_cms2/cychuang/dbl_pre_16/VV/ZZ_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_ 16_full_CR_inv_new/VV/ZZ_TuneCUETP8M1_13TeV-pythia8/ 0

