clear

echo "---------- Start Test Task1 ----------"
spark-submit --executor-memory 4G --driver-memory 4G task1.py ./data/yelp_train.csv ./output/task1.txt
echo "---------- End Test Task1 ----------"

echo "---------- Start Test Task2_1 ----------"
spark-submit task2_1.py ./data/yelp_train.csv ./data/yelp_val_in.csv ./output/task2_1.txt
spark-submit evl.py ./data/yelp_val.csv ./output/task2_1.txt
echo "---------- End Test Task2_1 ----------"

echo "---------- Start Test Task2_2 ----------"
spark-submit task2_2.py ./data/ ./data/yelp_val_in.csv ./output/task2_2.txt
spark-submit evl.py ./data/yelp_val.csv ./output/task2_2.txt
echo "---------- End Test Task2_2 ----------"

echo "---------- Start Test Task2_3 ----------"
spark-submit task2_3.py ./data/ ./data/yelp_val_in.csv ./output/task2_3.txt
spark-submit evl.py ./data/yelp_val.csv ./output/task2_3.txt
echo "---------- End Test Task2_3 ----------"

