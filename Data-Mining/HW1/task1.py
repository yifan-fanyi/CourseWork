# @yifan
# 2022.01.26
import sys
import json
from operator import add
from pyspark import SparkContext

class Task1:
    def total_number_of_reviews(rdd):
        return rdd.count()
    
    def number_of_reviews_in_2018(rdd):
        return rdd.filter(lambda x: x[:4] == '2018').count()
        
    def number_of_distinct_users_who_wrote_reviews(rdd):
        return rdd.distinct().count()

    def top_10_users_who_wrote_largest_numbers_of_reviews_and_number_of_reviews_they_wrote(rdd):
        return rdd.map(lambda x: (x, 1)).reduceByKey(add).takeOrdered(10, key=lambda x: (-x[1], x[0]))

    def number_of_distinct_businesses_have_been_reviewed(rdd):
        return rdd.distinct().count()

    def top_10_businesses_had_largest_numbers_of_reviews_and_number_of_reviews_they_had(rdd):
        return rdd.map(lambda x: (x, 1)).reduceByKey(add).takeOrdered(10, key=lambda x: (-x[1], x[0]))

if __name__ == '__main__':
    mySC = SparkContext.getOrCreate()
    save = {}
    lines = mySC.textFile(sys.argv[1]).map(lambda line: json.loads(line))
    save['n_review']       = Task1.total_number_of_reviews(
                                lines.map(lambda x: x['review_id']))
    save['n_review_2018']  = Task1.number_of_reviews_in_2018(
                                lines.map(lambda x: x['date']))
    save['n_user']         = Task1.number_of_distinct_users_who_wrote_reviews(
                                lines.map(lambda x: x['user_id']))
    save['top10_user']     = Task1.top_10_users_who_wrote_largest_numbers_of_reviews_and_number_of_reviews_they_wrote(
                                lines.map(lambda x: x['user_id']))
    save['n_business']     = Task1.number_of_distinct_businesses_have_been_reviewed(
                                lines.map(lambda x: x['business_id']))
    save['top10_business'] = Task1.top_10_businesses_had_largest_numbers_of_reviews_and_number_of_reviews_they_had(
                                lines.map(lambda x: x['business_id']))

    with open(sys.argv[2], 'w') as f:
        json.dump(save, f)