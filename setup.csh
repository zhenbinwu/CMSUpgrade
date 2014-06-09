setenv CURRENTDIR $PWD

#============================================================================#
#---------------------------   Setup CMSSW @ LPC   --------------------------#
#============================================================================#
if (  $HOST =~ cmslpc* ) then 
    source /uscmst1/prod/sw/cms/cshrc prod
    setenv LANG en_US.UTF-8
    ##grid-proxy-init -debug -verify
    ##voms-proxy-init -voms cms
    cd $HOME/CMSSW_5_2_5/src
    cmsenv
    cd $CURRENTDIR
endif

#============================================================================#
#--------------------------   Setup CMSSW @ CERN   --------------------------#
#============================================================================#


#============================================================================#
#-------------------------   Setup CMSSW @ Kodiak   -------------------------#
#============================================================================#


setenv DELPHES Delphes-3.0.10
setenv LD_LIBRARY_PATH $CURRENTDIR/$DELPHES/:$LD_LIBRARY_PATH
setenv PATH  $CURRENTDIR/BWTool/:$PATH

#============================================================================#
#---------------------------   Setup MT2 package   --------------------------#
#============================================================================#
setenv OxbridgeMT2 True

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
