#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src && eval `scramv1 runtime -sh`

full_sel_cr_inv /wk_cms2/cychuang/dbl_pre_sel_file_2016legacy/SingleTop/ST_s-channel_4f_leptonDecays_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/pre_sel_bpk_ntuple_ 16_full_CR_inv/SingleTop/ST_s-channel_4f_leptonDecays_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/ 0
full_sel_cr_inv /wk_cms2/cychuang/dbl_pre_sel_file_2016legacy/SingleTop/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/pre_sel_bpk_ntuple_ 16_full_CR_inv/SingleTop/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/ 0
full_sel_cr_inv /wk_cms2/cychuang/dbl_pre_sel_file_2016legacy/SingleTop/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/pre_sel_bpk_ntuple_ 16_full_CR_inv/SingleTop/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/ 0
full_sel_cr_inv /wk_cms2/cychuang/dbl_pre_sel_file_2016legacy/SingleTop/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/pre_sel_bpk_ntuple_ 16_full_CR_inv/SingleTop/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/ 0
full_sel_cr_inv /wk_cms2/cychuang/dbl_pre_sel_file_2016legacy/SingleTop/ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/pre_sel_bpk_ntuple_ 16_full_CR_inv/SingleTop/ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/ 0

