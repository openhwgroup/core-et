#!/bin/bash

FILES=`find $REPOROOT/dv/tests/ioshire/*/spMin -name "makefile"`
FIND="scripts\/makefile.defines"
REPLACE="scripts\/spMin\/makefile.defines"

for i in $FILES
do
    echo $i
    sed -i "s/$FIND/$REPLACE/g" $i
done

echo Done.
