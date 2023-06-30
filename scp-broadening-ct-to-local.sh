#!/bin/bash

mother_dir=$(pwd)

account=emolina@ui.hpc.utfsm.cl
folder=/work/emolina/broadening-analysis

syst="syst-ct"
a="npt2_50"
b="npt2_70"

# Create folders
mkdir ${mother_dir}/${syst}
mkdir ${mother_dir}/${syst}/${a}
mkdir ${mother_dir}/${syst}/${b}

# Clone the repo
git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst}/${a}/ptbroadening-analysis
git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf
git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst}/${b}/ptbroadening-analysis
git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf

# Compile the repos
cd ${mother_dir}/${syst}/${a}/ptbroadening-analysis
git checkout -b closure-test origin/closure-test
make

cd ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf
git checkout -b closure-test origin/closure-test
make

cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis
git checkout -b closure-test origin/closure-test
make 

cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf
git checkout -b closure-test origin/closure-test
make

# Obtain broadening results
scp ${account}:${folder}/${syst}/${a}/closure-test/output-files/ct_pt2-distributions.root    ${mother_dir}/${syst}/${a}/ptbroadening-analysis/output-files/pt2-distributions-bgtreated.root
scp ${account}:${folder}/${syst}/${a}/closure-test-xf/output-files/ct_pt2-distributions.root ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf/output-files/pt2-distributions-bgtreated.root
scp ${account}:${folder}/${syst}/${b}/closure-test/output-files/ct_pt2-distributions.root    ${mother_dir}/${syst}/${b}/ptbroadening-analysis/output-files/pt2-distributions-bgtreated.root
scp ${account}:${folder}/${syst}/${b}/closure-test-xf/output-files/ct_pt2-distributions.root ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf/output-files/pt2-distributions-bgtreated.root

# Obtain plots just in case
cd ${mother_dir}/${syst}/${a}/ptbroadening-analysis/bin
./calculate-meanpt2
./calculate-broadening
./print-plot-broadening-q2nuzh
./print-plot-broadening-q2nuzh-compact
./print-plot-broadening-q2nuzha13
./print-plot-broadening-q2-nu-zh-a13

cd ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf/bin
./calculate-meanpt2
./calculate-broadening
./print-plot-broadening-q2nuzh
./print-plot-broadening-q2nuzh-compact
./print-plot-broadening-q2nuzha13
./print-plot-broadening-q2-nu-zh-a13

cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis/bin
./calculate-meanpt2
./calculate-broadening
./print-plot-broadening-q2nuzh
./print-plot-broadening-q2nuzh-compact
./print-plot-broadening-q2nuzha13
./print-plot-broadening-q2-nu-zh-a13

cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf/bin
./calculate-meanpt2
./calculate-broadening
./print-plot-broadening-q2nuzh
./print-plot-broadening-q2nuzh-compact
./print-plot-broadening-q2nuzha13
./print-plot-broadening-q2-nu-zh-a13
