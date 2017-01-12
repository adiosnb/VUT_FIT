#/!bin/dash

LC_ALL=POSIX
OIFS=$IFS
#IFS=$'\n'
IFS='
'

DIR="."
IGNORE=""

ARGS=$*
MAX_SIZE_FILE=0
MAX_NESTED_FILE=1
NUM_OF_DIRS=1
NUM_OF_FILES=0
SIZE_OF_ALL_FILES=0
SIZES_OF_FILES_ARRAY=
EXTENSION_STRING=
EXTENSIONS=

###########################################################
#inspect_dir  nazov_adresara_k_preskumaniu  hlbka_zanorenia
inspect_dir() {
	cd "$1" 2> /dev/null
	if [ "$?" -ne "0" ] ; then
		echo "Chyba pri vstupovani do adresara!"
		echo "Program sa ukonci!"
		exit 1
	fi
	
	#nastavi najhlbsie zanoreny subor
	if [ $2 -gt $MAX_NESTED_FILE ] ; then 
		MAX_NESTED_FILE=$2
	fi
		
	#prejde kazdy subor zlozky
	if [ "` ls  -A`" != "" ] ; then
		#for file in * ; do
		for file in `find . -maxdepth 1 ! -name "."` ; do
			#kontrola ignorovanych suborov
			#check_file $file
			check=1
			if [ "$check" = "1" ] ; then
				#ak je adresar spravi toto
				if [ -d "$file" ]  ; then
					NUM_OF_DIRS=$(( $NUM_OF_DIRS + 1 ))	
					#rekurzivne volanie
					inspect_dir "$file" $(( $2 + 1 ))
				#ak je subor spravi toto
				else
					if [ -f "$file" ] ; then
						if [ -L "$file" ] ; then
							echo "" > /dev/null
						else
							NUM_OF_FILES=$(( $NUM_OF_FILES + 1 ))
							extension=${file##*.}
							file_size=`ls -al "./$file" | awk '{print $5}'`
						#file_size=`stat --printf="%s" $file`
						#otestuje ci je vacsie ako maximalne
							if [ "$extension" != "" ] ; then
								is_dot=`echo $file | tr '/' '\t' | cut -f2  |grep "\."` 
								is_dot=`echo $is_dot | tr '.' ' ' | wc -w`
								if [ $is_dot -gt 1 ] ; then
									add_size_to_extensions "$extension" $file_size
								fi
							fi
							if [ $file_size -gt $MAX_SIZE_FILE ] ; then
								MAX_SIZE_FILE=$file_size
							fi
						#zapise velkosti suborov do pola
							SIZES_OF_FILES_ARRAY="$SIZES_OF_FILES_ARRAY $file_size" 	
						fi
					fi
				fi
			fi
		
		done 
	fi
	cd ..
}

############################################################
#zapis do EXTENSION_STRING velkost suboru
#add_size_to_extensions extension file_size
add_size_to_extensions() {
	current_size_string=`echo $EXTENSION_STRING | tr '&' '\n' | grep "/$1/"`
	if [ "$current_size_string" = "" ] ; then
		current_size_string="/$1/"
	fi
	EXTENSION_STRING=`echo $EXTENSION_STRING | tr '&' '\n' | grep -v "/$1/"|tr '\n' '&'` 
	current_size_string="$current_size_string:$2"
	EXTENSION_STRING="$EXTENSION_STRING$current_size_string"
}

############################################################
#vyparsovanie extensions do stringu
get_extensions() {
	IFS=$OIFS
	EXTENSIONS="`echo $EXTENSION_STRING | tr '&' '\n' | cut -d: -f1 |cut -d'/' -f2 |sort `"
	EXTENSIONS=`echo $EXTENSIONS | tr ' ' ','`
	IFS='
'
	}

############################################################
#Vypis statistyky podla extension
#print_ext extension
print_ext() {
	ext_string=`echo $EXTENSION_STRING | tr '&' '\n' | grep "/$1/"`
	ext_string=`echo $ext_string | tr ':' '\n' | grep -v "/$1/" | sort -n`
	max=`echo $ext_string | wc -w`

	if [ "$max" -ne "1" ] ; then
		middle=$(($max/2))
		if [ $(($max % 2)) -eq "1" ] ; then
			middle=$(( $middle + 1 ))
			median=`echo $ext_string | cut -d' ' -f$middle`
		else
			median=`echo $ext_string | cut -d' ' -f$middle`
			middle=$(($middle + 1 ))
			median_next=`echo $ext_string | cut -d' ' -f$middle`
			median=$(($median + $median_next))
			median=$(($median / 2))
		fi

		average=0
		for i in $ext_string ; do
			average=$(( $average + $i ))
		done
		average=$(( $average / $max ))
		max=$(($max))
		max_size=`echo $ext_string |cut -d' ' -f$max`
	else
		max_size=`echo $ext_string`
		average=$max_size
		median=$max_size
	fi
echo "Files .$1: $max"
echo "  Largest file .$1: $max_size"
echo "  Average file size .$1: $average"
echo "  File size median .$1: $median"

}

############################################################
#kontrola ci nie je ignore
check_file() {
	check=1
	if [ "$IGNORE" != "" ] ; then
		value=`echo $1 | egrep "$IGNORE"`
		if [ "$value" != "" ] ; then
			check=0
		fi
	fi
}

############################################################
#MAIN

##############################################################
#Zaciatok triedenia vstupu a nastavenie DIR alebo IGNORE

state=0
for i in $* ; do
	case $state in
		0|3)	if [ $i = "-i" ] ; then
				if [ $state -ne 3 ] ; then
					state=1
				else
					echo "Zle argumety na vstupe!"
					echo "Program sa ukonci!"
					exit 1	
				fi
			else
				if ! [ -d $i ] ; then
					echo "Zadany adresar neexistuje!"
					echo "program sa ukoci!"
					exit 1
				fi
				DIR=$i
				state=2
			fi
		;;
		
		1)	if [ $i != "" ] ; then
				if [ $i != "-i" ] ; then
					IGNORE="$IGNORE $i"
					state=0
				else
					echo "Zle argumety na vstupe!"
					echo "Program sa ukonci!"
					exit 1
				fi
			else
					echo "Zle argumety na vstupe!"
					echo "Program sa ukonci!"
					exit 1
			fi
		;;
		
		2)	echo "Zle argumety na vstupe!"
			echo "Program sa ukonci!"
			exit 1
		;;
	esac
