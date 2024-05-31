#!/bin/bash

# Percorso alla cartella dove si trovano i test
# Inserire qui l'eventuale percorso dei test
test_folder=""

# Ciclo da 1 a 111 per eseguire il programma e confrontare gli output
for ((i=1; i<112; i++))
do
    input_file="open_${i}.txt"
    expected_output_file="open_${i}.output.txt"
    program_output="output.txt"

    # Esegui il programma C e salva l'output su un file temporaneo
    start_time=$(date +%s.%N)
    ./Aut2 <"$input_file" >"$program_output" 2> /dev/null
    exit_status=$?
    end_time=$(date +%s.%N)
    execution_time=$(echo "($end_time - $start_time) * 1000 / 1" | bc)

    if [ $exit_status -eq 139 ]; then
        echo "Test ${i}: Segmentation Fault"
    else
        # Confronta l'output del programma con l'output atteso
        diff_result=$(diff "$program_output" "$expected_output_file")

        if [ -z "$diff_result" ]; then
            echo "Test ${i}: OK (${execution_time} ms)"
        else
            echo "Test ${i}: FAILED (${execution_time} ms)"
        fi
    fi

    # Rimuovi il file temporaneo dell'output del programma
    # rm "$program_output"
done

