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
length=$(expr ${maxlen} + length " (memcheck) " + 3)

print_string_and_dots() {
	local string="$1"
	local length="$2"
	local strlen=$(expr length "${string}")
	local dotslen=$(expr ${length} - ${strlen} - 1)
	echo -n "${string} "
	for var in $(seq ${dotslen}); do
		echo -n "."
	done
	echo -n " "
}

print_ok() {
	local msg="$1"
	${echo} -e "\x1b[32mOK\x1b[0m"
	if [ -n "${msg}" ]; then
		${echo} "${msg}"
	fi
}

print_ko() {
	local msg="$1"
	${echo} -e "\x1b[31;1mKO!\x1b[0m"
	if [ -n "${msg}" ]; then
		${echo} "${msg}"
	fi
}

for file in ${files}; do
	print_string_and_dots "${file}" ${length}
	${file} >/dev/null 2>&1
	if [ $? -eq 0 ]; then
		print_ok
	else
		print_ko
		exitcode=1
	fi

	print_string_and_dots "${file} (memcheck)" ${length}
	valgrind_cmd="${libtool} --mode=execute ${valgrind} -q --error-exitcode=2 --suppressions=${cwd}/valgrind_suppressions --leak-check=full $file"
	valgrind_output=$(${valgrind_cmd} 2>&1 1>/dev/null)
	if [ $? -ne 2 ]; then
		print_ok
	else
		print_ko "${valgrind_output}"
		exitcode=1
	fi
done

exit ${exitcode}
