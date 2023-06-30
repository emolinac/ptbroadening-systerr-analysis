#!/bin/bash

mother_dir=$(pwd)

account=emolina@ui.hpc.utfsm.cl
folder=/work/emolina/broadening-analysis

syst="syst-rad"
a="rc-binomialerr"
b="rc-uncorrelatederr"

## Create folders
#mkdir ${mother_dir}/${syst}
#mkdir ${mother_dir}/${syst}/${a}
#mkdir ${mother_dir}/${syst}/${b}
#
## Clone the repo
#git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst}/${a}/ptbroadening-analysis
#git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf
#git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst}/${b}/ptbroadening-analysis
#git clone https://github.com/emolinac/ptbroadening-analysis.git ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf
#
## Get the correction branch and compile the repos
#cd ${mother_dir}/${syst}/${a}/ptbroadening-analysis
#git checkout -b corrections-analysis origin/corrections-analysis
#make
#
#cd ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf
#git checkout -b corrections-analysis origin/corrections-analysis
#make
#
#cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis
#git checkout -b corrections-analysis origin/corrections-analysis
#make 
#
#cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf
#git checkout -b corrections-analysis origin/corrections-analysis
#make
#
# Obtain broadening results
scp ${account}:${folder}/nominal/clas-hadron-corrections/output-files-rad/phi-distributions.root    ${mother_dir}/${syst}/${a}/ptbroadening-analysis/input-file/phi-distributions.root
scp ${account}:${folder}/nominal/clas-hadron-corrections-xf/output-files-rad/phi-distributions.root ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf/input-file/phi-distributions.root
scp ${account}:${folder}/nominal/clas-hadron-corrections/output-files-rad/phi-distributions.root    ${mother_dir}/${syst}/${b}/ptbroadening-analysis/input-file/phi-distributions.root
scp ${account}:${folder}/nominal/clas-hadron-corrections-xf/output-files-rad/phi-distributions.root ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf/input-file/phi-distributions.root

# Obtain plots just in case
cd ${mother_dir}/${syst}/${a}/ptbroadening-analysis/bin
./integrate-phi-q2nuzhpt2
./bgreduction-pt2-q2nuzh

cd ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf/bin
./integrate-phi-q2nuzhpt2
./bgreduction-pt2-q2nuzh

cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis/bin
./integrate-phi-q2nuzhpt2
./bgreduction-pt2-q2nuzh

cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf/bin
./integrate-phi-q2nuzhpt2
./bgreduction-pt2-q2nuzh
