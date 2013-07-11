#!/bin/csh

set PU=$1
set PRO=$2
set DIR=$3

#set PU=50PileUp
#set PRO=BB_14TEV_HT2
#set DIR=TEST_5_30

#============================================================================#
#-----------------------------   Setup the env   ----------------------------#
#============================================================================#
set LOCAL=`dirname $0`
cd $LOCAL/../
source setup.sh 
cd $LOCAL

#============================================================================#
#--------------------------   To Run the Process   --------------------------#
#============================================================================#
set EXE=./DelFill
set toru = ''

mkdir -p $DIR
if (`echo $PRO | grep -c "HT"`) then
  set totalfile = `ls FileList/HTBin/${PRO}*${PU}.list | wc -l`
  if ($totalfile == 0) then
    echo "No file for " $PRO " at " $PU
    exit
  endif

  set splitfile = `ls FileList/HTBin/${PRO}_*_${PU}.list | wc -l`

  if ($splitfile == 0) then
      set toru=`echo $PU $PRO $DIR $toru`
  else
    foreach file (`ls FileList/HTBin/${PRO}_*_${PU}.list`)
      echo $file
      echo $PU `basename $file | cut -f 1-4 -d '_'` $DIR
      set name = `basename $file | cut -f 1-4 -d '_'`
      set toru=`echo $PU $name $DIR $DIR/${name}_${PU}.log $toru`
    end
  endif
else
  set toru=`echo $PU $PRO $DIR $toru`
endif

echo "------------"
echo $toru | xargs -n 4 -P2  sh -c  $EXE' $0 $1 $2 > $3'



#============================================================================#
#---------------------------------   HTadd   --------------------------------#
#============================================================================#
#cd $DIR
#HTadd ${PRO}_${PU}.root ${PRO}_*_${PU}*root

