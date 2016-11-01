qt=0
if [ $# = 2 ]
then
	qt=$2
else
	qt=$1
fi

clear
for ((i=0; i<$qt; i++))
do
	INPUT=""
	for j in {1..5}
	do
		if (( $(shuf -i 0-1 -n 1) == 1 )) 
		then
			INPUT=${INPUT}R
		else
			INPUT=${INPUT}W
		fi
	done
	echo $INPUT
	./rw_problem $INPUT
	if [ $1 = -w ]
	then
		read -n 1 -s
		clear
	fi
done
echo Finished running tests