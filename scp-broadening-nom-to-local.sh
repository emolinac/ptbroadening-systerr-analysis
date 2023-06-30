#!/bin/bash

mother_dir=$(pwd)

account=emolina@ui.hpc.utfsm.cl
folder=/work/emolina/broadening-analysis

syst="syst-bgreduction"
a="no-bgreduction"
b="only-cutoff"

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
#make
#
#cd ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf
#make
#
#cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis
#make 
#
#cd ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf
#make
#
# Obtain broadening results
scp ${account}:${folder}/nominal/ptbroadening-analysis/output-files/pt2-distributions.root    ${mother_dir}/${syst}/${a}/ptbroadening-analysis/output-files/pt2-distributions.root
scp ${account}:${folder}/nominal/ptbroadening-analysis-xf/output-files/pt2-distributions.root ${mother_dir}/${syst}/${a}/ptbroadening-analysis-xf/output-files/pt2-distributions.root
scp ${account}:${folder}/nominal/ptbroadening-analysis/output-files/pt2-distributions.root    ${mother_dir}/${syst}/${b}/ptbroadening-analysis/output-files/pt2-distributions.root
scp ${account}:${folder}/nominal/ptbroadening-analysis-xf/output-files/pt2-distributions.root ${mother_dir}/${syst}/${b}/ptbroadening-analysis-xf/output-files/pt2-distributions.root
