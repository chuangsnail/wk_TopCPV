#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

full_sel_sr_unc /wk_cms2/cychuang/UncPreSel_NS/VV/WW_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_ VV/WW_TuneCUETP8M1_13TeV-pythia8/ 0
full_sel_sr_unc /wk_cms2/cychuang/UncPreSel_NS/VV/WZ_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_ VV/WZ_TuneCUETP8M1_13TeV-pythia8/ 0
full_sel_sr_unc /wk_cms2/cychuang/UncPreSel_NS/VV/ZZ_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_ VV/ZZ_TuneCUETP8M1_13TeV-pythia8/ 0

