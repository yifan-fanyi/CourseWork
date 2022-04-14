clear

echo "---------- Start Test Task2_3 ----------"
spark-submit competition.py ./data/ ./data/yelp_val_in.csv ./output/competition.txt
spark-submit evl.py ./data/yelp_val.csv ./output/competition.txt
echo "---------- End Test Task2_3 ----------"

