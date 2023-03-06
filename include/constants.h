#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <unordered_map>
#include <iostream>
#include "TROOT.h"
#include "TString.h"

// Analysis
// Integrations of Zh at 0.2
const int Zh_cutoff = 2;

// Systematic Errors settings
const int total_syst      = 10;
const int syst_variations = 2;

// Name templates
std::string meanpt2_file_template    = "results-meanpt2-";
std::string broadening_file_template = "results-broadening-";
std::string extension                = ".root";
std::string input_dir                = "/home/esteban/work/ptbroadening-systerr-analysis/input-files/";

std::string syst_legend[9][2] = {{", |#Delta Z|<2.5cm"      ,", |#Delta Z|<3.5cm"      },
                                     {", Acc>0.005"             ,", Acc>0.01"              },
                                     {", CT P^{2}_{T} Bins = 50",", CT P^{2}_{T} Bins = 70"},
                                     {", N_{acc}>0"             ,", N_{acc}>2"             }, 
                                     {", P^{2}_{T} Bins = 70"   ,", P^{2}_{T} Bins = 110"  },
                                     {", RC Treated"            ,", RC Not Treated"        },
                                     {", Tail with cutoff"      ,", Tail Not Treated"      },
                                     {", TOF P<2.5GeV"          ,", TOF P<2.9GeV"          },
                                     {", Vertex Cut HH"         ,", Vertex Cut RD"         }};
std::string syst[9]           = {"absdeltaz", "accminlim", "ct", "naccept", "npt2", "rad", "bgreduction", "tof", "vc"};

// Binning configuration
const int N_Q2  = 3;
const int N_Nu  = 3;
const int N_Zh  = 8;

const float Q2_limits[N_Q2+1]   = { 1.0 , 1.3 , 1.8 , 4.0 };
const float Nu_limits[N_Nu+1]   = { 2.2 , 3.2 , 3.7 , 4.26};
const float Zh_limits[N_Zh+1]   = { 0.0 , 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.8 , 1.0 };

// Targets
const int N_targets    = 6;
const int N_broadening = 3;

const double A13[N_targets] = { pow(2,1./3.) , pow(2,1./3.) , pow(2,1./3.) , pow(12,1./3.) , pow(55, 1./3.) , pow(208,1./3.) };

std::string targets[N_targets]               = { "DC" , "DFe" , "DPb" , "C" , "Fe" , "Pb" };
std::string broadening_targets[N_broadening] = { "C" , "Fe" , "Pb" };


// Visual Settings : General

const int targ_colors[N_targets] = { 2 , 4 , 1 , 2 , 4 , 1 };
const int targ_marker[syst_variations] = { 22 , 23 };

const int zh_colors[N_Zh]        = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8};
const int zh_marker[N_Zh]        = { 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27};

const double shift_x_q2[N_targets]  = { -0.1   , 0 , 0.1   , -0.1   , 0 , 0.1   };
const double shift_x_nu[N_targets]  = { -0.1   , 0 , 0.1   , -0.1   , 0 , 0.1   };
const double shift_x_zh[N_targets]  = { -0.015 , 0 , 0.015 , -0.015 , 0 , 0.015 };

const double shift_x_a13[4] = { -0.075 , -0.025 , 0.025 , 0.075 };

// Visual Settings : Deviation plots

const double dev_pads_bmargin[3] = { 0.1  , 0.1  , 0.1  };
const double dev_pads_tmargin[3] = { 0.05 , 0.05 , 0.05 };
const double dev_pads_lmargin[3] = { 0.12 , 0.06 , 0.06 };
const double dev_pads_rmargin[3] = { 0.06 , 0.06 , 0.06 };

const double dev_legend_deltaX = 0.15;
const double dev_legend_deltaY = 0.17;

const double dev_legend_X1[N_broadening] = { 0.14                     , 0.08                     , 0.08                     }; 
const double dev_legend_X2[N_broadening] = { 0.14 + dev_legend_deltaX , 0.08 + dev_legend_deltaX , 0.08 + dev_legend_deltaX };
const double dev_legend_Y1[N_broadening] = { 0.76                     , 0.76                     , 0.76                     };
const double dev_legend_Y2[N_broadening] = { 0.76 + dev_legend_deltaY , 0.76 + dev_legend_deltaY , 0.76 + dev_legend_deltaY };

#endif /* CONSTANTS_H */
