#include <iostream>
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "constants.h"
#include "plots.h"

int main(int argc, char *argv[])
{
    // Visual misc
    TStyle* style = new TStyle("my","my");
    set_style_properties(style);
    gROOT->SetStyle("my");

    // Open files
    TFile* fin = new TFile("../output-files/results-broadening-systerr.root");
    if(fin==NULL){std::cout<<"No input file!"<<std::endl; return 1;}

    // Check what systematics exist
    std::vector<int> syst_index;
    for(int index = 0 ; index < total_syst ; index++)
    {
        std::string file_name = input_dir+broadening_file_template+syst[index]+"-a"+extension;
        if(!gSystem->AccessPathName(file_name.c_str()))
        {
            // Store the index of the existing systematic
            std::cout<<syst[index]<<" systematic has been found. Adding to analysis."<<std::endl;
            syst_index.push_back(index);
        }
    }
    
    // Store the number systematic sources found
    const int syst_index_size = syst_index.size();
    
    // Create a canvas
    TCanvas* c = new TCanvas("c","",1400,500);

    // Declare histos
    TH1F* h[syst_index_size][N_broadening][syst_variations];
    
    // Declare graphs
    TGraphErrors* g[syst_index_size][N_broadening][syst_variations];
    
    // Obtain histos and graphs
    for(int syst_index = 0 ; syst_index < syst_index_size ; syst_index++)
    {
        for(int targ = 0 ; targ < N_broadening ; targ++)
        {
            for(int var = 0 ; var < syst_variations ; var++)
            {
                // TODO : Soft code the syst error source
                std::string dev_plot_name = "dev_broadening_Zh_" + syst[syst_index] + "_" + std::to_string(var) + "_" + broadening_targets[targ];
                h[targ][var] = (TH1F*) fin->Get(dev_plot_name.c_str());
                g[targ][var] = new TGraphErrors(N_Zh);

                //Scale deviation histogram to show it in percentage
                h[targ][var]->Scale(100.);

                // Get the TGraph from the histo
                th1f_to_tgrapherrors(h[targ][var] ,g[targ][var]);

                // Customize graph
                g[targ][var]->SetMarkerStyle(targ_marker[var]);
                g[targ][var]->SetMarkerColor(targ_colors[targ]);
                set_xerr_null(g[targ][var]);
                set_yerr_null(g[targ][var]);
            }
        }
    }
    // Set a TMultiGraph    
    TMultiGraph* mg[N_broadening];
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {   
        mg[targ] = new TMultiGraph();

        mg[targ]->Add(g[targ][0], "P");
        mg[targ]->Add(g[targ][1], "P");

        // Add the graph containing the nominal statistical error
        // HERE
    }

    // Draw the TCanvas
    c->Draw();
    c->cd();

    // Set TPads and plots
    // NOTE : Initially, I desgined a function to do what is inside the loop; however, it seems ROOT likes to define locally the properties of a Pad.
    TPad* p[N_broadening];
    double delta_pad = 1./N_broadening;
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Set the constructor
        p[targ] = new TPad(Form("p%i",targ),Form("p%i",targ), delta_pad*targ, 0, delta_pad*(targ+1), 1);
    
        // Set attributes of the pad
        set_dev_pad_attributes(p[targ], targ);
        
        // Draw the pads
        p[targ]->Draw();
        p[targ]->cd();

        // Draw the multigraph on the pad and set its properties
        mg[targ]->Draw("A");
        set_dev_multigraph_properties(mg[targ]);

        // Return to TCanvas
        c->cd();
    }

    // Set Legends
    TLegend* legend[N_broadening];
    for(int targ = 0 ; targ < N_broadening ; targ++)
    {
        // Define the TLegend
        legend[targ] = new TLegend(dev_legend_X1[targ], dev_legend_Y1[targ], dev_legend_X2[targ], dev_legend_Y2[targ] ,"","NDC");
        set_dev_legend_properties(legend[targ]);

        // Define the text for the TLegend
        std::string legend_text_0 = broadening_targets[targ] + syst_legend[0][0];
        std::string legend_text_1 = broadening_targets[targ] + syst_legend[0][1];

        legend[targ]->AddEntry(g[targ][0], legend_text_0.c_str(),"p");
        legend[targ]->AddEntry(g[targ][1], legend_text_1.c_str(),"p");

        // Draw the TLegend
        p[targ]->cd();
        legend[targ]->Draw();
    }
    
    // Print the plots
    c->Print("../output-plots/test-plot.pdf");

    // Close file
    fin->Close();

    // Free memory
    delete fin;

    std::cout<<"Finished printing plots"<<std::endl;

    return 0;
}