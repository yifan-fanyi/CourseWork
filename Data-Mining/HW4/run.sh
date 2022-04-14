clear

echo "---------- Start Test Task1 ----------"
spark-submit --packages graphframes:graphframes:0.8.2-spark3.1-s_2.12 task1.py 7 ./data/ub_sample_data.csv ./output/task1_out.txt
echo "---------- End Test Task1 ----------"

echo "---------- Start Test Task2 ----------"
spark-submit  task2.py 7 ./data/ub_sample_data.csv ./output/task2_1.txt ./output/task2_2.txt
echo "---------- End Test Task2 ----------"
