#include <iostream>
#include <fstream>
#include "constants.h"

void print_hline(std::ofstream& file)
{
    file<<"\\hline"<<std::endl; return;
}

void print_a13_systerr_header(std::ofstream& file)
{
    file<<"\\begin{table}[H]\\centering\\begin{tabular}{|c|c|c|c|}"<<std::endl;
    print_hline(file);
    file<<"Syst. & \\Delta P^2_{{T}_C} & \\Delta P^2_{{T}_Fe} & \\Delta P^2_{{T}_Pb} \\\\"<<std::endl;
    print_hline(file);
}

void print_a13_systerr(std::ofstream& file, TH1F* hC, TH1F* hFe, TH1F* hPb, std::string syst)
{
    file.precision(3);
    file<<syst<<" & "<<100.*hC->GetBinContent(1)<<" & "<<100.*hFe->GetBinContent(1)<<" & "<<100.*hPb->GetBinContent(1)<<"\\\\"<<std::endl;
    return;
}

void print_a13_systerr_end(std::ofstream& file)
{
    print_hline(file);
    file<<"\\end{tabular}\\caption{Systematic errors for $\\Delta P^2_T$ without $x_f$ cut.}\\label{tab:Syst_Broad}\\end{table}"<<std::endl;
    return;
}

void print_zh_results_header(std::ofstream& file)
{
    file<<"\\begin{longtable}{c|c c c c c c c c c}"<<std::endl;
    print_hline(file);
    file<<"$z_h$&$\\Delta P^2_{T_C}$ &$\\pm\\text{Stat.}$&$\\pm\\text{Syst.}$&$\\Delta P^2_{T_{Fe}}$&$\\pm\\text{Stat.}$&$\\pm\\text{Syst.}$&$\\Delta P^2_{T_{Pb}}$&$\\pm\\text{Stat.}$&$\\pm\\text{Syst.}$\\\\"<<std::endl;
    print_hline(file);
}

void print_zh_results(std::ofstream& file, TH1F* hC, TH1F* hFe, TH1F* hPb, TGraphErrors* gC_totalerr, TGraphErrors* gFe_totalerr, TGraphErrors* gPb_totalerr)
{
    // hX : Broadening histos with statistical errors
    // gX_totalerr : Broadening TGraphErrors with total errors

    // Obtain the total errors
    double* totalerrC  = gC_totalerr->GetEY();
    double* totalerrFe = gFe_totalerr->GetEY();
    double* totalerrPb = gPb_totalerr->GetEY();

    for(int Zh_bin = Zh_cutoff+1 ; Zh_bin <= hC->GetNbinsX() ; Zh_bin++)
    {
        int h_index = Zh_bin;
        int g_index = Zh_bin-1;

        // Obtain statistical errors
        double stat_err_C  = hC->GetBinError(h_index);
        double stat_err_Fe = hFe->GetBinError(h_index);
        double stat_err_Pb = hPb->GetBinError(h_index);

        // Obtain systematic errors
        double syst_err_C  = sqrt(totalerrC[g_index]*totalerrC[g_index]   - stat_err_C*stat_err_C  );
        double syst_err_Fe = sqrt(totalerrFe[g_index]*totalerrFe[g_index] - stat_err_Fe*stat_err_Fe);
        double syst_err_Pb = sqrt(totalerrPb[g_index]*totalerrPb[g_index] - stat_err_Pb*stat_err_Pb);

        // Print the row
        file.precision(1);
        file<<std::fixed<<Zh_limits[Zh_bin-1]<<"-"<<Zh_limits[Zh_bin]<<" & ";

        file.precision(2);    
        file<<std::scientific<<hC->GetBinContent(h_index)<<" & "<<stat_err_C<<" & "<<syst_err_C<<" & "
                                                <<hFe->GetBinContent(h_index)<<" & "<<stat_err_Fe<<" & "<<syst_err_Fe<<" & "
                                                <<hPb->GetBinContent(h_index)<<" & "<<stat_err_Pb<<" & "<<syst_err_Pb<<"\\\\"<<std::endl;
    }

    return;
}

void print_q2nu_bin(std::ofstream& file, int Q2_bin, int Nu_bin)
{
    file.precision(2);
    file<<std::fixed<<"\\multicolumn{10}{c}{$"<<Q2_limits[Q2_bin]<<"<Q^{2}[GeV^{2}]<"<<Q2_limits[Q2_bin+1]
                    <<"$ ; $"<<Nu_limits[Nu_bin]<<"<\\nu[GeV]<"<<Nu_limits[Nu_bin+1]<<"$}\\\\"<<std::endl;
    print_hline(file);

    return;
}

void print_zh_results_end(std::ofstream& file)
{
    print_hline(file);
    file<<"\\caption{Transverse momentum broadening results, in $[GeV^2]$, as a function of $z_h$.}\\end{longtable}"<<std::endl;
 
    return;
}

void print_q2nuzh_results_end(std::ofstream& file)
{
    print_hline(file);
    file<<"\\caption{Transverse momentum broadening results, in $[GeV^2]$, as a function of $Q^2$,$\\nu$, and $z_h$.}\\end{longtable}"<<std::endl;
 
    return;
}