done 

if [ "$state" -eq "1" ] ; then
	echo "Zle argumety na vstupe!"
	echo "Program sa ukonci!"
	exit 1
fi

check_file $DIR
if [ "$check" = "0" ] ; then 
	echo "Chyba vstupu!"
	echo "Medzi ignorovanymi subormi je prehladavany adresar!"
	exit 1
fi

#Koniec zadavania vstupu

inspect_dir $DIR 1
IFS=$OIFS
#vypocet priemernej velkosti
if [ $NUM_OF_FILES -ne 0 ] ; then
	average_size=0
	for size in $SIZES_OF_FILES_ARRAY ; do
		average_size=$(( $average_size + $size ))
	done
	average_size=$(( $average_size / $NUM_OF_FILES ))

	#zoradenie
	SIZES_OF_FILES_ARRAY=`echo $SIZES_OF_FILES_ARRAY | tr ' ' '\n' | sort -n`
	#vypocet medianu
	middle_size=$(($NUM_OF_FILES / 2))

	if [ "$(($NUM_OF_FILES % 2))" -eq "1" ] ; then
		middle_size=$(( $middle_size + 1 ))
		median=`echo $SIZES_OF_FILES_ARRAY | cut -d' ' -f $middle_size`
	else
		median=`echo $SIZES_OF_FILES_ARRAY | cut -d' ' -f $middle_size`
		middle_size=$(( $middle_size + 1 ))
		median_next=`echo $SIZES_OF_FILES_ARRAY | cut -d' ' -f $middle_size`
		
		median=$(( $median + $median_next ))
		median=$(( $median / 2))
	fi
fi

#ak su nezname vypise N\A
if [ $NUM_OF_FILES -eq 0 ] ; then
	MAX_SIZE_FILE="N/A"
	average_size="N/A"
	median="N/A"
fi

if [ $MAX_SIZE_FILE -eq 0 ] ; then 
	MAX_SIZE_FILE="N/A"
fi



#vystup
echo "Root directory: $DIR"		#skumany adresar
echo "Directories: $NUM_OF_DIRS"	#pocet adresarov
echo "Max depth: $MAX_NESTED_FILE"	#maximalne zanorenie
echo "Average no. of files: $(( $NUM_OF_FILES / $NUM_OF_DIRS ))"
echo "All files: $NUM_OF_FILES"
echo "  Largest file: $MAX_SIZE_FILE"
echo "  Average file size: $average_size"
echo "  File size median: $median"

IFS='
'
get_extensions

if [ "$EXTENSIONS" != "" ] ; then
	echo "File extensions: $EXTENSIONS"
	EXTENSIONS=`echo $EXTENSIONS | tr ',' '\n'`
	for ext in `echo $EXTENSION_STRING | tr '&' '\n' | cut -d: -f1 |cut -d'/' -f2 |sort ` ; do
		print_ext "$ext"

	done

else
	echo "File extensions:"
fi


IFS=$OIFS

exit 0


