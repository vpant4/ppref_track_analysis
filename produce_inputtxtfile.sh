#!/bin/bash

count=1
for subdir in /eos/cms/store/group/phys_heavyions/vpant/ppref2024output/PPRefZeroBiasPlusForward2/crab_ppref2024/250129_075322/000*/; do
  output_file="/afs/cern.ch/user/v/vpant/pprefanalysis/filelists/ppzerobias2024/pprefzerobiasplusforward2_set${count}.txt"
  find "$subdir" -type f -name "*.root" > "$output_file"
  ((count++))
done
