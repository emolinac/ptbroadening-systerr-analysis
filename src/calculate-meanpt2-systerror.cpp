#include <iostream>
#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"
#include "constants.h"

int main(int argc, char* argv[])
{
    std::string syst[10] = {"absdeltaz", "accminlim", "ct", "naccept", "npt2", "rad", "sectorpi", "bgreduction", "tof", "vc"};

    // Create output file
    TFile* fout = new TFile();

    // Open file that contains nominal results
    TFile* fin = new TFile("../input-files/results-meanpt2-nominal.root");
    if(fin==NULL){std::cout<<"Nominal file does not exist!"<<std::endl; return 0;}

    // Check what systematics exist
    std::vector<int> syst_index;
    for(int index = 0 ; index < total_syst ; index++)
    {
        std::string file_name = input_dir+meanpt2_file_template+syst[index]+"-a"+extension;
        if(!gSystem->AccessPathName(file_name.c_str()))
        {
            // Store the index of the existing systematic
            std::cout<<syst[index]<<" systematic has been found. Adding to analysis."<<std::endl;
            syst_index.push_back(index);
        }
    }

    // Declarate the array of systematics files
    TFile* fin_syst[syst_index.size()][syst_variations];
    for(int index = 0 ; index < syst_index.size() ; index++)
    {
        // Declare names so ROOT can open the files
        std::string file_name_a = input_dir+meanpt2_file_template+syst[syst_index[index]]+"-a"+extension;
        std::string file_name_b = input_dir+meanpt2_file_template+syst[syst_index[index]]+"-b"+extension;

        // Open systematics files
        fin_syst[index][0] = new TFile(file_name_a.c_str());
        fin_syst[index][1] = new TFile(file_name_b.c_str());
    }

    // Nominal histo
    TH1F* h_meanpt2_nominal[N_targets];
    // Systematic histos
    TH1F* h_meanpt2_syst[syst_index.size()][syst_variations][N_targets];
    // Deviation histos
    TH1F* h_meanpt2_devs[syst_index.size()][syst_variations][N_targets];
    // Syst Error histos
    TH1F* h_meanpt2_systerr[syst_index.size()][N_targets];

    // WARNING : CHANGE THESE NAMES ON WHEN YOU FINALLY TRANSLATE TO THIS FRAMEWORK!!!!!
    // Assign nominal histos
    for(int targ_index = 0 ; targ_index < N_targets ; targ_index++)
    {
        h_meanpt2_nominal[targ_index] = (TH1F*)fin->Get("meanPt2_"+targets[targ_index]+"_CLEAN_INTERPOLATED");
    }
    // Assign the constructor to the rest of histos
    for(int systsource_index = 0 ; systsource_index < syst_index.size() ; systsource_index++)
    {
        for(int targ_index = 0 ; targ_index < N_targets ; targ_index++)
        {
            h_meanpt2_systerr[systsource_index][targ_index] = new TH1F("","",N_Zh,Zh_limits);
            for(int systvar_index = 0 ; systvar_index < syst_variations ; systvar_index++)
            {
                h_meanpt2_syst[systsource_index][systvar_index][targ_index] = (TH1F*)fin_syst[systsource_index][systvar_index]->Get("meanPt2_"+targets[targ_index]+"_CLEAN_INTERPOLATED");
                h_meanpt2_devs[systsource_index][systvar_index][targ_index] = new TH1F("","",N_Zh,Zh_limits);
            }
        }
    }

    // Loop through systematics and the respective variations
    for(int systsource_index = 0 ; systsource_index < syst_index.size() ; systsource_index++)
    {
        for(int systvar_index = 0 ; systvar_index < syst_variations ; systvar_index++)
        {
            for(int targ_index = 0 ; targ_index < N_targets ; targ_index++)
            {
                // Calculate deviations
                h_meanpt2_devs[systsource_index][systvar_index][targ_index]->Add(h_meanpt2_nominal[targ_index],h_meanpt2_syst[systsource_index][systvar_index][targ_index],1,-1);
                h_meanpt2_devs[systsource_index][systvar_index][targ_index]->Divide(h_meanpt2_nominal[targ_index]);

                // Store
                fout->cd();
                fout->Write();
                gROOT->cd();
            }
        }
    }

    // Subtract nominal from systematics and assign to deviation histos
        // Stor deviation histos
    
    // Loop through bins of zh
        // Select the highest deviation
        // Calculate the systematic error
            // Assign error to systematic error histogram
        // Add in quadrature to statistical errors
    // Store systematic error histo

    return 0;
}
