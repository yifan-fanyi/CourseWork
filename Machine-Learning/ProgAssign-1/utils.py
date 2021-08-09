import numpy as np
from knn import KNN

############################################################################
# DO NOT MODIFY ABOVE CODES
############################################################################


# TODO: implement F1 score
def f1_score(real_labels, predicted_labels):
    """
    Information on F1 score - https://en.wikipedia.org/wiki/F1_score
    :param real_labels: List[int]
    :param predicted_labels: List[int]
    :return: float
    """
    assert len(real_labels) == len(predicted_labels)
    real_labels, predicted_labels = np.array(real_labels), np.array(predicted_labels)
    numerator   = 2 * np.sum(real_labels * predicted_labels)
    denominator = np.sum(real_labels) + np.sum(predicted_labels) 
    if denominator == 0:
        return 0
    return numerator / denominator
    raise NotImplementedError


class Distances:
    @staticmethod
    # TODO
    def canberra_distance(point1, point2):
        """
        :param point1: List[float]
        :param point2: List[float]
        :return: float
        """
        dist = 0
        for i in range(len(point1)):
            if np.abs(point1[i]) + np.abs(point2[i]) != 0:
                dist += np.abs(point1[i]-point2[i]) / (np.abs(point1[i]) + np.abs(point2[i]))
        return dist
        raise NotImplementedError

    @staticmethod
    # TODO
    def minkowski_distance(point1, point2):
        """
        Minkowski distance is the generalized version of Euclidean Distance
        It is also know as L-p norm (where p>=1) that you have studied in class
        For our assignment we need to take p=3
        Information on Minkowski distance - https://en.wikipedia.org/wiki/Minkowski_distance
        :param point1: List[float]
        :param point2: List[float]
        :return: float
        """
        point1, point2 = np.array(point1), np.array(point2)
        return np.power(np.sum(np.power(np.abs(point1-point2), 3)), 1/3)
        raise NotImplementedError

    @staticmethod
    # TODO
    def euclidean_distance(point1, point2):
        """
        :param point1: List[float]
        :param point2: List[float]
        :return: float
        """
        point1, point2 = np.array(point1), np.array(point2)
        return np.power(np.sum(np.power(np.abs(point1-point2), 2)), 1/2)
        raise NotImplementedError

    @staticmethod
    # TODO
    def inner_product_distance(point1, point2):
        """
        :param point1: List[float]
        :param point2: List[float]
        :return: float
        """
        point1, point2 = np.array(point1), np.array(point2)
        return np.inner(point1, point2)
        raise NotImplementedError

    @staticmethod
    # TODO
    def cosine_similarity_distance(point1, point2):
        """
       :param point1: List[float]
       :param point2: List[float]
       :return: float
       """
        point1, point2 = np.array(point1), np.array(point2)
        if (np.sqrt(np.sum(np.square(point1))) * np.sqrt(np.sum(np.square(point2)))) < 1e-20:
            return 1.
        return 1. - np.inner(point1, point2) / (np.sqrt(np.sum(np.square(point1))) * np.sqrt(np.sum(np.square(point2))))
        raise NotImplementedError

    @staticmethod
    # TODO
    def gaussian_kernel_distance(point1, point2):
        """
       :param point1: List[float]
       :param point2: List[float]
       :return: float
       """
        point1, point2 = np.array(point1), np.array(point2)
        return -np.exp(-0.5 * np.inner(point1-point2, point1-point2))
        raise NotImplementedError


