#include "TH1F.h"

// Returns 1 if the histogram is empty.
int empty_histo(TH1F* h)
{
    int empty  = 0;
    int result = 0;
    for(Int_t i = 1 ; i <= h->GetNbinsX() ; i++)
    {
        if(h->GetBinContent(i)==0) empty++;
    }
    result = (empty==h->GetNbinsX()) ? 1 : 0;
    return result;
}

// Scans histo and returns the first empty bin it founds. If there are no empty bins a -1 value is returned.
int first_empty_bin(TH1F* h)
{
    for(Int_t bin = 1 ; bin <= h->GetNbinsX() ; bin++)
    {
        if(h->GetBinContent(bin)==0)
        {
            return bin;
        }
    }

    return -1;
}

// Sets the deviation-from-nominal histogram
// Convention : 100% is 1
void set_deviation_histo(TH1F* h_deviation, TH1F* h_nominal, TH1F* h_variation)
{
    h_deviation->Add(h_nominal,h_variation,1,-1);
    h_deviation->Divide(h_nominal);

    return;
}

// Sets a histogram with the systematic error values
// Convention : 100% is 1
void set_systerr_histo(TH1F* h_syst, TH1F* h_variation_a, TH1F* h_variation_b)
{
    for(int bin = 1 ; bin <= h_syst->GetNbinsX() ; bin++)
    {
        double dev_a = h_variation_a->GetBinContent(bin);
        double dev_b = h_variation_b->GetBinContent(bin);
    
        // Select the maximum deviation
        double max_dev = TMath::Max(TMath::Abs(dev_a),TMath::Abs(dev_b));
    
        // Calculate the syst error and assign to the histos
        h_syst->SetBinContent(bin, max_dev/TMath::Sqrt(3.));
    }

    return;
}
