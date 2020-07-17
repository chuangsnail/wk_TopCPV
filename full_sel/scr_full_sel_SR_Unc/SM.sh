#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

full_sel_sr_unc /wk_cms2/cychuang/dbl_pre_16/SingleMuon/Run2016B/pre_sel_bpk_ntuple_ SingleMuon/Run2016B/ 1
full_sel_sr_unc /wk_cms2/cychuang/dbl_pre_16/SingleMuon/Run2016C/pre_sel_bpk_ntuple_ SingleMuon/Run2016C/ 1
full_sel_sr_unc /wk_cms2/cychuang/dbl_pre_16/SingleMuon/Run2016D/pre_sel_bpk_ntuple_ SingleMuon/Run2016D/ 1
full_sel_sr_unc /wk_cms2/cychuang/dbl_pre_16/SingleMuon/Run2016E/pre_sel_bpk_ntuple_ SingleMuon/Run2016E/ 1
full_sel_sr_unc /wk_cms2/cychuang/dbl_pre_16/SingleMuon/Run2016F/pre_sel_bpk_ntuple_ SingleMuon/Run2016F/ 1
full_sel_sr_unc /wk_cms2/cychuang/dbl_pre_16/SingleMuon/Run2016G/pre_sel_bpk_ntuple_ SingleMuon/Run2016G/ 1
full_sel_sr_unc /wk_cms2/cychuang/dbl_pre_16/SingleMuon/Run2016H/pre_sel_bpk_ntuple_ SingleMuon/Run2016H/ 1

