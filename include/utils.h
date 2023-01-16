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
