#!/bin/sh

parse() {
	# grep -e '[0-9]$' ..
	grep -e '[0-9]$' $1 | sed -e 's/__NR_//g' -e 's/SYS_//g' -e 's/SVR4_//g' -e 's/ULTRIX_//g' | awk '{print $3 " " $2 }' > $i.t
}

for i in `ls inc/*.h`; do parse $i ; done
