#include <iostream>
#include <string>
#include "TSystem.h"
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
    TFile* fin         = new TFile("../output-files/results-meanpt2-systerr.root");
    TFile* fin_nominal = new TFile("../input-files/results-meanpt2-nominal.root");
    if(fin==NULL){std::cout<<"No input file!"<<std::endl; return 1;}

    // Check what systematics exist
    // The vector stores the index of the systematic source that was found
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
    const int syst_index_vector_size = syst_index_vector.size();
    std::cout<<syst_index_vector_size<<std::endl;
    // Create a canvas
    TCanvas* c = new TCanvas("c","",1400,500);

    // Declare histos
    TH1F* h[syst_index_vector_size][N_targets][syst_variations];
    
    // Declare graphs
    TGraphErrors* g[syst_index_vector_size][N_targets][syst_variations];
    
    // Obtain histos and graphs
    for(int syst_index = 0 ; syst_index < syst_index_vector_size ; syst_index++)
    {
        for(int targ = 0 ; targ < N_targets ; targ++)
        {
            for(int var = 0 ; var < syst_variations ; var++)
            {
                // TODO : Soft code the syst error source
                std::string dev_plot_name = "dev_meanPt2_Zh_" + syst[syst_index_vector[syst_index]] + "_" + std::to_string(var) + "_" + targets[targ];
                h[syst_index][targ][var] = (TH1F*) fin->Get(dev_plot_name.c_str());
                g[syst_index][targ][var] = new TGraphErrors(N_Zh);

                //Scale deviation histogram to show it in percentage
                h[syst_index][targ][var]->Scale(100.);

                // Get the TGraph from the histo
                th1f_to_tgrapherrors(h[syst_index][targ][var] ,g[syst_index][targ][var]);

                // Customize graph
                g[syst_index][targ][var]->SetMarkerStyle(targ_marker[var]);
                g[syst_index][targ][var]->SetMarkerColor(targ_colors[targ]);
                set_xerr_null(g[syst_index][targ][var]);
                set_yerr_null(g[syst_index][targ][var]);
            }
        }
    }

    // Obtain nominal statistical error histo
    TH1F* h_nominal[N_targets];
    TGraphErrors* g_nominal[N_targets];
    TGraphErrors* g_nominal_staterr[N_targets];

    for(int targ = 0 ; targ < N_targets ; targ++)
    {
        std::string nom_plot_name = "meanPt2_Zh_" + targets[targ];
        h_nominal[targ]         = (TH1F*) fin_nominal->Get(nom_plot_name.c_str());
        g_nominal[targ]         = new TGraphErrors(N_Zh);
        g_nominal_staterr[targ] = new TGraphErrors(N_Zh);

        // Get the TGraph from the histo
        th1f_to_tgrapherrors(h_nominal[targ] ,g_nominal[targ]);

        // Set nominal statistical error as content of new TGraph
        get_staterr_graph(g_nominal[targ], g_nominal_staterr[targ]);
    }
    
    // Set color of the stat err graphs (sorry for not being classy enough)
    g_nominal_staterr[0]->SetFillColor(kRed-9);
    g_nominal_staterr[1]->SetFillColor(kBlue-9);
    g_nominal_staterr[2]->SetFillColor(kGray);
    g_nominal_staterr[3]->SetFillColor(kRed-9);
    g_nominal_staterr[4]->SetFillColor(kBlue-9);
    g_nominal_staterr[5]->SetFillColor(kGray);

    // Set a TMultiGraph    
    TMultiGraph* mg[syst_index_vector_size][N_targets];
    for(int syst_index = 0 ; syst_index < syst_index_vector_size ; syst_index++)
    {
        for(int targ = 0 ; targ < N_targets ; targ++)
        {   
            // Call the constructor of the multigraph
            mg[syst_index][targ] = new TMultiGraph();

            // Add the graph containing the nominal statistical error
            mg[syst_index][targ]->Add(g_nominal_staterr[targ], "E3");
            
            // Add the necessary graphs
            mg[syst_index][targ]->Add(g[syst_index][targ][0], "P");
            mg[syst_index][targ]->Add(g[syst_index][targ][1], "P");
        }
    }


    // Draw the TCanvas
    c->Draw();

    // Set TPads, TLegends and plots
    // NOTE : Initially, I desgined a function to do what is inside the loop; however, it seems ROOT likes to define locally the properties of a Pad.
    TPad* p[syst_index_vector_size][N_targets];
    TLegend* legend[syst_index_vector_size][N_targets];

    double dev_pad_limits[2][4] = { {0 , 1./3. , 2./3. , 1.} , {0 , 1./3. , 2./3. , 1.} };
    
    for(int syst_index = 0 ; syst_index < syst_index_vector_size ; syst_index++)
    {
        int solid = 0;
        for(int targ = 0 ; targ < N_targets ; targ++)
        {
            if(targ==3){solid++;}

            // Set the constructor
            p[syst_index][targ] = new TPad(Form("p%i%i",syst_index,targ),Form("p%i%i",syst_index,targ), dev_pad_limits[solid][targ - 3*solid], 0, dev_pad_limits[solid][targ - 3*solid + 1], 1);

            // Set attributes of the pad
            set_dev_pad_attributes(p[syst_index][targ], targ);

            // Select the TCanvas
            c->cd();

            // Draw the pads
            p[syst_index][targ]->Draw();
            p[syst_index][targ]->cd();

            // Draw the multigraph on the pad and set its properties
            mg[syst_index][targ]->Draw("A");
            set_dev_multigraph_properties(mg[syst_index][targ]);

            // Define the TLegend
            std::cout<<"Position = "<<(targ - 3*solid)<<std::endl;
            legend[syst_index][targ] = new TLegend(dev_legend_X1[targ - 3*solid], dev_legend_Y1[targ - 3*solid], dev_legend_X2[targ - 3*solid], dev_legend_Y2[targ - 3*solid] ,"","NDC");
            set_dev_legend_properties(legend[syst_index][targ]);

            // Define the text for the TLegend
            std::string legend_text_nom = targets[targ] + " Nominal Stat. Error";
            std::string legend_text_0   = targets[targ] + syst_legend[syst_index_vector[syst_index]][0];
            std::string legend_text_1   = targets[targ] + syst_legend[syst_index_vector[syst_index]][1];

            
            legend[syst_index][targ]->AddEntry(g_nominal_staterr[targ],legend_text_nom.c_str(),"f");    
            legend[syst_index][targ]->AddEntry(g[syst_index][targ][0], legend_text_0.c_str(),"p");
            legend[syst_index][targ]->AddEntry(g[syst_index][targ][1], legend_text_1.c_str(),"p");

            // Draw the TLegend
            legend[syst_index][targ]->Draw();

            // Print liquid targets plots
            if(targ==2)
            {
                std::string plot_name = "../output-plots/dev_meanpt2_Zh_" + syst[syst_index_vector[syst_index]] + "_liquid.pdf";
                c->Print(plot_name.c_str());
                std::cout<<"Liquid graphs OK"<<std::endl;
            }
        }

        // Print solid targets plots
        std::string plot_name = "../output-plots/dev_meanpt2_Zh_" + syst[syst_index_vector[syst_index]] + "_solid.pdf";
        c->Print(plot_name.c_str());
        std::cout<<"Solid graphs OK"<<std::endl;
    }
    
    std::cout<<"Plots OK"<<std::endl;
    // Close file
    fin->Close();

    // Free memory
    delete fin;

    std::cout<<"Finished printing dev. plots!"<<std::endl;

    return 0;
}