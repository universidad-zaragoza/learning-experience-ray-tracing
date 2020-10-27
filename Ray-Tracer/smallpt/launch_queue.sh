
width=1024
height=768

echo "scene, width, height, threads, samples, divisions, time (s)"

for scene in sky nightsky island vista overlap wada wada2 forest
do
    for threads in 2 4 8
    do
        for samples in 2 4
        do
            for divisions in 1 2 4 8 16 32 64 128
            do
                if [ ${divisions} -lt ${threads} ]
                then
                    continue
                fi
                output_file="./images/queue_${scene}_${width}_${height}_${threads}_${samples}_${divisions}.png"
                ex_time=$(./smallpt_queue -scene "${scene}" -width "${width}" -height "${height}" -threads "${threads}" -samples "${samples}" -divisions ${divisions} -output "${output_file}" 2> /dev/null |  awk  '$1 ~ /Execution/ { print substr($3, 1, length($3)-1)}')
                echo "${scene}, ${width}, ${height}, ${threads}, ${samples}, ${divisions}, ${ex_time}"
            done
        done
    done
done # scene

exit 0
