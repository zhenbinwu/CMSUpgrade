#!/bin/tcsh

foreach file (`ls *list`)
  set ht=`echo $file| cut -f 1-3 -d _ `
  set pu=`echo $file| cut -f 4- -d _ `
  if ( `wc -l $file | cut -f 1 -d ' ' ` < 200) then
    continue 
  endif
  split -a 1 -l 100 -d $file splited_file_
  echo "------------------------"
  ls splited_file_*
  set num = `ls splited_file_*|wc -l`
  foreach sp (`seq 1 ${num}`)
    echo ${ht}_${sp}_${pu}
    set sp2 = `echo ${sp}-1 | bc`
    mv splited_file_${sp2} ${ht}_${sp}_${pu}
    #rename s/x\(\d+\)/${ht}_\1/ $sp 
  end
end
