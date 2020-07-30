#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

UncPreSel /wk_cms2/youying/public/bprimekit_2016_Legacy/WW_TuneCUETP8M1_13TeV-pythia8/bpk_ntuple_ VV/WW_TuneCUETP8M1_13TeV-pythia8/ 0
UncPreSel /wk_cms2/youying/public/bprimekit_2016_Legacy/WZ_TuneCUETP8M1_13TeV-pythia8/bpk_ntuple_ VV/WZ_TuneCUETP8M1_13TeV-pythia8/ 0
UncPreSel /wk_cms2/youying/public/bprimekit_2016_Legacy/ZZ_TuneCUETP8M1_13TeV-pythia8/bpk_ntuple_ VV/ZZ_TuneCUETP8M1_13TeV-pythia8/ 0

