#!/bin/bash

count=1
for subdir in /eos/cms/store/group/phys_heavyions/vpant/ppref2024output/Pythia8_NoPU_0p5MEvents_TuneCP5_QCD_ptHat15_5360GeV/crab_ppref2024/250303_053315/000*/; do
  output_file="/afs/cern.ch/user/v/vpant/pprefanalysis/filelists/MC/Pythiapthat15/Pythiapthat15.txt"
  find "$subdir" -type f -name "*.root" > "$output_file"
  ((count++))
done
