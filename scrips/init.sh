#!/bin/bash

# rename to project specific values
NAME="lbin"
CONST="lbin"
STRUCT="lbin"
FN="lbin"

# will be replaced
lbin_NAME="lbin"
lbin_STRUCT="lbin"
lbin_CONST="lbin"
lbin_FN="lbin"

function replace() {
	echo "Replacing $1 with $2"
	find ./ -type f -name '*.c' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.h' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.md' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.lua' -exec sed -i "s/$1/$2/g" {} \;
	find ./ -type f -name '*.sh' -exec sed -i "s/$1/$2/g" {} \;
	find ./doc/ -type f -name '*' -exec sed -i "s/$1/$2/g" {} \;
}

replace $lbin_NAME $NAME
replace $lbin_CONST $CONST
replace $lbin_STRUCT $STRUCT
replace $lbin_FN $FN

mv "include/$lbin_NAME.h" "include/$NAME.h"
mv "src/$lbin_NAME.c" "src/$NAME.c"
mv "doc/$lbin_NAME" "doc/$NAME" 
