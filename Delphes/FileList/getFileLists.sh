#!/bin/bash

## This is the script I got from John Stupak for the Delphes HTbin sample

#baseDir=/pnfs/cms/WAX/11/store/user/snowmass/Delphes-3.0.9
baseDir=/pnfs/cms/WAX/11/store/user/snowmass/HTBinned/Delphes-3.0.9.1

for PU in No 50 140
  do
  for background in `ls ${baseDir}/${PU}PileUp`
    do
    for file in `ls ${baseDir}/${PU}PileUp/${background}/*root`
      do
      dcachePath=dcap://cmsgridftp.fnal.gov:24125/pnfs/fnal.gov/usr/`echo ${file} | cut -d '/' -f 3-`
      echo $dcachePath >> ntuples/${background}_${PU}PileUp.list
    done
  done
done


## Move the file to my naming convention

for PU in No 50 140
do
  mv ntuples/B-4p-0-1-v1510_14TEV_${PU}PileUp.list                         HTBin/B_14TEV_HT1_${PU}PileUp.list
  mv ntuples/BB-4p-0-300-v1510_14TEV_${PU}PileUp.list                      HTBin/BB_14TEV_HT1_${PU}PileUp.list
  mv ntuples/BB-4p-1300-2100-v1510_14TEV_${PU}PileUp.list                  HTBin/BB_14TEV_HT4_${PU}PileUp.list
  mv ntuples/BB-4p-2100-100000-v1510_14TEV_${PU}PileUp.list                HTBin/BB_14TEV_HT5_${PU}PileUp.list
  mv ntuples/BB-4p-300-700-v1510_14TEV_${PU}PileUp.list                    HTBin/BB_14TEV_HT2_${PU}PileUp.list
  mv ntuples/BB-4p-700-1300-v1510_14TEV_${PU}PileUp.list                   HTBin/BB_14TEV_HT3_${PU}PileUp.list
  mv ntuples/BBB-4p-0-600-v1510_14TEV_${PU}PileUp.list                     HTBin/BBB_14TEV_HT1_${PU}PileUp.list
  mv ntuples/BBB-4p-1300-100000-v1510_14TEV_${PU}PileUp.list               HTBin/BBB_14TEV_HT3_${PU}PileUp.list
  mv ntuples/BBB-4p-600-1300-v1510_14TEV_${PU}PileUp.list                  HTBin/BBB_14TEV_HT2_${PU}PileUp.list
  mv ntuples/Bj-4p-0-300-v1510_14TEV_${PU}PileUp.list                      HTBin/BJ_14TEV_HT1_${PU}PileUp.list
  mv ntuples/Bj-4p-1100-1800-v1510_14TEV_${PU}PileUp.list                  HTBin/BJ_14TEV_HT4_${PU}PileUp.list
  mv ntuples/Bj-4p-1800-2700-v1510_14TEV_${PU}PileUp.list                  HTBin/BJ_14TEV_HT5_${PU}PileUp.list
  mv ntuples/Bj-4p-2700-3700-v1510_14TEV_${PU}PileUp.list                  HTBin/BJ_14TEV_HT6_${PU}PileUp.list
  mv ntuples/Bj-4p-300-600-v1510_14TEV_${PU}PileUp.list                    HTBin/BJ_14TEV_HT2_${PU}PileUp.list
  mv ntuples/Bj-4p-3700-100000-v1510_14TEV_${PU}PileUp.list                HTBin/BJ_14TEV_HT7_${PU}PileUp.list
  mv ntuples/Bj-4p-600-1100-v1510_14TEV_${PU}PileUp.list                   HTBin/BJ_14TEV_HT3_${PU}PileUp.list
  mv ntuples/Bjj-vbf-4p-0-700-v1510_14TEV_${PU}PileUp.list                 HTBin/BJJ_14TEV_HT1_${PU}PileUp.list
  mv ntuples/Bjj-vbf-4p-1400-2300-v1510_14TEV_${PU}PileUp.list             HTBin/BJJ_14TEV_HT3_${PU}PileUp.list
  mv ntuples/Bjj-vbf-4p-2300-3400-v1510_14TEV_${PU}PileUp.list             HTBin/BJJ_14TEV_HT4_${PU}PileUp.list
  mv ntuples/Bjj-vbf-4p-3400-100000-v1510_14TEV_${PU}PileUp.list           HTBin/BJJ_14TEV_HT5_${PU}PileUp.list
  mv ntuples/Bjj-vbf-4p-700-1400-v1510_14TEV_${PU}PileUp.list              HTBin/BJJ_14TEV_HT2_${PU}PileUp.list
  mv ntuples/H-4p-0-300-v1510_14TEV_${PU}PileUp.list                       HTBin/H_14TEV_HT1_${PU}PileUp.list
  mv ntuples/H-4p-1500-100000-v1510_14TEV_${PU}PileUp.list                 HTBin/H_14TEV_HT4_${PU}PileUp.list
  mv ntuples/H-4p-300-800-v1510_14TEV_${PU}PileUp.list                     HTBin/H_14TEV_HT2_${PU}PileUp.list
  mv ntuples/H-4p-800-1500-v1510_14TEV_${PU}PileUp.list                    HTBin/H_14TEV_HT3_${PU}PileUp.list
  mv ntuples/LL-4p-0-100-v1510_14TEV_${PU}PileUp.list                      HTBin/LL_14TEV_HT1_${PU}PileUp.list
  mv ntuples/LL-4p-100-200-v1510_14TEV_${PU}PileUp.list                    HTBin/LL_14TEV_HT2_${PU}PileUp.list
  mv ntuples/LL-4p-1400-100000-v1510_14TEV_${PU}PileUp.list                HTBin/LL_14TEV_HT6_${PU}PileUp.list
  mv ntuples/LL-4p-200-500-v1510_14TEV_${PU}PileUp.list                    HTBin/LL_14TEV_HT3_${PU}PileUp.list
  mv ntuples/LL-4p-500-900-v1510_14TEV_${PU}PileUp.list                    HTBin/LL_14TEV_HT4_${PU}PileUp.list
  mv ntuples/LL-4p-900-1400-v1510_14TEV_${PU}PileUp.list                   HTBin/LL_14TEV_HT5_${PU}PileUp.list
  mv ntuples/LLB-4p-0-400-v1510_14TEV_${PU}PileUp.list                     HTBin/LLB_14TEV_HT1_${PU}PileUp.list
  mv ntuples/LLB-4p-400-900-v1510_14TEV_${PU}PileUp.list                   HTBin/LLB_14TEV_HT2_${PU}PileUp.list
  mv ntuples/LLB-4p-900-100000-v1510_14TEV_${PU}PileUp.list                HTBin/LLB_14TEV_HT3_${PU}PileUp.list
  mv ntuples/tB-4p-0-500-v1510_14TEV_${PU}PileUp.list                      HTBin/TB_14TEV_HT1_${PU}PileUp.list
  mv ntuples/tB-4p-1500-2200-v1510_14TEV_${PU}PileUp.list                  HTBin/TB_14TEV_HT4_${PU}PileUp.list
  mv ntuples/tB-4p-2200-100000-v1510_14TEV_${PU}PileUp.list                HTBin/TB_14TEV_HT5_${PU}PileUp.list
  mv ntuples/tB-4p-500-900-v1510_14TEV_${PU}PileUp.list                    HTBin/TB_14TEV_HT2_${PU}PileUp.list
  mv ntuples/tB-4p-900-1500-v1510_14TEV_${PU}PileUp.list                   HTBin/TB_14TEV_HT3_${PU}PileUp.list
  mv ntuples/tj-4p-0-500-v1510_14TEV_${PU}PileUp.list                      HTBin/TJ_14TEV_HT1_${PU}PileUp.list
  mv ntuples/tj-4p-1000-1600-v1510_14TEV_${PU}PileUp.list                  HTBin/TJ_14TEV_HT3_${PU}PileUp.list
  mv ntuples/tj-4p-1600-2400-v1510_14TEV_${PU}PileUp.list                  HTBin/TJ_14TEV_HT4_${PU}PileUp.list
  mv ntuples/tj-4p-2400-100000-v1510_14TEV_${PU}PileUp.list                HTBin/TJ_14TEV_HT5_${PU}PileUp.list
  mv ntuples/tj-4p-500-1000-v1510_14TEV_${PU}PileUp.list                   HTBin/TJ_14TEV_HT2_${PU}PileUp.list
  mv ntuples/tt-4p-0-600-v1510_14TEV_${PU}PileUp.list                      HTBin/TT_14TEV_HT1_${PU}PileUp.list
  mv ntuples/tt-4p-1100-1700-v1510_14TEV_${PU}PileUp.list                  HTBin/TT_14TEV_HT3_${PU}PileUp.list
  mv ntuples/tt-4p-1700-2500-v1510_14TEV_${PU}PileUp.list                  HTBin/TT_14TEV_HT4_${PU}PileUp.list
  mv ntuples/tt-4p-2500-100000-v1510_14TEV_${PU}PileUp.list                HTBin/TT_14TEV_HT5_${PU}PileUp.list
  mv ntuples/tt-4p-600-1100-v1510_14TEV_${PU}PileUp.list                   HTBin/TT_14TEV_HT2_${PU}PileUp.list

done
