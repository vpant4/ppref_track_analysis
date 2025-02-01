#!/bin/bash

# echo "Setup CMSSW (ROOT version)"
cd /afs/cern.ch/user/v/vpant/pPb_8.16TeV_Analysis
#eval `scramv1 runtime -sh`
#cd /afs/cern.ch/user/v/vpant/CMSSW_13_0_5/src/pPb2016skims
#mkdir -p cond
echo "Submit jobs at "
echo PWD: $PWD

./Tree_Analyzer $1 $2 $3 $4 $5 $6
