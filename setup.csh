setenv CURRENTDIR $PWD

#============================================================================#
#---------------------------   Setup CMSSW @ LPC   --------------------------#
#============================================================================#
if (  $HOST =~ cmslpc* ) then 
    source /uscmst1/prod/sw/cms/cshrc prod
    ##grid-proxy-init -debug -verify
    ##voms-proxy-init -voms cms
    cd $HOME/CMSSW_5_2_5/src
    cmsenv
    setenv LANG en_US.UTF-8
    cd $CURRENTDIR
endif

#============================================================================#
#--------------------------   Setup CMSSW @ CERN   --------------------------#
#============================================================================#


#============================================================================#
#-------------------------   Setup CMSSW @ Kodiak   -------------------------#
#============================================================================#
if (  $HOST =~ n* ) then 
    source ~cmssoft/cshrc
    cd $HOME/CMSSW_5_3_10/src
    cmsenv
    setenv LANG en_US.UTF-8
    cd $CURRENTDIR
endif

#setenv DELPHES Delphes-TP        # TP Version 
#setenv DELPHES Delphes-3.0.10    # ECFA Version
setenv DELPHES Delphes-3.0.9      # Snowmass Version
setenv LD_LIBRARY_PATH $CURRENTDIR/$DELPHES/:$LD_LIBRARY_PATH
setenv PATH  $CURRENTDIR/BWTool/:$PATH

#============================================================================#
#---------------------------   Setup MT2 package   --------------------------#
#============================================================================#
setenv OxbridgeMT2 False

if ( $OxbridgeMT2  == True ) then
  if (! -d OxbridgeMT2) then
    mkdir -p temp
    cd temp
    wget http://www.hep.phy.cam.ac.uk/~lester/dtm662/mt2/Releases/oxbridgekinetics.tar.gz
    tar -xzvf oxbridgekinetics.tar.gz 
    cd oxbridgekinetics-0.6/
    ./configure --prefix=$CURRENTDIR/OxbridgeMT2/ --without-minuit2
    make
    make install
    cd ../..
    rm -rf temp
  endif

  setenv LD_LIBRARY_PATH $CURRENTDIR/OxbridgeMT2/lib/:$LD_LIBRARY_PATH
endif

#============================================================================#
#------------------------   Setup Boost from CMSSW   ------------------------#
#============================================================================#
if ( $?CMSSW_FWLITE_INCLUDE_PATH) then 
  setenv BOOSTROOT `echo $CMSSW_FWLITE_INCLUDE_PATH | awk -F: '{for(i=0;++i<=NF;){if ($i ~ /.*boost.*/) print $i}}'`
endif
