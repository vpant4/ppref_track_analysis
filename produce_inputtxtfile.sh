#!/bin/bash

count=1
for subdir in /eos/cms/store/group/phys_heavyions/vpant/ppref2024output/PPRefZeroBiasPlusForward4/crab_ppref2024/250213_054928/000*/; do
  output_file="/afs/cern.ch/user/v/vpant/pprefanalysis/filelists/ppzerobias2024/pprefzerobiasplusforward4_set${count}.txt"
  find "$subdir" -type f -name "*.root" > "$output_file"
  ((count++))
done
