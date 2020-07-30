#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

UncPreSel /wk_cms2/youying/public/bprimekit_2016_Legacy/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/bpk_ntuple_ TTbar/ 0

