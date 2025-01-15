#!/bin/bash


traverse()
{
    if [ -d "$1" ]
    then 
        for j in "$1"/*
        do
            traverse "$j" $2
        done
    elif [ -f "$1" ]
    then
        
        y1="$1"
        x1=${y1: -2:2}
        

        if [ $x1 = ".c" ]
        then
            mkdir $destination/C/$2
            mv "$1" "$destination/C/$2/main.c"
            if [ $organize_flag -ne 0 ]
            then
                echo "Organizing files of $2"
            fi

            if [ $execution_show_flag -ne 0 ]
            then
                echo "Executing files of $2"
            fi

            if [ $execute_flag -ne 0 ]
            then
                
                gcc "$destination/C/$2/main.c" -o "$destination/C/$2/main.out"
                k=0
                match=0
                not_match=0

                for j1 in "$testcase"/*
                do
                    k=`expr $k + 1`
                    touch "$destination/C/$2/out$k.txt"
                    ./"$destination/C/$2/main.out" < $j1 > "$destination/C/$2/out$k.txt"

                    t1=`diff "$destination/C/$2/out$k.txt" "$answer/ans$k.txt"`

                    if [ "$t1" ]
                    then
                        not_match=`expr $not_match + 1`
                    
                    else
                        match=`expr $match + 1`
                    fi



                done    

                echo "$2,C,$match,$not_match" >> "$destination/result.csv" 

            fi

        fi

        x2=${y1: -5:5}
        

        if [ "$x2" = ".java" ]
        then
            mkdir $destination/Java/$2
            
            mv "$1" "$destination/Java/$2/Main.java"
            if [ $organize_flag -ne 0 ]
            then
                echo "Organizing files of $2"
            fi

            if [ $execution_show_flag -ne 0 ]
            then
                echo "Executing files of $2"
            fi

            if [ $execute_flag -ne 0 ]
            then
                
                javac "$destination/Java/$2/Main.java"
                k=0
                match=0
                not_match=0

                for j1 in "$testcase"/*
                do
                    k=`expr $k + 1`
                    touch "$destination/Java/$2/out$k.txt"
                    java -cp "$destination/Java/$2" Main < $j1 > "$destination/Java/$2/out$k.txt"

                    t1=`diff "$destination/Java/$2/out$k.txt" "$answer/ans$k.txt"`

                    if [ "$t1" ]
                    then
                        not_match=`expr $not_match + 1`
                    
                    else
                        match=`expr $match + 1`
                    fi


                done    

                echo "$2,Java,$match,$not_match" >> "$destination/result.csv"

            fi
        fi

        x3=${y1: -3:3}
      

        if [ $x3 = ".py" ]
        then
            mkdir $destination/Python/$2
            
            mv "$1" "$destination/Python/$2/main.py"
            if [ $organize_flag -ne 0 ]
            then
                echo "Organizing files of $2"
            fi

            if [ $execution_show_flag -ne 0 ]
            then
                echo "Executing files of $2"
            fi
            
            if [ $execute_flag -ne 0 ]
            then
                
                k=0
                match=0
                not_match=0

                for j1 in "$testcase"/*
                do
                    k=`expr $k + 1`
                    touch "$destination/Python/$2/out$k.txt"
                    python3 "$destination/Python/$2/main.py" < $j1 > "$destination/Python/$2/out$k.txt"

                    t1=`diff "$destination/Python/$2/out$k.txt" "$answer/ans$k.txt"`

                    if [ "$t1" ]
                    then
                        not_match=`expr $not_match + 1`
                    
                    else
                        match=`expr $match + 1`
                    fi



                done    

                echo "$2,Python,$match,$not_match" >> "$destination/result.csv"

            fi    




        fi


    fi


}

organize_flag=1
execute_flag=1
execution_show_flag=1


if [ $# -lt 4 ]
then
    echo "Usage:"
    echo "./organize.sh <submission folder> <target folder> <test folder> <answer folder> [-v] [-noexecute]"
    echo ""
    echo "-v: verbose"
    echo "-noexecute: do not execute code files"
    echo ""
    exit

elif [ $# -eq 6 ]
then
    organize_flag=1
    execute_flag=0
    execution_show_flag=0

elif [ $# -eq 5 ]
then
    if [ "$5" = "-v" ]
    then
        organize_flag=1
        execute_flag=1
        execution_show_flag=1
    else
        organize_flag=0
        execute_flag=0
        execution_show_flag=0
    fi

elif [ $# -eq 4 ]
then
    organize_flag=0
    execute_flag=1
    execution_show_flag=0


fi



source="$1"

testcase="$3"
answer="$4"

if [ -d "$2" ]
then
    destination="$2"
else
    destination="$2"
    mkdir $destination
fi


count_1=0

for j in "$testcase"/*
do
    count_1=`expr $count_1 + 1`
done

if [ $organize_flag -ne 0 ]
then
    echo "Found $count_1 test files"
fi



mkdir new_tempory
mkdir $destination/C
mkdir $destination/Java
mkdir $destination/Python

if [ $execute_flag -ne 0 ]
then
    echo "student_id,type,match,not_match" >> "$destination/result.csv"
fi


for file in "$source"/*
do
    #echo "$file"
	x=${file: -11:7}
    #echo $x
    mkdir new_tempory/$x
    unzip -qq -d new_tempory/$x "$file"
    traverse new_tempory/$x $x

done












rm -R new_tempory

