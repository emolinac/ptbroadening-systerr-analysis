#include <iostream>
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
    TFile* fin         = new TFile(broadening_syst_results_diff.c_str());
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
    TCanvas* c = new TCanvas("c","",1300,500);

    // Declare arrays to contain histos and graphs
    TH1F*         h[N_Q2][N_Nu][N_broadening];
    TGraphErrors* g[N_Q2][N_Nu][N_broadening];

    TH1F*         h_syst[syst_index_vector_size][N_Q2][N_Nu][N_broadening];
    
    TGraphErrors* g_withsyst[N_Q2][N_Nu][N_broadening];

    // Obtain NOMINAL histos and graphs
    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            for(int targ = 0 ; targ < N_broadening ; targ++)
            {
                std::string histo_name = "broadening_Zh_" + broadening_targets[targ] + "_" + std::to_string(Q2_bin) + std::to_string(Nu_bin);
                h[Q2_bin][Nu_bin][targ] = (TH1F*) fin_nominal->Get(histo_name.c_str());

                g[Q2_bin][Nu_bin][targ] = new TGraphErrors(N_Zh);
                g_withsyst[Q2_bin][Nu_bin][targ] = new TGraphErrors(N_Zh);

                th1f_to_tgrapherrors(h[Q2_bin][Nu_bin][targ], g[Q2_bin][Nu_bin][targ]);
                th1f_to_tgrapherrors(h[Q2_bin][Nu_bin][targ], g_withsyst[Q2_bin][Nu_bin][targ]);

                // Customize graphs
                g[Q2_bin][Nu_bin][targ]->SetMarkerStyle(targ_marker_ndim[Q2_bin][targ]);
                g[Q2_bin][Nu_bin][targ]->SetMarkerColor(targ_colors[targ]);
                g[Q2_bin][Nu_bin][targ]->SetLineColor(targ_colors[targ]);
                set_xerr_null(g[Q2_bin][Nu_bin][targ]);
                
                g_withsyst[Q2_bin][Nu_bin][targ]->SetMarkerStyle(targ_marker_ndim[Q2_bin][targ]);
                g_withsyst[Q2_bin][Nu_bin][targ]->SetMarkerColor(targ_colors[targ]);
                g_withsyst[Q2_bin][Nu_bin][targ]->SetLineColor(targ_colors[targ]);
                set_xerr_null(g_withsyst[Q2_bin][Nu_bin][targ]);

                // Slightly shift according to target
                shift_x(g[Q2_bin][Nu_bin][targ], shift_x_zh_big[targ]);
                shift_x(g_withsyst[Q2_bin][Nu_bin][targ], shift_x_zh_big[targ]);

                //Slightly shift according to Nu bin
                shift_x(g[Q2_bin][Nu_bin][targ], shift_x_zh_compact[Q2_bin]);
                shift_x(g_withsyst[Q2_bin][Nu_bin][targ], shift_x_zh_compact[Q2_bin]);
            }
        }
    }

    // Obtain SYSTEMATICS histos and set the systematic errors on the results plots
    for(int syst_index = 0 ; syst_index < syst_index_vector_size ; syst_index++)
    {
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
            {
                for(int targ = 0 ; targ < N_broadening ; targ++)
                {
                    std::string systerr_plot_name = "systerr_broadening_Zh_" + syst[syst_index_vector[syst_index]] + "_" + broadening_targets[targ] + "_" + std::to_string(Q2_bin) + std::to_string(Nu_bin);
                    h_syst[syst_index][Q2_bin][Nu_bin][targ] = (TH1F*) fin->Get(systerr_plot_name.c_str());

                    // Set the systematic errors in the results plot
                    set_syst_errors(h_syst[syst_index][Q2_bin][Nu_bin][targ], g_withsyst[Q2_bin][Nu_bin][targ]);
                }                
            }
        }
    }

    // Open txt file and print the values on it
    std::ofstream txt_file("../output-tables/syst-q2nuzh-table.txt");
    print_zh_results_header(txt_file);
    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            print_q2nu_bin(txt_file, Q2_bin, Nu_bin);
            print_zh_results(txt_file,h[Q2_bin][Nu_bin][0],h[Q2_bin][Nu_bin][1],h[Q2_bin][Nu_bin][2],
                             g_withsyst[Q2_bin][Nu_bin][0],g_withsyst[Q2_bin][Nu_bin][1],g_withsyst[Q2_bin][Nu_bin][2]);
            print_hline(txt_file);
        }
    }
    print_q2nuzh_results_end(txt_file);
    txt_file.close();

    // Set a 3x3 TMultiGraph array
    TMultiGraph* mg[N_Nu];
    for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
    {
        mg[Nu_bin] = new TMultiGraph();
        for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
        {
            for(int targ = 0 ; targ < N_broadening ; targ++)
            {
                mg[Nu_bin]->Add(g[Q2_bin][Nu_bin][targ], "AP||");
                mg[Nu_bin]->Add(g_withsyst[Q2_bin][Nu_bin][targ], "APEZ0");
            }
        }
    }

    // Loop through the 3x3 TPad array
    TPad* p[N_Nu];
    for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
    {
        p[Nu_bin] = new TPad(Form("p%i",Nu_bin),Form("p%i",Nu_bin),pad_X1_compact[Nu_bin],pad_Y1_compact[Nu_bin],pad_X2_compact[Nu_bin],pad_Y2_compact[Nu_bin]);
        set_pad_attributes(p[Nu_bin], Nu_bin);
    }

    // Draw the TCanvas
    c->Draw();
    c->cd();

    // Draw the pads and the multigraphs in it
    for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
    {
        p[Nu_bin]->Draw();
        p[Nu_bin]->cd();

        mg[Nu_bin]->Draw("A");

        set_broadening_q2nuzh_multigraph_properties(mg[Nu_bin]);

        // Return to the canvas
        c->cd();
    }

    // Draw Q2 and Nu kinematical limits
    TLatex* t_nu = new TLatex();
    
    set_latex_properties(t_nu);

    set_nu_limits_pads(p[0],p[1],p[2],t_nu);

    // Draw TLegend wth target info
    TLegend* target_legend_1 = new TLegend(p[0]->GetLeftMargin()     ,1-(p[0]->GetTopMargin()+0.14),p[0]->GetLeftMargin()+0.28,1-p[0]->GetTopMargin(),"","NDC");
    TLegend* target_legend_2 = new TLegend(p[0]->GetLeftMargin()+0.28,1-(p[0]->GetTopMargin()+0.14),p[0]->GetLeftMargin()+0.56,1-p[0]->GetTopMargin(),"","NDC");  
    TLegend* target_legend_3 = new TLegend(p[0]->GetLeftMargin()+0.56,1-(p[0]->GetTopMargin()+0.14),p[0]->GetLeftMargin()+0.84,1-p[0]->GetTopMargin(),"","NDC");
    target_legend_1->SetFillStyle(0);
    target_legend_1->SetBorderSize(0);
    target_legend_2->SetFillStyle(0);
    target_legend_2->SetBorderSize(0);
    target_legend_3->SetFillStyle(0);
    target_legend_3->SetBorderSize(0);
    
    target_legend_1->AddEntry(g[0][0][0],"C 1.0<Q^{2}(GeV^{2})<1.3", "p");
    target_legend_1->AddEntry(g[1][0][0],"C 1.3<Q^{2}(GeV^{2})<1.8", "p");
    target_legend_1->AddEntry(g[2][0][0],"C 1.8<Q^{2}(GeV^{2})<4.0", "p");  
    target_legend_2->AddEntry(g[0][0][1],"Fe 1.0<Q^{2}(GeV^{2})<1.3","p");
    target_legend_2->AddEntry(g[1][0][1],"Fe 1.3<Q^{2}(GeV^{2})<1.8","p");
    target_legend_2->AddEntry(g[2][0][1],"Fe 1.8<Q^{2}(GeV^{2})<4.0","p");
    target_legend_3->AddEntry(g[0][0][2],"Pb 1.0<Q^{2}(GeV^{2})<1.3","p");
    target_legend_3->AddEntry(g[1][0][2],"Pb 1.3<Q^{2}(GeV^{2})<1.8","p");
    target_legend_3->AddEntry(g[2][0][2],"Pb 1.8<Q^{2}(GeV^{2})<4.0","p");
    p[0]->cd();
    target_legend_1->Draw("SAME");
    target_legend_2->Draw("SAME");
    target_legend_3->Draw("SAME");

    // Print
    c->Print("../output-plots/broadening-q2nuzh-compact.pdf");
    c->Print("../output-plots/broadening-q2nuzh-compact.png");

    // Close file
    fin->Close();

    // Free memory
    delete fin;

    std::cout<<"Finished printing plots"<<std::endl;

    return 0;
}