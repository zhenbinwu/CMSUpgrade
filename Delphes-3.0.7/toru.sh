#!/bin/sh
source ~cmssoft/shrc
cd ~/CMSSW_5_2_5
cmsenv
cd /home/benwu/CMSSW_5_2_5/src/Delphes-3.0.7/
/home/benwu/CMSSW_5_2_5/src/Delphes-3.0.7/DelphesLHEF $pufile Delphes_Wino${wino}_${pu}_${arg1}.root LHE/${wino}_splited/wino${wino}_events_${arg1}.lhe