class HyperparameterTuner:
    def __init__(self):
        self.best_k = None
        self.best_distance_function = None
        self.best_scaler = None
        self.best_model = None

    # TODO: find parameters with the best f1 score on validation dataset
    def tuning_without_scaling(self, distance_funcs, x_train, y_train, x_val, y_val):
        """
        In this part, you should try different distance function you implemented in part 1.1, and find the best k.
        Use k range from 1 to 30 and increment by 2. Use f1-score to compare different models.

        :param distance_funcs: dictionary of distance functions you must use to calculate the distance.
            Make sure you loop over all distance functions for each data point and each k value.
            You can refer to test.py file to see the format in which these functions will be
            passed by the grading script
        :param x_train: List[List[int]] training data set to train your KNN model
        :param y_train: List[int] train labels to train your KNN model
        :param x_val:  List[List[int]] Validation data set will be used on your KNN predict function to produce
            predicted labels and tune k and distance function.
        :param y_val: List[int] validation labels

        Find(tune) best k, distance_function and model (an instance of KNN) and assign to self.best_k,
        self.best_distance_function and self.best_model respectively.
        NOTE: self.best_scaler will be None

        NOTE: When there is a tie, choose model based on the following priorities:
        Then check distance function  [canberra > minkowski > euclidean > gaussian > inner_prod > cosine_dist]
        If they have same distance fuction, choose model which has a less k.
        """
        prior = {'canberra':0, 'minkowski':1, 'euclidean':2, 'gaussian':3, 'inner_prod':4, 'cosine_dist':5}
        bestF1 = -1.
        for k in range(1, 30, 2):
            for distFuncName in distance_funcs.keys():
                knn = KNN(k=k, distance_function=distance_funcs[distFuncName])
                knn.train(x_train, y_train)
                pY = knn.predict(x_val)
                f1 = f1_score(y_val, pY)
                if f1 > bestF1:
                    self.best_k = k
                    self.best_distance_function = distFuncName
                    self.best_model = knn
                    bestF1 = f1
                    #print("updateN F1: k=%2d, bestF1=%1.4f, func=%11s"%(self.best_k, bestF1, self.best_distance_function))
                if abs(f1 - bestF1) < 1e-20:
                    if (self.best_distance_function is None) or (prior[self.best_distance_function] > prior[distFuncName]):
                        self.best_k = k
                        self.best_distance_function = distFuncName
                        self.best_model = knn
                        bestF1 = f1
                        #print("updateP F1: k=%3d, bestF1=%1.4f, func=%11s"%(self.best_k, bestF1, self.best_distance_function))
        return self
        raise NotImplementedError

    # TODO: find parameters with the best f1 score on validation dataset, with normalized data
    def tuning_with_scaling(self, distance_funcs, scaling_classes, x_train, y_train, x_val, y_val):
        """
        This part is similar to Part 1.3 except that before passing your training and validation data to KNN model to
        tune k and disrance function, you need to create the normalized data using these two scalers to transform your
        data, both training and validation. Again, we will use f1-score to compare different models.
        Here we have 3 hyperparameters i.e. k, distance_function and scaler.

        :param distance_funcs: dictionary of distance funtions you use to calculate the distance. Make sure you
            loop over all distance function for each data point and each k value.
            You can refer to test.py file to see the format in which these functions will be
            passed by the grading script
        :param scaling_classes: dictionary of scalers you will use to normalized your data.
        Refer to test.py file to check the format.
        :param x_train: List[List[int]] training data set to train your KNN model
        :param y_train: List[int] train labels to train your KNN model
        :param x_val: List[List[int]] validation data set you will use on your KNN predict function to produce predicted
            labels and tune your k, distance function and scaler.
        :param y_val: List[int] validation labels

        Find(tune) best k, distance_funtion, scaler and model (an instance of KNN) and assign to self.best_k,
        self.best_distance_function, self.best_scaler and self.best_model respectively

        NOTE: When there is a tie, choose model based on the following priorities:
        For normalization, [min_max_scale > normalize];
        Then check distance function  [canberra > minkowski > euclidean > gaussian > inner_prod > cosine_dist]
        If they have same distance function, choose model which has a less k.
        """
        bestF1 = -1.
        prior = {'canberra':0, 'minkowski':1, 'euclidean':2, 'gaussian':3, 'inner_prod':4, 'cosine_dist':5, 
                 'min_max_scale':0, 'normalize':1}
        for scName in scaling_classes.keys():
            SC = scaling_classes[scName]()
            x_train_sc = SC(x_train)
            x_val_sc = SC(x_val)
            for k in range(1, 30, 2):
                for distFuncName in distance_funcs.keys():
                    knn = KNN(k=k, distance_function=distance_funcs[distFuncName])
                    knn.train(x_train_sc, y_train)
                    pY = knn.predict(x_val_sc)
                    f1 = f1_score(y_val, pY)
                    if f1 > bestF1:
                        self.best_k = k
                        self.best_distance_function = distFuncName
                        self.best_model = knn
                        self.best_scaler = scName
                        bestF1 = f1
                        #print("updateN F1: %13s, k=%3d, bestF1=%1.4f, func=%11s"%(self.best_scaler, self.best_k, bestF1, self.best_distance_function))
                    if abs(f1 - bestF1) < 1e-20:
                        if (self.best_scaler is None) or (prior[self.best_scaler] > prior[scName]) or (self.best_distance_function is None) or (prior[self.best_distance_function] > prior[distFuncName]):
                            self.best_k = k
                            self.best_distance_function = distFuncName
                            self.best_model = knn
                            self.best_scaler = scName
                            #print("updateP F1: %13s, k=%3d, bestF1=%1.4f, func=%11s"%(self.best_scaler, self.best_k, bestF1, self.best_distance_function))
        return self
        raise NotImplementedError


