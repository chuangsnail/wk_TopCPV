#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

full_sel_sr_unc /wk_cms2/youying/public/bprimekit_2016_Legacy/SingleMuon/Run2016B/bpk_ntuple_ SingleMuon/Run2016B/ 1
full_sel_sr_unc /wk_cms2/youying/public/bprimekit_2016_Legacy/SingleMuon/Run2016C/bpk_ntuple_ SingleMuon/Run2016C/ 1
full_sel_sr_unc /wk_cms2/youying/public/bprimekit_2016_Legacy/SingleMuon/Run2016D/bpk_ntuple_ SingleMuon/Run2016D/ 1
full_sel_sr_unc /wk_cms2/youying/public/bprimekit_2016_Legacy/SingleMuon/Run2016E/bpk_ntuple_ SingleMuon/Run2016E/ 1
full_sel_sr_unc /wk_cms2/youying/public/bprimekit_2016_Legacy/SingleMuon/Run2016F/bpk_ntuple_ SingleMuon/Run2016F/ 1
full_sel_sr_unc /wk_cms2/youying/public/bprimekit_2016_Legacy/SingleMuon/Run2016G/bpk_ntuple_ SingleMuon/Run2016G/ 1
full_sel_sr_unc /wk_cms2/youying/public/bprimekit_2016_Legacy/SingleMuon/Run2016H/bpk_ntuple_ SingleMuon/Run2016H/ 1

