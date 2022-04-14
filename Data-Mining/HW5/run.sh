clear

echo "---------- Start Test Task1 ----------"
python3 task1.py ./data/users.txt 100 30 ./output/task1_out.txt
echo "---------- End Test Task1 ----------"

echo "---------- Start Test Task2 ----------"
python3 task2.py ./data/users.txt 300 30 ./output/task2.txt
echo "---------- End Test Task2 ----------"


echo "---------- Start Test Task3 ----------"
python3 task3.py ./data/users.txt 100 30 ./output/task3.txt
echo "---------- End Test Task3 ----------"
