//#include "TopCPViolation/selected/interface/Reweight.h"
#include "../interface/Reweight.h"


double SF_2D::GetTH2FBinContent(double _x,double _y)
{
	int _xLabel = 1, _yLabel = 1;
	for(int i=1;i<=h->GetXaxis()->GetNbins();i++)
	{
		double low_edge = h->GetXaxis()->GetBinLowEdge(i);
		double up_edge = h->GetXaxis()->GetBinUpEdge(i);
		if( ( _x > low_edge  ) && ( _x <= up_edge ) )
		{
			_xLabel = i;
			break;
		}
	}	
	for(int j=1;j<=h->GetYaxis()->GetNbins();j++)
	{
		double low_edge = h->GetYaxis()->GetBinLowEdge(j);
		double up_edge = h->GetYaxis()->GetBinUpEdge(j);
		if( ( _y > low_edge  ) && ( _y <= up_edge ) )
		{
			_yLabel = j;
			break;
		}
	}
	double result = 0.;
	result = h->GetBinContent( _xLabel , _yLabel );
	
	return result;
}

double SF_2D::GetTH2DBinContent(double _x,double _y)
{
	int _xLabel = 1, _yLabel = 1;
	for(int i=1;i<=hd->GetXaxis()->GetNbins();i++)
	{
		double low_edge = hd->GetXaxis()->GetBinLowEdge(i);
		double up_edge = hd->GetXaxis()->GetBinUpEdge(i);
		if( ( _x >= low_edge  ) && ( _x < up_edge ) )
		{
			_xLabel = i;
			break;
		}
	}	
	for(int j=1;j<=hd->GetYaxis()->GetNbins();j++)
	{
		double low_edge = hd->GetYaxis()->GetBinLowEdge(j);
		double up_edge = hd->GetYaxis()->GetBinUpEdge(j);
		if( ( _y >= low_edge  ) && ( _y < up_edge ) )
		{
			_yLabel = j;
			break;
		}
	}
	double result = 0.;
	result = hd->GetBinContent( _xLabel , _yLabel );
	
	return result;
}

double LeptonSFMgr::GetLepSF( const string& ch, const double& pt, const double& eta )
{
	double sf = 1.;
	if( ch == "mu" ) {
		if( h_tightMuIDSF != NULL) {
			SF_2D SF( h_tightMuIDSF );
			sf *= SF.GetTH2FBinContent( eta , pt );
		}
		if( h_MuISOSF != NULL) {
			SF_2D SF( h_MuISOSF );
			sf *= SF.GetTH2DBinContent( eta , pt );
		}
		if( h_MuTrgSF != NULL) {
			SF_2D SF( h_MuTrgSF );
			sf *= SF.GetTH2FBinContent( eta , pt );
		}
	}
	else if( ch == "el" ) {
		if( h_tightElIDSF != NULL) {
			SF_2D SF( h_tightElIDSF );
			sf *= SF.GetTH2FBinContent( eta , pt );
		}
		if( h_ElRECOSF != NULL) {
			SF_2D SF( h_ElRECOSF );
			sf *= SF.GetTH2FBinContent( eta , pt );
		}
		if( h_ElTrgSF != NULL) {
			SF_2D SF( h_ElTrgSF );
			sf *= SF.GetTH2DBinContent( eta , pt );
		}
	}
	
	return sf;
}



void PileUpMgr::RegInPUvec( const string& filename )
{
	
	ifstream fin;
	fin.open( (char*)filename.c_str() );
	//fstream fin;
	//fin.open( file_name , ios::in );
	
	if(!fin){ 
		cerr << "there is something wrong in opening the file \"" << filename <<  "\"" << endl;
   	}

	string input_str;
	while( getline( fin, input_str ) ) {
		pileupweights.push_back( stod( input_str ) );
	}
	
	fin.close();
}


bool PileUpMgr::PassPileUpWeight()
{
	int _nPU = -1;
	
	for(int i=0;i<(int)evt->nBX;i++) {
		if(evt->BXPU[i] == 0) {
			_nPU = evt->TrueIT[i];
			break;
		}
	}
	
	if( _nPU <= 74 && _nPU >= 0 ) {
		PU_weight = (double)(pileupweights.at( _nPU ));
	}
	else {
		PU_weight = 0.;
		return false;
	}
	return true;
}

void HLTMgr::GetHLTinfo( const string& key )
{
	if(key == "MC_mu")
	{

		HLT_MC_mu.push_back( 1319 );
		HLT_MC_mu.push_back( 2415 );
/*		
		HLT_info.push_back( 2412 );
		HLT_info.push_back( 2413 );
		HLT_info.push_back( 2414 );
		HLT_info.push_back( 2415 );
*/
	}
	else if(key == "Data_mu")
	{
		HLT_Data_mu.push_back( 1315 );	
		HLT_Data_mu.push_back( 1316 );	
		HLT_Data_mu.push_back( 1317 );	
		HLT_Data_mu.push_back( 1318 );	
		HLT_Data_mu.push_back( 1319 );	
		HLT_Data_mu.push_back( 1320 );	
		HLT_Data_mu.push_back( 1321 );	
		HLT_Data_mu.push_back( 1322 );	
		HLT_Data_mu.push_back( 1323 );	
		HLT_Data_mu.push_back( 1324 );
		
		HLT_Data_mu.push_back( 2412 );			
		HLT_Data_mu.push_back( 2413 );			
		HLT_Data_mu.push_back( 2414 );			
		HLT_Data_mu.push_back( 2415 );
	}
	else if(key == "MC_el")
	{

		HLT_MC_el.push_back( 923 );	
		
/*		
		HLT_MC_el.push_back( 2412 );
		HLT_MC_el.push_back( 2413 );
		HLT_MC_el.push_back( 2414 );
		HLT_MC_el.push_back( 2415 );
*/
	}
	else if(key == "Data_el")
	{
		HLT_Data_el.push_back( 917 );		
		HLT_Data_el.push_back( 918 );		
		HLT_Data_el.push_back( 919 );		
		HLT_Data_el.push_back( 920 );		
		HLT_Data_el.push_back( 921 );		
		HLT_Data_el.push_back( 922 );		
		HLT_Data_el.push_back( 923 );		
	}
	else
	{	cout << "There is no option of this HLT trigger!" << endl;	}

}

bool HLTMgr::PassHLT( const bool& _isdata, const string& ch )
{
	vector<int>* info;

	if(_isdata) {
		if( ch == "el" )
			info = &HLT_Data_el;
		else if( ch == "mu" )
			info = &HLT_Data_mu;
	}
	else {
		if( ch == "el" )
			info = &HLT_MC_el;
		else if( ch == "mu" )
			info = &HLT_MC_mu;
	}

	assert( info != NULL );
	//if( (int)info->size() == 0 ) return true;

	for(int i=0;i<(int)info->size();i++){
		if( trg->TrgBook[ (int)info->at(i) ] == 1 )	return true;
	}	
	
	return false;
}








