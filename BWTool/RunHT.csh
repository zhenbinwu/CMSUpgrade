#!/bin/csh

set PRO       = $1
set DIR       = $2

set HTSplit   = 1 
set SplitLine = 100
set DEL       = /uscms/home/benwu/work/CMSUpgrade/Delphes/
set EXE       = ${DEL}/DelFill
set PUs       = PILEUPS

#set PRO=WJETS_13TEV
#set DIR=TEST_5_30
#============================================================================#
#-----------------------------   Setup the env   ----------------------------#
#============================================================================#
echo "============  Running on" $HOST " ============"
set LOCAL=`dirname $0`
cd $LOCAL
set LOCAL=`pwd`  
source ${DEL}/../setup.sh 
cd $LOCAL
tar -xzf FileList.tgz

#============================================================================#
#--------------------------   To Run the Process   --------------------------#
#============================================================================#
set toru = ''
set count = 0

foreach PU (`echo $PUs`)
  #============================================================================#
  #-----------------------------   HTBin Sample   -----------------------------#
  #============================================================================#
  if (`echo $PRO | grep -c "HT"`) then
    set totalfile = `ls FileList/HTBin/${PRO}_${PU}.list | wc -l`
    if ($totalfile == 0) then
      echo "No file for " $PRO " at " $PU
      exit
    endif

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ No Splitting ~~~~~
    if (${HTSplit} == 0) then ## No splitting 
    echo $PU $PRO $DIR 
    set toru=`echo $PU $PRO $DIR ${PRO}_${PU}.log $toru`
    @ count += 1
    else  ## split the HTBin filelist into several run

    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Splitting ~~~~~
    ## First, splitting the Filelist into several runs locally
    foreach file (`ls FileList/HTBin/${PRO}_${PU}.list`)
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
        mv ${PRO}_split_${sp} FileList/HTBin/${PRO}_${sp}_${PU}.list
      end
    end

    ## NOW, let us adding this into the list

    foreach file (`ls FileList/HTBin/${PRO}_*_${PU}.list`)
      @ count += 1
      set name = `basename $file | cut -f 1-4 -d '_'`
      echo $PU $name $DIR
      set toru=`echo $PU $name $DIR ${name}_${PU}.log $toru`
    end
  endif
else
  #============================================================================#
  #---------------------------   Inclusive Sample   ---------------------------#
  #============================================================================#
  ## Inclusive sample
  set totalfile = `ls FileList/DEL309/${PRO}*${PU}.list | wc -l`
  if ($totalfile == 0) then
    echo "No file for " $PRO " at " $PU
    exit
  endif

  set Filesize = `wc FileList/DEL309/${PRO}*${PU}.list | tail -1 | cut -f 3 -d ' '`
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ No Splitting ~~~~~
  if (${HTSplit} == 0 || $SplitLine > $Filesize) then ## No splitting 
    set HTSplit = 0
    echo $PU $PRO $DIR 
    set toru=`echo $PU $PRO $DIR ${PRO}_${PU}.log $toru`
    @ count += 1
  else
    #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Splitting ~~~~~
    ## First, splitting the Filelist into several runs locally
    foreach file (`ls FileList/DEL309/${PRO}_${PU}.list`)
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
        mv ${PRO}_split_${sp} FileList/DEL309/${PRO}_${sp}_${PU}.list
      end
    end

    ## NOW, let us adding this into the list

    foreach file (`ls FileList/DEL309/${PRO}_*_${PU}.list`)
      @ count += 1
      set name = `basename $file | cut -f 1-3 -d '_'`
      echo $PU $name $DIR
      set toru=`echo $PU $name $DIR ${name}_${PU}.log $toru`
    end
  endif

  endif
end
#echo $toru

echo "------------ " $count "jobs in total ------------"
echo $toru | xargs -n 4 -P8  sh -c  $EXE' $0 $1 $2 > $3'



#============================================================================#
#---------------------------------   HTadd   --------------------------------#
#============================================================================#
foreach PU (NoPileUp 50PileUp 140PileUp)
  foreach Pro (`ls *${PU}.root | cut -f 1 -d '_' | uniq`)
    set NewPro = `echo $PRO | cut -f 2- -d '_'`
    set NewPro = ${Pro}_${NewPro}
    if (${HTSplit} == 1) then
      ${DEL}/../BWTool/HTadd  ${NewPro}_${PU}.root ${NewPro}_*_${PU}.root
      if ($? == 0) then
        tar -rf ${PRO}.tar ${NewPro}_*_${PU}.root
      endif
      if ($? == 0) then
        rm ${NewPro}_*_${PU}.root
      endif
    endif
  end
end

if ($? == 0) then
  tar -rf ${PRO}.tar *.log
  tar -czf ${PRO}.tgz ${PRO}.tar
endif

if ($? == 0) then
  rm *.log
  rm -rf ${PRO}.tar
endif
