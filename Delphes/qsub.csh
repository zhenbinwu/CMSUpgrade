#!/bin/tcsh

foreach i (`seq 5`)
  qsub toru.sh $i
end
