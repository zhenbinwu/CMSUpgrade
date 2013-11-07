source /uscmst1/prod/sw/cms/cshrc prod
#grid-proxy-init -debug -verify
#voms-proxy-init -voms cms
cd /uscms/home/benwu/CMSSW_5_2_5/src
#cd CMSSW_6_0_1_PostLS1v2_patch3/
#cms
cmsenv
cd -
setenv CURRENTDIR /uscms/home/benwu/work/CMSUpgrade/

#if ( ! -d OxbridgeMT2) then
  #mkdir -p temp
  #cd temp
  #wget http://www.hep.phy.cam.ac.uk/~lester/dtm662/mt2/Releases/oxbridgekinetics.tar.gz
  #tar -xzvf oxbridgekinetics.tar.gz 
  #cd oxbridgekinetics-0.6/ 
  #./configure --prefix=$CURRENTDIR/OxbridgeMT2/ --without-minuit2
  #make
  #make install
  #cd ../..
  #rm -rf temp
#endif

#unsetenv CURRENTDIR

setenv DELPHES Delphes-3.0.10
setenv LD_LIBRARY_PATH $CURRENTDIR/OxbridgeMT2/lib/:$CURRENTDIR/$DELPHES/:$LD_LIBRARY_PATH
setenv PATH  $CURRENTDIR/BWTool/:$PATH
