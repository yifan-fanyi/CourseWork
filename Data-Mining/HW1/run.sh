
echo "---------- Start Test Task1 ----------"
spark-submit --executor-memory 4G --driver-memory 4G task1.py ./data/test_review.json ./result/task1_out.json
echo "---------- End Test Task1 ----------"

echo "---------- Start Test Task2 ----------"
spark-submit --executor-memory 4G --driver-memory 4G task2.py ./data/test_review.json ./result/task2_out.json 2
echo "---------- End Test Task2 ----------"

echo "---------- Start Test Task3 ----------"
spark-submit --executor-memory 4G --driver-memory 4G task3.py ./data/test_review.json ./data/business.json ./result/task3a_out.txt ./result/task3b_out.json
echo "---------- End Test Task3 ----------"

# on server
#export PYSPARK_PYTHON=python3.6
#/opt/spark/spark-3.1.2-bin-hadoop3.2/bin/spark-submit --executor-memory 4G --driver-memory 4G task1.py ../resource/asnlib/publicdata/test_review.json task1_out.json

#export PYSPARK_PYTHON=python3.6
#/opt/spark/spark-3.1.2-bin-hadoop3.2/bin/spark-submit --executor-memory 4G --driver-memory 4G task2.py ../resource/asnlib/publicdata/test_review.json task2_out.json 2

#export PYSPARK_PYTHON=python3.6
#/opt/spark/spark-3.1.2-bin-hadoop3.2/bin/spark-submit --executor-memory 4G --driver-memory 4G task3.py ../resource/asnlib/publicdata/test_review.json ../resource/asnlib/publicdata/business.json task3a_out.json task3b_out.json


