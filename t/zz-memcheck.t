#!/bin/sh

cwd=$(dirname $0)
scriptname=$(basename $0)
libtool=$(which libtool)
valgrind=$(which valgrind)

findoptions=$(echo -type f -executable ! -name $scriptname ! -regex .*\.libs/.*)
files=$(find ${cwd} ${findoptions})

count=0
for file in ${files}; do
    count=$(expr $count + 1)
done

current=0

print_ok() {
    current=$(expr $current + 1)
    echo "ok ${current} no memory leaks in $1"
}

print_not_ok() {
    current=$(expr $current + 1)
    echo "not ok ${current} no memory leaks in $1"
    echo "# rerun with '${valgrind_cmd}' for details"
}

echo "1..${count}"

for file in ${files}; do
	valgrind_cmd="${libtool} --mode=execute ${valgrind} -q --error-exitcode=2 --suppressions=${cwd}/valgrind_suppressions --leak-check=full $file"
	valgrind_output=$(${valgrind_cmd} 2>&1 1>/dev/null)
	if [ $? -ne 2 ]; then
		print_ok "${file}"
	else
		print_not_ok "${file}"
	fi
done
