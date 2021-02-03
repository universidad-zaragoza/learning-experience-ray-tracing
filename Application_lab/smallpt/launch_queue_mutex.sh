
width=4096
height=2160

out_dir="/tmp/$(whoami)/images"

mkdir -p "${out_dir}" 
echo "mutex, scene, width, height, threads, samples, divisions, time (s)"

#for mutex in def K n s
for mutex in asm asm_sleep
do
for scene in forest
do
    for threads in 4 8 32 64
    do
        for samples in 1
        do
            for divisions in 32 64 128
            do
                if [ ${divisions} -lt ${threads} ]
                then
                    continue
                fi
                output_file="${out_dir}/queue_mutex_${mutex}_${scene}_${width}_${height}_${threads}_${samples}_${divisions}.png"
                ex_time=$(./smallpt_queue_mutex_${mutex} -scene "${scene}" -width "${width}" -height "${height}" -threads "${threads}" -samples "${samples}" -divisions ${divisions} -output "${output_file}" 2> /dev/null |  awk  '$1 ~ /Execution/ { print substr($3, 1, length($3)-1)}')
                echo "${mutex}, ${scene}, ${width}, ${height}, ${threads}, ${samples}, ${divisions}, ${ex_time}"
            done
        done
    done
done # scene
done #mutex
exit 0
