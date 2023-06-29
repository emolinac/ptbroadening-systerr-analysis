#include <iostream>
#include <string>
#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "constants.h"
#include "utils.h"

// Reminder 1: c_str() returns a pointer to an array filled with the characters of the string specified.

int main(int argc, char* argv[])
{
    // Open file that contains nominal results
    TFile* fin = new TFile(broadening_nominal_results.c_str());
    if(fin==NULL){std::cout<<"Nominal file does not exist!"<<std::endl; return 0;}

    // Check what systematics exist
    std::vector<int> syst_index_vector;
    for(int index = 0 ; index < total_syst ; index++)
    {
        std::string file_name = input_dir+broadening_file_template+syst[index]+"-a"+extension;
        if(!gSystem->AccessPathName(file_name.c_str()))
        {
            // Store the index of the existing systematic
            std::cout<<syst[index]<<" systematic has been found. Adding to analysis."<<std::endl;
            syst_index_vector.push_back(index);
        }
    }

    // Store the number systematic sources found
    const int syst_index_size = syst_index_vector.size();
    if(syst_index_size==0){std::cout<<"There are no sources of systematic errors! Exiting."<<std::endl; return 0;}

    // Create output file
    TFile* fout = new TFile(broadening_syst_results_zh.c_str(),"RECREATE");
    gROOT->cd();

    // Declare the array of systematics files
    TFile* fin_syst[syst_index_size][syst_variations];
    for(int index = 0 ; index < syst_index_size ; index++)
    {
        // Declare names so ROOT can open the files
        std::string file_name_a = input_dir+broadening_file_template+syst[syst_index_vector[index]]+"-a"+extension;
        std::string file_name_b = input_dir+broadening_file_template+syst[syst_index_vector[index]]+"-b"+extension;

        // Open systematics files
        fin_syst[index][0] = new TFile(file_name_a.c_str());
        fin_syst[index][1] = new TFile(file_name_b.c_str());
    }

    // Nominal histo
    TH1F* h_broadening_nominal[N_broadening];
    // Systematic histos
    TH1F* h_broadening_syst[syst_index_size][syst_variations][N_broadening];
    // Deviation histos
    TH1F* h_broadening_devs[syst_index_size][syst_variations][N_broadening];
    // Syst Error histos
    TH1F* h_broadening_systerr[syst_index_size][N_broadening];

    // WARNING : CHANGE THESE NAMES ON WHEN YOU FINALLY TRANSLATE TO THIS FRAMEWORK!!!!!
    // Assign nominal histos
    for(int targ_index = 0 ; targ_index < N_broadening ; targ_index++)
    {
        h_broadening_nominal[targ_index] = (TH1F*)fin->Get(("broadening_Zh_"+broadening_targets[targ_index]).c_str());
    }
    
    // Assign the constructor to the rest of histos
    for(int systsource_index = 0 ; systsource_index < syst_index_size ; systsource_index++)
    {
        for(int targ_index = 0 ; targ_index < N_broadening ; targ_index++)
        {
            h_broadening_systerr[systsource_index][targ_index] = new TH1F("","",N_Zh,Zh_limits);
            for(int systvar_index = 0 ; systvar_index < syst_variations ; systvar_index++)
            {
                h_broadening_syst[systsource_index][systvar_index][targ_index] = (TH1F*)fin_syst[systsource_index][systvar_index]->Get(("broadening_Zh_"+broadening_targets[targ_index]).c_str());
                h_broadening_devs[systsource_index][systvar_index][targ_index] = new TH1F("","",N_Zh,Zh_limits);
            }
        }
    }

    // Loop through systematics and the respective variations
    for(int targ_index = 0 ; targ_index < N_broadening ; targ_index++)
    {
        for(int systsource_index = 0 ; systsource_index < syst_index_size ; systsource_index++)
        {
            for(int systvar_index = 0 ; systvar_index < syst_variations ; systvar_index++)
            {
                // Calculate deviations and set them in a histogram
                set_deviation_histo(h_broadening_devs[systsource_index][systvar_index][targ_index], h_broadening_nominal[targ_index], h_broadening_syst[systsource_index][systvar_index][targ_index]);
                
                // Store
                fout->cd();
                h_broadening_devs[systsource_index][systvar_index][targ_index]->Write(("dev_broadening_Zh_"+syst[systsource_index]+"_"+std::to_string(systvar_index)+"_"+broadening_targets[targ_index]).c_str());
                gROOT->cd();
            }

            // Calculate the systematic errors and set them in a histogram
            set_systerr_histo(h_broadening_systerr[systsource_index][targ_index], h_broadening_devs[systsource_index][0][targ_index], h_broadening_devs[systsource_index][1][targ_index]);
            
            // Store
            fout->cd();
            h_broadening_systerr[systsource_index][targ_index]->Write(("systerr_broadening_Zh_"+syst[systsource_index]+"_"+broadening_targets[targ_index]).c_str());
            gROOT->cd();
        }
    }

    fout->Close();
    delete fout;

    return 0;
}
