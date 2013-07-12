#!/bin/csh

set PRO=$1
set DIR=$2

#set PRO=WJETS_13TEV
#set DIR=TEST_5_30

#============================================================================#
#-----------------------------   Setup the env   ----------------------------#
#============================================================================#
echo "============  Running on" $HOST " ============"
set LOCAL=`dirname $0`
cd $LOCAL
set LOCAL=`pwd`  
cd ..
source setup.sh 
cd $LOCAL

#============================================================================#
#--------------------------   To Run the Process   --------------------------#
#============================================================================#
set EXE=./DelFill
set toru = ''
set count = 0


mkdir -p $DIR
foreach PU (NoPileUp 50PileUp 140PileUp)
  ## HT bin sample
  if (`echo $PRO | grep -c "HT"`) then
    set totalfile = `ls FileList/HTBin/${PRO}*${PU}.list | wc -l`
    if ($totalfile == 0) then
      echo "No file for " $PRO " at " $PU
      exit
    endif

    set splitfile = `ls FileList/HTBin/${PRO}_*_${PU}.list | wc -l`

    if ($splitfile == 0) then
      set toru=`echo $PU $PRO $DIR $toru`
      @ count += 1
    else
      foreach file (`ls FileList/HTBin/${PRO}_*_${PU}.list`)
        @ count += 1
        echo $count ":" $file
        echo $PU `basename $file | cut -f 1-4 -d '_'` $DIR
        set name = `basename $file | cut -f 1-4 -d '_'`
        set toru=`echo $PU $name $DIR $DIR/${name}_${PU}.log $toru`
      end
    endif
  else
    ## Inclusive sample
    set totalfile = `ls FileList/DEL309/${PRO}*${PU}.list | wc -l`
    if ($totalfile == 0) then
      echo "No file for " $PRO " at " $PU
      exit
    endif

    echo $PU $PRO $DIR 
    set toru=`echo $PU $PRO $DIR $toru`
    @ count += 1
  endif
end

echo "------------ " $count "jobs in total ------------"
echo $toru | xargs -n 4 -P6  sh -c  $EXE' $0 $1 $2 > $3'



#============================================================================#
#---------------------------------   HTadd   --------------------------------#
#============================================================================#
#cd $DIR
#HTadd ${PRO}_${PU}.root ${PRO}_*_${PU}*root

