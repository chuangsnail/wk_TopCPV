#!/bin/env python
# -*- coding: utf-8 -*-

import ROOT
from ROOT import TAttLine
from ROOT import TCanvas, TColor, TGaxis, TH1F, TPad, TLegend
from ROOT import kBlack, kBlue, kRed
import argparse
import sys
from array import array
import subprocess
import os
import re

def GausFit( _h, name ):
    
    obs_names = re.split('(\d+)',name[3])
    obs_name = obs_names[0] + "_{" + obs_names[1] + "}"

    x = ROOT.RooRealVar( "x", "A_{cp}(" + obs_name + ")\'s syst. [%]", _h.GetXaxis().GetXmin(), _h.GetXaxis().GetXmax() )
    _h.SetDirectory( 0 )
    _data = ROOT.RooDataHist( "data", "dh", ROOT.RooArgList(x), _h )
    mean = ROOT.RooRealVar( "mean" ,"Mean of Gaussian" ,-0.1 ,0.1 )
    sigma = ROOT.RooRealVar( "sigma" ,"Width of Gaussian" ,0.01 ,-0.1 ,0.1 )
    #mean = ROOT.RooRealVar( "mean" ,"Mean of Gaussian" ,-0.01 ,0.01 )
    #sigma = ROOT.RooRealVar( "sigma" ,"Width of Gaussian" ,0.001 ,-0.01 ,0.01 )

    gauss = ROOT.RooGaussian( "gauss" ,"gauss(x,mean,sigma)" ,x ,mean ,sigma )

    result = gauss.fitTo( _data, ROOT.RooFit.PrintLevel(-1) )

    c = ROOT.TCanvas()
    xframe = x.frame( ROOT.RooFit.Title(" ") )
    _data.plotOn( xframe, ROOT.RooLinkedList() )
    gauss.plotOn( xframe )
    
    _data.SetTitle( "" )
    #xframe.GetXaxis().SetLimits( xframe.GetXaxis().GetXmin()*100, xframe.GetXaxis().GetXmax()*100 )
    #xframe.GetYaxis().SetTitle( "Sets" )

    leg = TLegend( 0.7, 0.7, 0.85, 0.85 )
    leg.AddEntry( 0, "Gaus. Fit" )
    leg.AddEntry( 0, "#mu : " + "%.4f" % round(mean.getValV(), 4) + "%", "l" )
    leg.AddEntry( 0, "#sigma : " + "%.4f" % round(sigma.getValV(), 4) + "%", "l" )

    tl = ROOT.TLatex()
    tl.SetNDC( True )
    tl.SetTextFont(43)
    tl.SetTextSize(22)
    tl.SetTextAlign(33)

    xframe.Draw()
    leg.Draw( "SAME" )
    tl.DrawLatex(0.86, 0.95, "#bf{5000 datasets}")

    c.Print( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/distribution/obs_syst_fit/Fitted_syst_" + name[0] + "_" + name[1] + "_" + name[2] + "_" + name[3] + "_" + name[4] + ".pdf" )

    return mean.getValV(), sigma.getValV();


def GetSyst( name_list ):
    
    obs_names = re.split('(\d+)',name_list[3])
    obs_name = obs_names[0] + "_{" + obs_names[1] + "}"

    var_name = name_list[0] + "_" + name_list[1] + "_" + name_list[2] + "_" + name_list[3]
    nom_name = "nominal" + "_" + name_list[2] + "_" + name_list[3] 

    print "var_name : " + var_name
    print "nom_name : " + nom_name

    strategy_source = "chi2"
    if "mva" in name_list[4]:
        strategy_source = "mva"
        print "strategy_source is mva !!"

    filename = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/result/" + strategy_source + "/" + nom_name + ".txt"
    f = open( filename , "r" )
    contents_n = f.readlines()
    f.close()           # need to be close, or f1 would not store things in it

    filename = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/result/" + strategy_source + "/" + var_name + ".txt"
    f = open( filename , "r" )
    contents_v = f.readlines()
    f.close()           # need to be close, or f1 would not store things in it

    var_name =  var_name + "_" + name_list[4]
    nom_name =  nom_name + "_" + name_list[4]

    f1 = ROOT.TFile.Open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/distribution/obs_syst_root/h_syst_" + var_name + ".root", "RECREATE" )

# to decide the range of histogram

    #h_syst = ROOT.TH1F( "h_syst", "", 150, -0.00015, 0.00015 )
    h_syst = ROOT.TH1F( "h_syst", "", 150, -0.015, 0.015 )
    h_v = ROOT.TH1F( "h_v", ";A_{cp}(" + obs_name + ") [%];Sets", 50, -2, 2 )
    h_n = ROOT.TH1F( "h_n", ";A_{cp}(" + obs_name + ") [%];Sets", 50, -2, 2 )

    print "h_v title : " + str( h_v.GetTitle() )

    print "len( nom_# ):" + str( len( contents_n ) )
    print "len( var_# ):" + str( len( contents_v ) )

    #for ( line_n, line_v ) in ( contents_n, contents_v ) :
    for i in range(5000):
        j = i
        if "down" in name_list:
            j = i + 5000
        line_n = contents_n[j]
        line_v = contents_v[i]

        words_n = line_n.split(",")

		# chi2, MVA-A
        sig_y_n =     float( words_n[4] )
        sig_mr_n =    float( words_n[5] )
        bkg_y_n =     float( words_n[6] )
        bkg_mr_n =    float( words_n[7] )
        
        # MVA-B
        #sig_y_n =     float( words_n[0] )
        #sig_mr_n =    float( words_n[1] )
        #bkg_y_n =     float( words_n[2] )
        #bkg_mr_n =    float( words_n[3] )

        Np_n = sig_y_n * ( 1 - sig_mr_n ) - bkg_y_n * ( 1 - bkg_mr_n )
        Nn_n = sig_y_n * sig_mr_n - bkg_y_n * bkg_mr_n

        #Acp_n = ( Np_n - Nn_n )/( Np_n + Nn_n )
        Acp_n = ( ( Np_n - Nn_n )/( Np_n + Nn_n ) ) * 100

        words_v = line_v.split(",")
        # chi2, MVA-A
        sig_y_v =     float( words_v[4] )
        sig_mr_v =    float( words_v[5] )
        bkg_y_v =     float( words_v[6] )
        bkg_mr_v =    float( words_v[7] )
        
        # MVA-B
        #sig_y_v =     float( words_v[0] )
        #sig_mr_v =    float( words_v[1] )
        #bkg_y_v =     float( words_v[2] )
        #bkg_mr_v =    float( words_v[3] )

        Np_v = sig_y_v * ( 1 - sig_mr_v ) - bkg_y_v * ( 1 - bkg_mr_v )
        Nn_v = sig_y_v * sig_mr_v - bkg_y_v * bkg_mr_v

        #Acp_v = ( Np_v - Nn_v )/( Np_v + Nn_v )
        Acp_v = ( ( Np_v - Nn_v )/( Np_v + Nn_v ) ) * 100
        syst = Acp_v - Acp_n

        #print "Acp_v " + str( Acp_v ) 
        #print "Acp_n " + str( Acp_n ) 
        #print syst

        h_v.Fill( Acp_v )
        h_n.Fill( Acp_n )
        h_syst.Fill( syst )

    #mean = h_syst.GetMean()
    #width = h_syst.GetStdDev()

    mean, width = GausFit( h_syst, name_list )
    
    c = ROOT.TCanvas()

    h_v.SetLineColor( 4 )
    h_n.SetLineColor( 1 )
    
    h_v.SetLineWidth( 3 )
    h_n.SetLineWidth( 2 )

    leg = ROOT.TLegend( 0.725, 0.725, 0.85, 0.85 )
    leg.AddEntry( h_n, "Nominal", "l" )
    leg.AddEntry( h_v, "w/ syst. error", "l" )

    h_n.SetMaximum( h_n.GetMaximum() * 1.3 )
    h_v.SetMaximum( h_n.GetMaximum() * 1.3 )

    h_n.SetStats( False )
    h_v.SetStats( False )
    
    tl = ROOT.TLatex()
    tl.SetNDC( True )
    tl.SetTextFont(43)
    tl.SetTextSize(22)
    tl.SetTextAlign(33)
    
    h_n.Draw()
    h_v.Draw( "same" )
    leg.Draw( "same" )
    tl.DrawLatex(0.86, 0.95, "#bf{5000 datasets}")
    
    pdf_name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/distribution/dev_obs/Dev_Obs_" + var_name + ".pdf"

    c.Print( pdf_name )
    
    h_syst.Write()
    f1.Close()

    return mean, width;


def main(args) :

    parser = argparse.ArgumentParser( "Convert from .hle to .root" )

    parser.add_argument( '-s', '--syst', dest='syst',type=str, required=True )
    parser.add_argument( '-c', '--channel', dest='channel',type=str, required=True )
    parser.add_argument( '-o', '--obs', dest='obs',type=str, required=True )
    parser.add_argument( '-v', '--varied', dest='varied',type=str, required=True )  #'up', 'down'
    parser.add_argument( '-r', '--strategy', dest='strategy', type=str, default='mvaa' )
    #parser.add_argument( '-m', '--mlbcut', dest='mlbcut', type=bool, default=True )
    try:
        opt = parser.parse_args(args[1:])
    except:
        print "[Error] argument parser!"
        parser.print_help()
        raise


    name_list = [ opt.syst, opt.varied, opt.channel, opt.obs, opt.strategy ]


    mean, width = GetSyst( name_list )


    print "syst mean : " + str( mean )
    print "syst width : " + str( width )

    var_name = name_list[0] + "_" + name_list[1] + "_" + name_list[2] + "_" + name_list[3] + "_" + name_list[4];
    
    f2 = open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/systematic_result_" + opt.strategy + ".txt", "a" )
    f2.write( var_name + " Mean : " + str( mean ) )
    f2.write( " , StdDev : " + str( width ) + "\n\n" )
    f2.close()


if __name__ == '__main__':
    main(sys.argv)



