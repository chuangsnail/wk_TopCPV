#!/bin/env python
# -*- coding: utf-8 -*-

import ROOT
import argparse
import sys
from array import array
import subprocess
import os
from math import sqrt

import re

def GausFit( _h, name ):
    
    obs_names = re.split('(\d+)',name[3])
    obs_name = obs_names[0] + "_{" + obs_names[1] + "}"
    
    sb = ""
    title_name = ""
    print "_h.GetName() " + _h.GetName()

    if "sig" in _h.GetName():
        title_name = "Signal yields' syst. [%]"
        sb = "sig"
    elif "bkg" in _h.GetName():
        title_name = "Background yields' syst. [%]"
        sb = "bkg"

    x = ROOT.RooRealVar( "x", title_name + " (" + obs_name+ ")", _h.GetXaxis().GetXmin(), _h.GetXaxis().GetXmax() )
    _h.SetDirectory( 0 )
    _data = ROOT.RooDataHist( "data", "dh", ROOT.RooArgList(x), _h )
    mean = ROOT.RooRealVar( "mean" ,"Mean of Gaussian" , _h.GetMean(), -20. ,20. )
    sigma = ROOT.RooRealVar( "sigma" ,"Width of Gaussian" , _h.GetStdDev(), -100. ,100. )
    
    _data.SetTitle("")

    gauss = ROOT.RooGaussian( "gauss" ,"gauss(x,mean,sigma)" ,x ,mean ,sigma )

    result = gauss.fitTo( _data, ROOT.RooFit.PrintLevel(-1) )

    c = ROOT.TCanvas()
    xframe = x.frame( ROOT.RooFit.Title(" ") )
    _data.plotOn( xframe, ROOT.RooLinkedList() )
    gauss.plotOn( xframe )
    
    leg = ROOT.TLegend( 0.7, 0.7, 0.85, 0.85 )
    leg.AddEntry( 0, "Gaus. Fit" )
    leg.AddEntry( 0, "#mu : " + "%2.2f" % mean.getValV() + "%", "l" )
    leg.AddEntry( 0, "#sigma : " + "%2.2f" % sigma.getValV() + "%", "l" )

    tl = ROOT.TLatex()
    tl.SetNDC( True )
    tl.SetTextFont(43)
    tl.SetTextSize(22)
    tl.SetTextAlign(33)

    xframe.Draw()
    leg.Draw("same")
    tl.DrawLatex(0.89, 0.95, "#bf{5000 datasets}")

    c.Print( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/distribution/yields_syst_fit/Fitted_yields_syst_" + name[0] + "_" + name[1] + "_" + name[2] + "_" + name[3] + "_" + sb + ".pdf" )

    return mean.getValV(), sigma.getValV();


def main(args) :

    parser = argparse.ArgumentParser( "Convert from .hle to .root" )

    parser.add_argument( '-s', '--syst', dest='syst',type=str, required=True )
    parser.add_argument( '-c', '--channel', dest='channel',type=str, required=True )
    parser.add_argument( '-v', '--varied', dest='varied',type=str, required=True )  #'up', 'down'
    parser.add_argument( '-o', '--obs', dest='obs',type=str, required=True )
    try:
        opt = parser.parse_args(args[1:])
    except:
        print "[Error] argument parser!"
        parser.print_help()
        raise
    
    name_list = [ opt.syst, opt.varied, opt.channel, opt.obs ]
    
    mean_sig, width_sig, mean_bkg, width_bkg = Get_Yields_Syst( name_list )
    
    var_name = name_list[0] + "_" + name_list[1] + "_" + name_list[2] + "_" + name_list[3];
    
    f2 = open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/yields_syst_result.txt", "a" )
    f2.write( var_name + " SigY Mean : " + str( mean_sig ) + " , " )
    f2.write( "StdDev : " + str( width_sig ) + '\n' )
    f2.write( var_name + " BkgY Mean : " + str( mean_bkg ) + " , " )
    f2.write( "StdDev : " + str( width_bkg ) + "\n\n" )
    f2.close()


def Get_Yields_Syst( name_list ):
    
    var_name = name_list[0] + "_" + name_list[1] + "_" + name_list[2] + "_" + name_list[3];
    nom_name = "nominal" + "_" + name_list[2] + "_" + name_list[3];

    filename1 = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/result/" + var_name + ".txt"
    f = open( filename1 , "r")
    contents_v = f.readlines()
    f.close()           # need to be close, or f1 would not store things in it
    
    filename2 = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/result/" + nom_name + ".txt"
    f = open( filename2 , "r")
    contents_n = f.readlines()
    f.close()           # need to be close, or f1 would not store things in it

    #### to decide the range of histogram ####

    tmp_sig_y_mean_n = 0.
    tmp_bkg_y_mean_n = 0.
    tmp_sig_y_mean_v = 0.
    tmp_bkg_y_mean_v = 0.
    
    tmp_sig_y_width_n = 0.
    tmp_bkg_y_width_n = 0.

    tmp_sig_s_mean = 0.
    tmp_bkg_s_mean = 0.
    
    tmp_sig_s_width = 0.
    tmp_bkg_s_width = 0.

    test_no = 1000.
    for i in range(int(test_no)):
        j = i
        if "down" in name_list:
            j = i + 5000
        line_n = contents_n[j]
        line_v = contents_v[i]

        words_n = line_n.split(",")
        sig_y_n =     float( words_n[4] ) - float( words_n[6] )
        bkg_y_n =     float( words_n[6] )

        words_v = line_v.split(",")
        sig_y_v =     float( words_v[4] ) - float( words_v[6] )
        bkg_y_v =     float( words_v[6] )
        
        tmp_sig_y_mean_n = tmp_sig_y_mean_n + sig_y_n/test_no
        tmp_sig_y_mean_v = tmp_sig_y_mean_v + sig_y_v/test_no
        tmp_bkg_y_mean_n = tmp_bkg_y_mean_n + bkg_y_n/test_no
        tmp_bkg_y_mean_v = tmp_bkg_y_mean_v + bkg_y_v/test_no

        tmp_sig_y_width_n = tmp_sig_y_width_n + sig_y_n*sig_y_n/test_no
        tmp_bkg_y_width_n = tmp_bkg_y_width_n + bkg_y_n*bkg_y_n/test_no
        
        sig_syst = ( ( sig_y_v - sig_y_n )/ sig_y_n ) * 100;
        bkg_syst = ( ( bkg_y_v - bkg_y_n )/ bkg_y_n ) * 100;

        tmp_sig_s_mean += sig_syst/test_no
        tmp_bkg_s_mean += bkg_syst/test_no

        tmp_sig_s_width += sig_syst**2/test_no
        tmp_bkg_s_width += bkg_syst**2/test_no

    tmp_sig_y_width_n = sqrt( tmp_sig_y_width_n - tmp_sig_y_mean_n**2 )
    tmp_bkg_y_width_n = sqrt( tmp_bkg_y_width_n - tmp_bkg_y_mean_n**2 )

    tmp_sig_s_width = sqrt( tmp_sig_s_width - tmp_sig_s_mean**2 )
    tmp_bkg_s_width = sqrt( tmp_bkg_s_width - tmp_bkg_s_mean**2 )

#    tmp_sig_y_mean = ( tmp_sig_y_mean_n + tmp_sig_y_mean_v )/2.
#    tmp_bkg_y_mean = ( tmp_bkg_y_mean_n + tmp_bkg_y_mean_v )/2.
#    tmp_sig_y_range = abs( tmp_sig_y_mean_n - tmp_sig_y_mean_v ) * 8
#    tmp_bkg_y_range = abs( tmp_bkg_y_mean_n - tmp_bkg_y_mean_v ) * 8
#
#    h_sig_yields_v = ROOT.TH1F( "h_sig_yields_v", ";Signal yields;Events", 100, tmp_sig_y_mean - tmp_sig_y_range, tmp_sig_y_mean + tmp_sig_y_range )
#    h_sig_yields_n = ROOT.TH1F( "h_sig_yields_n", ";Signal yields;Events", 100, tmp_sig_y_mean - tmp_sig_y_range, tmp_sig_y_mean + tmp_sig_y_range )
#    h_bkg_yields_v = ROOT.TH1F( "h_bkg_yields_v", ";Background yields;Events", 100, tmp_bkg_y_mean - tmp_bkg_y_range, tmp_bkg_y_mean + tmp_bkg_y_range )
#    h_bkg_yields_n = ROOT.TH1F( "h_bkg_yields_n", ";Background yields;Events", 100, tmp_bkg_y_mean - tmp_bkg_y_range, tmp_bkg_y_mean + tmp_bkg_y_range )
#    h_sig_syst = ROOT.TH1F( "h_sig_syst", "", 250, -0.01, 0.01 )
#    h_bkg_syst = ROOT.TH1F( "h_bkg_syst", "", 300, -0.15, 0.15 )

    h_sig_yields_v = ROOT.TH1F( "h_sig_yields_v", ";Signal yields;Sets", 100, tmp_sig_y_mean_n - tmp_sig_y_width_n*4.5, tmp_sig_y_mean_n + tmp_sig_y_width_n*4.5 )
    h_sig_yields_n = ROOT.TH1F( "h_sig_yields_n", ";Signal yields;Sets", 100, tmp_sig_y_mean_n - tmp_sig_y_width_n*4.5, tmp_sig_y_mean_n + tmp_sig_y_width_n*4.5 )
    h_bkg_yields_v = ROOT.TH1F( "h_bkg_yields_v", ";Background yields;Sets", 100, tmp_bkg_y_mean_n - tmp_bkg_y_width_n*4.5, tmp_bkg_y_mean_n + tmp_bkg_y_width_n*4.5 )
    h_bkg_yields_n = ROOT.TH1F( "h_bkg_yields_n", ";Background yields;Sets", 100, tmp_bkg_y_mean_n - tmp_bkg_y_width_n*4.5, tmp_bkg_y_mean_n + tmp_bkg_y_width_n*4.5 )
    h_sig_syst = ROOT.TH1F( "h_sig_syst", "", 100, tmp_sig_s_mean - tmp_sig_s_width*3.5, tmp_sig_s_mean + tmp_sig_s_width*3.5 )
    h_bkg_syst = ROOT.TH1F( "h_bkg_syst", "", 100, tmp_bkg_s_mean - tmp_bkg_s_width*3.5, tmp_bkg_s_mean + tmp_bkg_s_width*3.5 )

#    print "tmp_sig_s_mean : " +  str( tmp_sig_s_mean )
#    print "tmp_sig_s_w : " +  str( tmp_sig_s_width )
#    print "tmp_bkg_s_mean : " +  str( tmp_bkg_s_mean )
#    print "tmp_bkg_s_w : " +  str( tmp_bkg_s_width )

    ### start to loop all the samples ###

    for i in range(5000):
        j = i
        if "down" in name_list:
            j = i + 5000
        line_n = contents_n[j]
        line_v = contents_v[i]

        words_n = line_n.split(",")
        sig_y_n =     float( words_n[4] ) - float( words_n[6] )
        bkg_y_n =     float( words_n[6] )

        words_v = line_v.split(",")
        sig_y_v =     float( words_v[4] ) - float( words_v[6] )
        bkg_y_v =     float( words_v[6] )

        sig_syst = ( ( sig_y_v - sig_y_n )/ sig_y_n ) * 100;
        bkg_syst = ( ( bkg_y_v - bkg_y_n )/ bkg_y_n ) * 100;

        h_sig_yields_v.Fill( sig_y_v )
        h_sig_yields_n.Fill( sig_y_n )
        h_bkg_yields_v.Fill( bkg_y_v )
        h_bkg_yields_n.Fill( bkg_y_n )

        h_sig_syst.Fill( sig_syst )
        h_bkg_syst.Fill( bkg_syst )

    #mean = h_syst.GetMean()
    #width = h_syst.GetStdDev()

    mean_sig, width_sig = GausFit( h_sig_syst, name_list )
    mean_bkg, width_bkg = GausFit( h_bkg_syst, name_list )

    c1 = ROOT.TCanvas()
    
    tl = ROOT.TLatex()
    tl.SetNDC( True )
    tl.SetTextFont(43)
    tl.SetTextSize(22)
    tl.SetTextAlign(33)

    h_sig_yields_n.SetLineColor( 1 )
    h_sig_yields_v.SetLineColor( 4 )
    
    h_sig_yields_n.SetLineWidth( 2 )
    h_sig_yields_v.SetLineWidth( 3 )

    h_sig_yields_n.SetStats( False )
    h_sig_yields_v.SetStats( False )
    
    leg1 = ROOT.TLegend( 0.725, 0.725, 0.85, 0.85 )
    leg1.AddEntry( h_sig_yields_n, "Nominal", "l" )
    leg1.AddEntry( h_sig_yields_v, "w/ syst. error", "l" )
    
    h_sig_yields_n.SetMaximum( h_sig_yields_n.GetMaximum() * 1.3 )
    h_sig_yields_v.SetMaximum( h_sig_yields_n.GetMaximum() * 1.3 )

    h_sig_yields_n.Draw()
    h_sig_yields_v.Draw( "same" )
    leg1.Draw( "same" )
    tl.DrawLatex(0.89, 0.95, "#bf{5000 datasets}")
    
    pdf_name1 = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/distribution/dev_yields/Dev_sig_yields_" + var_name + ".pdf"

    c1.Print( pdf_name1 )

    c2 = ROOT.TCanvas()

    h_bkg_yields_n.SetLineColor( 1 )
    h_bkg_yields_v.SetLineColor( 4 )
    
    h_bkg_yields_n.SetLineWidth( 2 )
    h_bkg_yields_v.SetLineWidth( 3 )
    
    leg2 = ROOT.TLegend( 0.725, 0.725, 0.85, 0.85 )
    leg2.AddEntry( h_bkg_yields_n, "Nominal", "l" )
    leg2.AddEntry( h_bkg_yields_v, "w/ syst. error", "l" )
    
    h_bkg_yields_n.SetMaximum( h_bkg_yields_n.GetMaximum() * 1.3 )
    h_bkg_yields_v.SetMaximum( h_bkg_yields_n.GetMaximum() * 1.3 )
    
    h_bkg_yields_n.SetStats( False )
    h_bkg_yields_v.SetStats( False )

    h_bkg_yields_n.Draw()
    h_bkg_yields_v.Draw( "same" )
    leg2.Draw( "same" )
    tl.DrawLatex(0.86, 0.95, "#bf{5000 datasets}")
    
    pdf_name2 = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/distribution/dev_yields/Dev_bkg_yields_" + var_name + ".pdf"

    c2.Print( pdf_name2 )

    #print "sig_syst " + str( h_sig_syst.GetMean() )
    #print "bkg_syst " + str( h_bkg_syst.GetMean() )

    return mean_sig, width_sig, mean_bkg, width_bkg;


if __name__ == '__main__':
    main(sys.argv)

