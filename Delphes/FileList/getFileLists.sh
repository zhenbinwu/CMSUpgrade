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
      dcachePath=dcap://cmsgridftp.fnal.gov:24125/pnfs/fnal.gov/usr/`echo ${baseDir}/${PU}PileUp/${background} | cut -d '/' -f 3-`/${file}
      echo $dcachePath >> ntuples/${background}_${PU}PileUp_files.txt
    done
  done
done


