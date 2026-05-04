#!/usr/bin/bash

names=$(grep -oh "class .*(" enc.py | cut -d " " -f2 | rev | cut -c 2- | rev | grep -v "M.reg\|X31reg\|BAreg\|SPreg\|FPOFFreg")

for name in $names; do
   if $(grep -q -r "class.*($name)" ../../.)
   then
      echo "$name found"
   else
      echo "$name missing"
   fi
done
