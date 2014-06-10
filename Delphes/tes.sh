#!/bin/sh

while true; do
  ps 870 -o pid=,%mem=,vsz= >> /tmp/mem.log
  gnuplot /tmp/gnuplot.script
  sleep 1
done &


