#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation && eval `scramv1 runtime -sh`

Unc_Fit nominal mu O3 1 5000
Unc_Fit nominal mu O6 2 5000
Unc_Fit nominal mu O12 3 5000
Unc_Fit nominal mu O14 4 5000

"""
Unc_Fit nominal el O3 5 5000
Unc_Fit nominal el O6 6 5000
Unc_Fit nominal el O12 7 5000
Unc_Fit nominal el O14 8 5000
"""

Unc_Fit nominal mu O3 9 5000
Unc_Fit nominal mu O6 10 5000
Unc_Fit nominal mu O12 11 5000
Unc_Fit nominal mu O14 12 5000
"""
Unc_Fit nominal el O3 13 5000
Unc_Fit nominal el O6 14 5000
Unc_Fit nominal el O12 15 5000
Unc_Fit nominal el O14 16 5000
"""
