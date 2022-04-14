
clear

echo "---------- Start Test Task1 ----------"
spark-submit --executor-memory 4G --driver-memory 4G task1.py 1 4 data/small2.csv output/task1_1.txt
echo "---------- End Test Task1 ----------"



echo "---------- Start Test Task2 ----------"
spark-submit task2.py 20 50 data/ta_feng_all_months_merged.csv output/task2.txt 
echo "---------- End Test Task2 ----------"
