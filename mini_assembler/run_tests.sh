#!bin/sh

exec_file_name=minassm
option=$1

example_1=tests/example_1/example_1.asm
example_2=tests/example_2/errors.asm
example_3=tests/example_3/errors_warnings.asm
example_4=tests/example_4/example.asm

lst_filename_1=tests/example_1/example_1.lst
lst_filename_2=tests/example_2/errors_1.lst
lst_filename_3=tests/example_3/errors_warnings.lst
lst_filename_4=tests/example_4/example.lst


test_examples(){
	./$exec_file_name -i $1 -o $2
	printf "\nasm file = "
	echo $1
	printf "lst file = "
	echo $2
	echo
	if [ -e $2 ]; then
		cat $2
	fi
	return 0
}


if [ -e $exec_file_name ];
then
	case $option in 
		1) test_examples $example_1 $lst_filename_1;;
		2) test_examples $example_2 $lst_filename_2;;
		3) test_examples $example_3 $lst_filename_3;;
		4) test_examples $example_4 $lst_filename_4;;
	esac
else
	echo "File minassm does not exists!"
fi


