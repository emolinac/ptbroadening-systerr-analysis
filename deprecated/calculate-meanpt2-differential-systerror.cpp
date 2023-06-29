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
    TFile* fin = new TFile(meanpt2_nominal_results.c_str());
    if(fin==NULL){std::cout<<"Nominal file does not exist!"<<std::endl; return 0;}

    // Check what systematics exist
    std::vector<int> syst_index_vector;
    for(int index = 0 ; index < total_syst ; index++)
    {
        std::string file_name = input_dir+meanpt2_file_template+syst[index]+"-a"+extension;
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
    TFile* fout = new TFile(meanpt2_syst_results_diff.c_str(),"RECREATE");
    gROOT->cd();

    // Declare the array of systematics files
    TFile* fin_syst[syst_index_size][syst_variations];
    for(int index = 0 ; index < syst_index_size ; index++)
    {
        // Declare names so ROOT can open the files
        std::string file_name_a = input_dir+meanpt2_file_template+syst[syst_index_vector[index]]+"-a"+extension;
        std::string file_name_b = input_dir+meanpt2_file_template+syst[syst_index_vector[index]]+"-b"+extension;

        // Open systematics files
        fin_syst[index][0] = new TFile(file_name_a.c_str());
        fin_syst[index][1] = new TFile(file_name_b.c_str());
    }

    // Nominal histo
    TH1F* h_meanpt2_nominal[N_Q2][N_Nu][N_targets];
    // Systematic histos
    TH1F* h_meanpt2_syst[syst_index_size][syst_variations][N_Q2][N_Nu][N_targets];
    // Deviation histos
    TH1F* h_meanpt2_devs[syst_index_size][syst_variations][N_Q2][N_Nu][N_targets];
    // Syst Error histos
    TH1F* h_meanpt2_systerr[syst_index_size][N_Q2][N_Nu][N_targets];

    // WARNING : CHANGE THESE NAMES ON WHEN YOU FINALLY TRANSLATE TO THIS FRAMEWORK!!!!!
    // Assign nominal histos
    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            for(int targ_index = 0 ; targ_index < N_targets ; targ_index++)
            {
                h_meanpt2_nominal[Q2_bin][Nu_bin][targ_index] = (TH1F*)fin->Get(("meanPt2_Zh_"+targets[targ_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)).c_str());
            }
        }
    }
        
    // Assign the constructor to the rest of histos
    for(int systsource_index = 0 ; systsource_index < syst_index_size ; systsource_index++)
    {
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                for(int targ_index = 0 ; targ_index < N_targets ; targ_index++)
                {
                    h_meanpt2_systerr[systsource_index][Q2_bin][Nu_bin][targ_index] = new TH1F("","",N_Zh,Zh_limits);
                    for(int systvar_index = 0 ; systvar_index < syst_variations ; systvar_index++)
                    {
                        h_meanpt2_syst[systsource_index][systvar_index][Q2_bin][Nu_bin][targ_index] = (TH1F*)fin_syst[systsource_index][systvar_index]->Get(("meanPt2_Zh_"+targets[targ_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)).c_str());
                        h_meanpt2_devs[systsource_index][systvar_index][Q2_bin][Nu_bin][targ_index] = new TH1F("","",N_Zh,Zh_limits);
                    }
                }
            }
        }
    }

    // Loop through systematics and the respective variations
    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            for(int targ_index = 0 ; targ_index < N_targets ; targ_index++)
            {
                for(int systsource_index = 0 ; systsource_index < syst_index_size ; systsource_index++)
                {
                    for(int systvar_index = 0 ; systvar_index < syst_variations ; systvar_index++)
                    {
                        // Calculate deviations and set them in a histogram
                        set_deviation_histo(h_meanpt2_devs[systsource_index][systvar_index][Q2_bin][Nu_bin][targ_index], h_meanpt2_nominal[Q2_bin][Nu_bin][targ_index], h_meanpt2_syst[systsource_index][systvar_index][Q2_bin][Nu_bin][targ_index]);
                        // Store
                        fout->cd();
                        h_meanpt2_devs[systsource_index][systvar_index][Q2_bin][Nu_bin][targ_index]->Write(("dev_meanPt2_Zh_"+syst[systsource_index]+"_"+std::to_string(systvar_index)+"_"+targets[targ_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)).c_str());
                        gROOT->cd();
                    }
                    // Calculate the systematic errors and set them in a histogram
                    set_systerr_histo(h_meanpt2_systerr[systsource_index][Q2_bin][Nu_bin][targ_index], h_meanpt2_devs[systsource_index][0][Q2_bin][Nu_bin][targ_index], h_meanpt2_devs[systsource_index][1][Q2_bin][Nu_bin][targ_index]);
                    // Store
                    fout->cd();
                    h_meanpt2_systerr[systsource_index][Q2_bin][Nu_bin][targ_index]->Write(("systerr_meanPt2_Zh_"+syst[systsource_index]+"_"+targets[targ_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)).c_str());
                    gROOT->cd();
                }
            }
        }
    }

    fout->Close();
    delete fout;

    return 0;
}
