#include <iostream>
#include <fstream>
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TSystem.h"
#include "constants.h"
#include "plots.h"
#include "utils.h"
#include "latex.h"

int main(int argc, char *argv[])
{
    // Visual misc
    TStyle* style = new TStyle("my","my");
    set_style_properties(style);
    gROOT->SetStyle("my");

    // Open file
    TFile* fin         = new TFile(broadening_syst_results_zh.c_str());
    TFile* fin_nominal = new TFile(broadening_nominal_results.c_str());
    if(fin==NULL){std::cout<<"No input file!"<<std::endl; return 1;}

    // Check what systematics exist
    // The vector stores the index of the systematic source that was found
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
    const int syst_index_vector_size = syst_index_vector.size();

    // Create a canvas
    TCanvas* c = new TCanvas("c","",800,600);

    // Declare arrays to contain histos and graphs
    TH1F*         h[N_broadening];
    TGraphErrors* g[N_broadening];

    TH1F*         h_syst[syst_index_vector_size][N_broadening];
    
    TGraphErrors* g_withsyst[N_broadening];

    // Obtain NOMINAL histos and graphs
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        std::string histo_name = "broadening_Zh_" + broadening_targets[targ];
        h[targ] = (TH1F*) fin_nominal->Get(histo_name.c_str());
        g[targ] = new TGraphErrors(N_Zh);
        g_withsyst[targ] = new TGraphErrors(N_Zh);
        th1f_to_tgrapherrors(h[targ], g[targ]);
        th1f_to_tgrapherrors(h[targ], g_withsyst[targ]);

        // Customize graphs
        g[targ]->SetMarkerStyle(targ_marker[targ + 3]);
        g[targ]->SetMarkerColor(targ_colors[targ]);
        g[targ]->SetLineColor(targ_colors[targ]);
        set_xerr_null(g[targ]);
        shift_x(g[targ], shift_x_zh[targ]);

        g_withsyst[targ]->SetMarkerStyle(targ_marker[targ + 3]);
        g_withsyst[targ]->SetMarkerColor(targ_colors[targ]);
        g_withsyst[targ]->SetLineColor(targ_colors[targ]);
        set_xerr_null(g_withsyst[targ]);
        shift_x(g_withsyst[targ], shift_x_zh[targ]);
    }
    
    // Obtain SYSTEMATICS histos and set the systematic errors on the results plots
    for(int syst_index = 0 ; syst_index < syst_index_vector_size ; syst_index++)
    {
        for(int targ = 0 ; targ < N_broadening ; targ++)
        {
            std::string systerr_plot_name = "systerr_broadening_Zh_" + syst[syst_index_vector[syst_index]] + "_" + broadening_targets[targ];
            std::cout<<"Obtaining "<<systerr_plot_name<<std::endl;
            h_syst[syst_index][targ] = (TH1F*) fin->Get(systerr_plot_name.c_str());
        
            // Set the systematic errors in the results plot
            set_syst_errors(h_syst[syst_index][targ], g_withsyst[targ]);
        }                
    }

    // Open txt file and print the values on it
    std::ofstream txt_file("../output-tables/syst-zh-table.txt");
    print_zh_results_header(txt_file);
    print_zh_results(txt_file,h[0],h[1],h[2],g_withsyst[0],g_withsyst[1],g_withsyst[2]);
    print_zh_results_end(txt_file);
    txt_file.close();

    // Set a 3x3 TMultiGraph array
    TMultiGraph* mg;

    mg = new TMultiGraph();
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        mg->Add(g[targ], "AP||");
        mg->Add(g_withsyst[targ], "APEZ0");
    }
    
    // Loop through the 3x3 TPad array
    TPad* p = new TPad("p","p",0,0,1,1);;
    set_pad_attributes(p);

    // Draw the TCanvas
    c->Draw();
    c->cd();

    // Draw the pad and the multigraph in it
    p->Draw();
    p->cd();
    
    mg->Draw("A");
    set_broadening_zh_multigraph_properties(mg);
    
    // Return to the canvas
    c->cd();
    
        // Print
    c->Print("../output-plots/broadening-zh.pdf");
    c->Print("../output-plots/broadening-zh.png");

    // Close file
    fin->Close();

    // Free memory
    delete fin;

    std::cout<<"Finished printing plots"<<std::endl;

    return 0;
}