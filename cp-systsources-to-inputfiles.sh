#!/bin/bash

mother_dir=$(pwd)

syst_folder=("syst-acc" "syst-deltaz" "syst-naccept" "syst-pt2binning" "syst-tof" "syst-vertexcut" "syst-ct")
syst=("acc" "deltaz" "naccept" "pt2binning" "tof" "vertexcut" "ct")
a=("accg005" "deltaz-leq-25" "nacceptg0" "70bins"  "tof-momentuml25" "hh-vc" "npt2_50")
b=("accg01"  "deltaz-leq-35" "nacceptg2" "110bins" "tof-momentuml29" "rd-vc" "npt2_70")

for (( index=0 ; index <${#syst_folder[@]} ; index++))
do
    if [ -e "${mother_dir}/${syst_folder[${index}]}/${a[${index}]}/ptbroadening-analysis/output-files/results-broadening.root" ]
    then
        echo "-------------------------------------------------------------------------------"
        echo "${syst[${index}]} broadening results exist! Copying to input-files"
        cp ${mother_dir}/${syst_folder[${index}]}/${a[${index}]}/ptbroadening-analysis/output-files/results-broadening.root ${mother_dir}/input-files/results-broadening-${syst[${index}]}-a.root
        cp ${mother_dir}/${syst_folder[${index}]}/${b[${index}]}/ptbroadening-analysis/output-files/results-broadening.root ${mother_dir}/input-files/results-broadening-${syst[${index}]}-b.root
        echo "-------------------------------------------------------------------------------"
    else
        echo "${a[${index}]} do not exists!"
    fi
done
