/**********************************
 *
 *	File Name : Unc_Fit.cc
 *	Date : 200622
 *	Description : 
 *	Usage : exe {condition} {channel} {obs} {rnd seed} {set #} {mva/chi2}	
 *
 * ********************************/

#include "TopCPViolation/select/interface/SelMgr.h"
#include "TopCPViolation/select/interface/about_time.h"
#include "TopCPViolation/select/interface/fit_yields.h"

using namespace std;


int main(int argc,char* argv[])
{

	if( argc == 7 )
	{
		string condition = string( argv[1] );
		string channel = string( argv[2] );
		string obs = string( argv[3] );

		string seed_str = string( argv[4] );
		string set_no_str = string( argv[5] );
		int seed = stoi( seed_str );
		int set_no = stoi( set_no_str );

		string strategy = string( argv[6] );

		string start_time = get_time_str( minute );

		string path = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/UncTemplate/"+ strategy + "/";

		Get_fit_result( condition, \
			string( path + "Mlb_Obs-nominal-SR_signal-2ch-bin100_50.root" ),				\
			string( path + "Mlb_Obs-nominal-SR_bkg-2ch-bin100_50.root" ),					\
			string( path + "Mlb_Obs-" + condition + "-SR_signal-2ch-bin100_50.root" ),	\
			string( path + "Mlb_Obs-nominal-CR_data-2ch-bin100_50.root" ),					\
			channel,	\
			obs,		\
			seed,		\
			set_no,		\
			strategy	\
			);

		string end_time = get_time_str( minute );

		cout << "start time : " << start_time << ", end time : " << end_time << endl;
		cerr << endl << "[END]" << endl;
	}
	else if( argc == 5 )
	{
		string condition = string( argv[1] );
		string seed_str = string( argv[2] );
		string set_no_str = string( argv[3] );
		int seed = stoi( seed_str );
		int set_no = stoi( set_no_str );

		string strategy = string( argv[6] );
		
		string start_time = get_time_str( minute );
		
        string chs[2] = { "mu", "el" };
        string obss[4] = { "O3", "O6", "O12", "O14" };

        for(int i=0;i<2;++i)
        {
            for(int j=0;j<4;++j)
            {
                Get_fit_result( condition, \
                    "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/UncTemplate/mva/Mlb_Obs-nominal-SR_signal-2ch-bin100_50.root",				\
                    "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/UncTemplate/mva/Mlb_Obs-nominal-SR_bkg-2ch-bin100_50.root",					\
                    string("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/UncTemplate/mva/Mlb_Obs-" + condition + "-SR_signal-2ch-bin100_50.root"),	\
                    "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/UncTemplate/mva/Mlb_Obs-nominal-CR_data-2ch-bin100_50.root",					\
                    chs[i],	\
                    obss[j],		\
                    seed,		\
                    set_no,		\
					strategy	\
                    );
                seed++; 
            }
        }

		
		string end_time = get_time_str( minute );

		cout << "start time : " << start_time << ", end time : " << end_time << endl;
		cerr << endl << "[END]" << endl;
	}

    return 0;
}

