#!/bin/sh

cwd=$(dirname $0)
scriptname=$(basename $0)
exitcode=0
echo=$(which echo)
libtool=$(which libtool)
valgrind=$(which valgrind)

findoptions=$(echo -type f -executable ! -name $scriptname ! -regex .*\.libs/.*)
files=$(find ${cwd} ${findoptions})
maxlen=0
for file in ${files}; do
	len=$(expr length ${file})
	if [ ${len} -gt ${maxlen} ]; then
		maxlen=${len}
	fi
done
length=$(expr ${maxlen} + length " memcheck " + 3)

print_status_line() {
	local string="$1"
	local strlen=$(expr length "${string}")
	local dotslen=$(expr ${length} - ${strlen} - 1)
	echo -n "${string} "
	for var in $(seq ${dotslen}); do
		echo -n "."
	done
}

print_ok_ko() {
	local status="$1"
	local errormsg="$2"
	if [ ${status} -eq 0 ]; then
		${echo} -e " \x1b[32mOK\x1b[0m"
	else
		${echo} -e " \x1b[31;1mKO!\x1b[0m"
		${echo} "${errormsg}"
		exitcode=1
	fi
}

for file in ${files}; do
	print_status_line "${file}"
	$file >/dev/null 2>&1
	print_ok_ko $?

	print_status_line "${file} memcheck"
	valgrind_cmd="${libtool} --mode=execute ${valgrind} -q --error-exitcode=1 --suppressions=${cwd}/valgrind_suppressions --leak-check=full $file"
	valgrind_output=$(${valgrind_cmd} 2>&1 1>/dev/null)
	print_ok_ko $? "${valgrind_output}"
done

exit $exitcode
