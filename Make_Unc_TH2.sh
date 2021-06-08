#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

"""
Make_Unc_TH2 mva 0.22 nominal SR_signal

Make_Unc_TH2 mva 0.22 btag_up SR_signal
Make_Unc_TH2 mva 0.22 btag_down SR_signal
Make_Unc_TH2 mva 0.22 lepsf_up SR_signal
Make_Unc_TH2 mva 0.22 lepsf_down SR_signal
Make_Unc_TH2 mva 0.22 pu_up SR_signal
Make_Unc_TH2 mva 0.22 pu_down SR_signal
Make_Unc_TH2 mva 0.22 JER_up SR_signal
Make_Unc_TH2 mva 0.22 JER_down SR_signal
Make_Unc_TH2 mva 0.22 JES_up SR_signal
Make_Unc_TH2 mva 0.22 JES_down SR_signal
"""
Make_Unc_TH2 mva 0.22 nominal CR_data
#Make_Unc_TH2 mva 0.22 nominal SR_bkg
"""
Make_Unc_TH2 chi2 20. nominal SR_signal

Make_Unc_TH2 chi2 20. btag_up SR_signal
Make_Unc_TH2 chi2 20. btag_down SR_signal
Make_Unc_TH2 chi2 20. lepsf_up SR_signal
Make_Unc_TH2 chi2 20. lepsf_down SR_signal
Make_Unc_TH2 chi2 20. pu_up SR_signal
Make_Unc_TH2 chi2 20. pu_down SR_signal
Make_Unc_TH2 chi2 20. JER_up SR_signal
Make_Unc_TH2 chi2 20. JER_down SR_signal
Make_Unc_TH2 chi2 20. JES_up SR_signal
Make_Unc_TH2 chi2 20. JES_down SR_signal
"""
Make_Unc_TH2 chi2 20. nominal CR_data
#Make_Unc_TH2 chi2 20. nominal SR_bkg

