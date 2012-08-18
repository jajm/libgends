#!/bin/sh

cwd=$(dirname $0)
scriptname=$(basename $0)
exitcode=0

findoptions=$(echo -type f -executable ! -name $scriptname ! -regex .*\.libs/.*)

for file in $(find $cwd $findoptions); do
	echo -n "$file... "
	$file >/dev/null 2>&1
	if [ $? -eq 0 ]; then
		echo "OK"
	else
		echo "KO !"
		exitcode=1
	fi
done

exit $exitcode
