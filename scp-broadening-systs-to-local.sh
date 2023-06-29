#!/bin/bash

mother_dir=$(pwd)

account=emolina@ui.hpc.utfsm.cl
folder=/work/emolina/broadening-analysis

syst=("syst-acc" "syst-deltaz" "syst-naccept" "syst-pt2binning" "syst-tof" "syst-vertexcut")
a=("accg005" "deltaz-leq-25" "nacceptg0" "110bins" "tof-momentuml25" "hh-vc")
b=("accg01"  "deltaz-leq-35" "nacceptg2" "70bins"  "tof-momentuml29" "rd-vc")

index=5

# Create folders
mkdir ${mother_dir}/${syst[index]}
mkdir ${mother_dir}/${syst[index]}/${a[${index}]}
mkdir ${mother_dir}/${syst[index]}/${b[${index}]}

# Clone the repo
git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst[index]}/${a[${index}]}/ptbroadening-analysis
git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst[index]}/${a[${index}]}/ptbroadening-analysis-xf
git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst[index]}/${b[${index}]}/ptbroadening-analysis
git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst[index]}/${b[${index}]}/ptbroadening-analysis-xf

# Compile the repos
cd ${mother_dir}/${syst[index]}/${a[${index}]}/ptbroadening-analysis
make

cd ${mother_dir}/${syst[index]}/${a[${index}]}/ptbroadening-analysis-xf
make

cd ${mother_dir}/${syst[index]}/${b[${index}]}/ptbroadening-analysis
make 

cd ${mother_dir}/${syst[index]}/${b[${index}]}/ptbroadening-analysis-xf
make

# Obtain broadening results
scp ${account}:${folder}/${syst[index]}/${a[${index}]}/ptbroadening-analysis/output-files/results-broadening.root    ${mother_dir}/${syst[index]}/${a[${index}]}/ptbroadening-analysis/output-files/
scp ${account}:${folder}/${syst[index]}/${a[${index}]}/ptbroadening-analysis-xf/output-files/results-broadening.root ${mother_dir}/${syst[index]}/${a[${index}]}/ptbroadening-analysis-xf/output-files/
scp ${account}:${folder}/${syst[index]}/${b[${index}]}/ptbroadening-analysis/output-files/results-broadening.root    ${mother_dir}/${syst[index]}/${b[${index}]}/ptbroadening-analysis/output-files/
scp ${account}:${folder}/${syst[index]}/${b[${index}]}/ptbroadening-analysis-xf/output-files/results-broadening.root ${mother_dir}/${syst[index]}/${b[${index}]}/ptbroadening-analysis-xf/output-files/

# Obtain plots just in case
cd ${mother_dir}/${syst[index]}/${a[${index}]}/ptbroadening-analysis/bin
./print-plot-broadening-q2nuzh
./print-plot-broadening-q2nuzh-compact
./print-plot-broadening-q2nuzha13
./print-plot-broadening-q2-nu-zh-a13

cd ${mother_dir}/${syst[index]}/${a[${index}]}/ptbroadening-analysis-xf/bin
./print-plot-broadening-q2nuzh
./print-plot-broadening-q2nuzh-compact
./print-plot-broadening-q2nuzha13
./print-plot-broadening-q2-nu-zh-a13

cd ${mother_dir}/${syst[index]}/${b[${index}]}/ptbroadening-analysis/bin
./print-plot-broadening-q2nuzh
./print-plot-broadening-q2nuzh-compact
./print-plot-broadening-q2nuzha13
./print-plot-broadening-q2-nu-zh-a13

cd ${mother_dir}/${syst[index]}/${b[${index}]}/ptbroadening-analysis-xf/bin
./print-plot-broadening-q2nuzh
./print-plot-broadening-q2nuzh-compact
./print-plot-broadening-q2nuzha13
./print-plot-broadening-q2-nu-zh-a13
