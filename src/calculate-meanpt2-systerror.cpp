#include <iostream>
#include "TSystem.h"
#include "TFile.h"
#include "constants.h"

int main(int argc, char* argv[])
{
    const char* syst[10] = {"absdeltaz", "accminlim", "ct", "naccept", "npt2", "rad", "sectorpi", "bgreduction", "tof", "vc"};

    // Open file that contains nominal results
    TFile* fin = new TFile("../input-files/results-meanpt2-nominal.root");
    if(fin==NULL){std::cout<<"Nominal file does not exist!"<<std::endl; return 0;}

    // Check what systematics exist
    std::vector<int> syst_index;
    for(int index = 0 ; index < total_syst ; index++)
    {      
        std::cout<<syst[index]<<" searching..."<<std::endl; 
        if(!gSystem->AccessPathName((const char*)(*meanpt2_file_template+syst[index]+'a'+*extension)))
        {
            std::cout<<syst[index]<<" systematic has been found. Adding to analysis."<<std::endl;
            syst_index.push_back(index);
            std::cout<<"Size of the vector is "<<syst_index.size()<<std::endl;
        }
    }
        // Open systematics files
    
    // Store the number of systematics present!

    // Create arrays of histos to be used : Nominal + N_syst*N_variations (histos from file), N_syst*N_variations (deviation histos), N_syst (syst error histo)

    // Assign nominal histo

    // Loop through systematics and the respective variations
        // Assign systematics histos

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
