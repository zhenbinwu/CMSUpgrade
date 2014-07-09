#!/bin/csh

set DIR        = $1
set SplitLine  = 20
set GetNewList = 0

if (GetNewList == 1) then
  rm ${DIR}/*
  rm -rf ${DIR}/ntuples
  ./getFileLists.sh ${DIR}
else if (GetNewList == -1) then
  rm ${DIR}/*.list
  find ${DIR} -type f -exec mv '{}' '{}'.list \;
endif


foreach file (`ls ${DIR}/*list`)
  
  if (`echo $file | grep -c "HT"`) then
    set PRO = `echo $file | cut -f 2 -d / | cut -f 1 -d \. | cut -f 1-3 -d '_'`
    set PU = `echo $file | cut -f 2 -d / | cut -f 1 -d \. | cut -f 4 -d '_'`
  else
    set PRO = `echo $file | cut -f 2 -d / | cut -f 1 -d \. | cut -f 1-2 -d '_'`
    set PU = `echo $file | cut -f 2 -d / | cut -f 1 -d \. | cut -f 3 -d '_'`
  endif

  set Filesize = `wc $file | tail -1 | cut -f 3 -d ' '`
  if ($SplitLine > $Filesize) then
    continue
  endif
  
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
    mv ${PRO}_split_${sp} ${DIR}/${PRO}_${sp}_${PU}.list
  end
  mv $file ${DIR}/${PRO}_${PU}
end

