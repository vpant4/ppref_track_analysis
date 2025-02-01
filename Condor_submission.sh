#!/bin/bash

# Directory containing input files
input_dir="/afs/cern.ch/user/v/vpant/pPb_8.16TeV_Analysis/MC_files"
output_dir="/eos/cms/store/group/phys_heavyions/vpant/MC_output_files"


# Loop through input files
for input_file in "$input_dir"/MC_pthat*_pgoing_unembedded.txt ; do
    # Extract base name without .txt extension
    pthat=$(basename "$input_file" "_pgoing_unembedded.txt" | sed 's/MC_pthat//')
    file_base="${input_file%.txt}"
    output_file="$output_dir/MC_pthat_$pthat_Pbgoing"
    # Execute the Python command
    python3 HTCondor_submit.py -i "$file_base" -o "$output_file" -m 1 -p "$pthat" -s htcon_sub_MC_$pthat
done

input_dir_MB="/afs/cern.ch/user/v/vpant/pPb_8.16TeV_Analysis/Pbgoing_files"
output_dir_MB="/eos/cms/store/group/phys_heavyions/vpant/Output_root_files/Pbgoing/MB_Data"

for input_file in "$input_dir_MB"/MB_PD*_Pbgoing.txt ; do
    # Extract base name without .txt extension                                                                                          
    file_base="${input_file%.txt}"
    num=$(basename "$input_file" "_Pbgoing.txt" | sed 's/MB_PD//')
    output_file="$output_dir_MB/MB_PD$num_Pbgoing"
    # Execute the Python command                                                                                                            
    python3 HTCondor_submit.py -i "$file_base" -o "$output_file" -m 0 -p 0 -s htcon_sub_$num
done


input_dir_trigger="/eos/cms/store/group/phys_heavyions/vpant/DATA_SAMPLES/PAEGJet/Pbgoing"
output_dir_trigger="/eos/cms/store/group/phys_heavyions/vpant/Output_root_files/Pbgoing/Trigger_Data"

for input_file in "$input_dir_trigger"/listoffiles_DATA_PAEGJet_Pbgoing.txt ; do
    # Extract base name without .txt extension                                                                                                                                                                                                                                 
    file_base="${input_file%.txt}"
    
    output_file="$output_dir_trigger/$Trigger_output"
    # Execute the Python command                                                                                                       
                                                                                  
    python3 HTCondor_submit.py -i "$file_base" -o "$output_file" -m 0 -p 0 -s htcon_sub_trigger
done