class NormalizationScaler:
    def __init__(self):
        pass

    # TODO: normalize data
    def __call__(self, features):
        """
        Normalize features for every sample

        Example
        features = [[3, 4], [1, -1], [0, 0]]
        return [[0.6, 0.8], [0.707107, -0.707107], [0, 0]]

        :param features: List[List[float]]
        :return: List[List[float]]
        """
        normalized = []
        for fea in features:
            norm = float(np.sqrt(np.inner(fea, fea)))
            if norm > 1e-20:
                tmp = []
                for i in range(len(fea)):
                    tmp.append(fea[i] / norm)
                normalized.append(tmp)
            else:
                normalized.append(fea)
        return normalized
        raise NotImplementedError


class MinMaxScaler:
    """
    Please follow this link to know more about min max scaling
    https://en.wikipedia.org/wiki/Feature_scaling
    You should keep some states inside the object.
    You can assume that the parameter of the first __call__
    will be the training set.

    Hints:
        1. Use a variable to check for first __call__ and only compute
            and store min/max in that case.

    Note:
        1. You may assume the parameters are valid when __call__
            is being called the first time (you can find min and max).

    Example:
        train_features = [[0, 10], [2, 0]]
        test_features = [[20, 1]]

        scaler1 = MinMaxScale()
        train_features_scaled = scaler1(train_features)
        # train_features_scaled should be equal to [[0, 1], [1, 0]]

        test_features_scaled = scaler1(test_features)
        # test_features_scaled should be equal to [[10, 0.1]]

        new_scaler = MinMaxScale() # creating a new scaler
        _ = new_scaler([[1, 1], [0, 0]]) # new trainfeatures
        test_features_scaled = new_scaler(test_features)
        # now test_features_scaled should be [[20, 1]]

    """

    def __init__(self):
        self.min = None
        self.max = None
        pass

    def __call__(self, features):
        """
        normalize the feature vector for each sample . For example,
        if the input features = [[2, -1], [-1, 5], [0, 0]],
        the output should be [[1, 0], [0, 1], [0.333333, 0.16667]]

        :param features: List[List[float]]
        :return: List[List[float]]
        """
        tmp = np.array(features)
        if self.min is None:
            self.min = np.min(tmp, axis=0)
            self.max = np.max(tmp, axis=0)
        for i in range(len(self.min)):
            if abs(self.max[i] - self.min[i]) < 1e-10:
                tmp[:, i] *= 0
            else:
                tmp[:, i] = (tmp[:, i] - self.min[i]) / (self.max[i] - self.min[i])
        return tmp.tolist()
        raise NotImplementedError


