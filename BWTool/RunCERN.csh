#!/bin/csh

set PRO       = $1
set DIR       = $2
set PUs       = $3
set DEC       = $4

set HTSplit   = 1 
set SplitLine = 100
set DEL       = DELDIR
set EXE       = ${DEL}/DELEXE

#set PRO=WJETS_13TEV
#set DIR=TEST_5_30
#============================================================================#
#-----------------------------   Setup the env   ----------------------------#
#============================================================================#
echo "============  Running on" $HOST " ============"
set LOCAL=`dirname $0`
cd $LOCAL
set LOCAL=`pwd`  
cd ~/CMSSW_5_3_8/src
eval `scramv1 runtime -csh`
cd -
setenv DELPHES Delphes-3.0.10
setenv LD_LIBRARY_PATH ${DEL}/../${DELPHES}:$PWD/$DELPHES/:$LD_LIBRARY_PATH
setenv PATH  $PWD/BWTool/:$PATH
cd $LOCAL
#tar -xzf $DEL/FileList.tgz

#============================================================================#
#--------------------------   To Run the Process   --------------------------#
#============================================================================#
set toru = ''
set count = 0

foreach PU (`echo $PUs`)
  #============================================================================#
  #---------------------------   Inclusive Sample   ---------------------------#
  #============================================================================#
  ## Inclusive sample
  set totalfile = `ls FileList/${DEC}/${PRO}*${PU}.list | wc -l`
  if ($totalfile == 0) then
    echo "No file for " $PRO " at " $PU " with " $DEC
    exit
  endif

  set Filesize = `wc FileList/${DEC}/${PRO}*${PU}.list | tail -1 | cut -f 3 -d ' '`
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ No Splitting ~~~~~
  if (${HTSplit} == 0 || $SplitLine > $Filesize) then ## No splitting 
    set HTSplit = 0
    echo $PU $PRO $DIR $DEC
    set toru=`echo $PU $PRO $DIR $DEC ${PRO}_${PU}_${DEC}.log $toru`
    @ count += 1
  else
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Splitting ~~~~~
    ## First, splitting the Filelist into several runs locally
    foreach file (`ls FileList/${DEC}/${PRO}_${PU}.list`)
      ## Check the file size, no more than 100 splited file
      set wcline = `wc -l $file| cut -f 1 -d ' ' `
      set MinLine = `echo ${wcline}/100 | bc`
      if ($MinLine > $SplitLine) then
        set NewSplitLine = `echo ${MinLine}+1 | bc`
        split -l $NewSplitLine -d $file ${PRO}_split_
      else
        split -l $SplitLine -d $file ${PRO}_split_
      endif

      set num = `ls ${PRO}_split_*|wc -l `
      set num = `echo $num-1 |bc`
      foreach sp (`seq -f "%02g" 00 1 ${num}`)
        mv ${PRO}_split_${sp} FileList/${DEC}/${PRO}_${sp}_${PU}.list
      end
    end

    ## NOW, let us adding this into the list

    foreach file (`ls FileList/${DEC}/${PRO}_*_${PU}.list`)
      @ count += 1

      if (`echo $PRO | grep -c "HT"`) then
        set name = `basename $file | cut -f 1-4 -d '_'`
      else
        set name = `basename $file | cut -f 1-3 -d '_'`
      endif
      echo $PU $name $DIR $DEC
      set toru=`echo $PU $name $DIR $DEC ${name}_${PU}_${DEC}.log $toru`
    end
  endif
end
#echo $toru

echo "------------ " $count "jobs in total ------------"
echo $toru | xargs -n 5 -P4  sh -c  $EXE' $0 $1 $2 $3 > $4'



##============================================================================#
##---------------------------------   HTadd   --------------------------------#
##============================================================================#
#ls
foreach PU (`echo $PUs`)
  foreach Pro (`ls *${PU}.root | cut -f 1 -d '_' | uniq`)
    set NewPro = `echo $PRO | cut -f 2- -d '_'`
    set NewPro = ${Pro}_${NewPro}
    ls ${NewPro}_*_${PU}.root
    echo ${NewPro}_${PU}_${DEC}.root 
    if (${HTSplit} == 1) then
      ${DEL}/../BWTool/HTadd  ${NewPro}_${PU}_${DEC}.root ${NewPro}_*_${PU}_${DEC}.root
      if ($? == 0) then
        tar -rf ${PRO}_${PU}_${DEC}.tar ${NewPro}_*_${PU}_${DEC}.root
      endif
      if ($? == 0) then
        rm ${NewPro}_*_${PU}_${DEC}.root
      endif
    #else
      #mv ${NewPro}_${PU}.root ${NewPro}_${PU}_${DEC}.root 
    endif
  end

  if ($? == 0) then
    tar -rf ${PRO}_${PU}_${DEC}.tar *.log
    tar -czf ${PRO}_${PU}_${DEC}.tgz ${PRO}_${PU}_${DEC}.tar
  endif

  if ($? == 0) then
    rm *.log
    rm -rf ${PRO}_${PU}_${DEC}.tar
  endif
end
