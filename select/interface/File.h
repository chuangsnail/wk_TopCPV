#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <vector>
#include "stdlib.h"
#include "stdio.h"

#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"		//which is in the library "roofit"
#include "TPad.h"			//for gPad
#include "THStack.h"
#include "TColor.h"
#include "TLegend.h"

#include "TopCPViolation/select/interface/BranchInfo.h"

using namespace std;


class File
{
public:
	char files_name[400];
	TChain* ch;
	string tree_name;	
	double lumi_weight;
	
	File( char* f_name = NULL )
	{
		//files_name = new char[300];
		
		//files_name = f_name;

		tree_name = "root";

		for(int i=0;i<400;i++)
		{
			files_name[i] = f_name[i]; 
		}
		
		ch = new TChain( (char*)tree_name.c_str() );	//because in bprimekit ntuple, the default TChain name is 'root'
		ch->Add( files_name );
		
		lumi_weight = 1. ;
	}
		
	~File()
	{
		//delete [] Files_name;
		//Files_name = NULL;
		//delete ch;

		//'delete' if there is a 'new'
		//so here we don't need to delete
		//ch = NULL;
	}

	void AddFile( char* file )
	{
		ch->Add( file );
	}
	
	void SetLumiWeight( double w )
	{
		lumi_weight = w;
	}

	void SetTreeName( string& new_tree_name )
	{
		tree_name = new_tree_name;	
	}
	
};

/*
class InputFileManager
{
public:

	void 

};
*/

#endif
