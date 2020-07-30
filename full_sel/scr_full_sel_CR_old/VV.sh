#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src && eval `scramv1 runtime -sh`

full_sel_cr /wk_cms2/cychuang/dbl_pre_sel_file_2016legacy/VV/WW_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_ 16_full_CR/VV/WW_TuneCUETP8M1_13TeV-pythia8/ 0
full_sel_cr /wk_cms2/cychuang/dbl_pre_sel_file_2016legacy/VV/WZ_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_ 16_full_CR/VV/WZ_TuneCUETP8M1_13TeV-pythia8/ 0
full_sel_cr /wk_cms2/cychuang/dbl_pre_sel_file_2016legacy/VV/ZZ_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_ 16_full_CR/VV/ZZ_TuneCUETP8M1_13TeV-pythia8/ 0

