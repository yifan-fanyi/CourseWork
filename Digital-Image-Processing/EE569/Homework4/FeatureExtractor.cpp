//
//  FeatureExtractor.cpp
//  EE569
//
//  Created by Alex on 2019/3/6.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "FeatureExtractor.hpp"

void SelectGoodMatch(vector<DMatch>& matches, vector<DMatch>& good_matches, C_d& ndist){
    double minDist(INT_MAX);
    for(int i(0); i < Dim1(matches); i++ ){
        double dist = matches[i].distance;
        if(dist < minDist){
            minDist = dist;}
    }
    for(int i(0); i < Dim1(matches); i++){
        if(matches[i].distance <= ndist*minDist){
            good_matches.push_back(matches[i]);
        }
    }
}

vector<KeyPoint> SelectKeyPointOct(vector<KeyPoint> KPs, C_i& Octave){
    vector<KeyPoint> res;
    for(int i(0); i < Dim1(KPs); ++i){
        string s = to_string(KPs[i].octave);
        if(KPs[i].size < Octave){
            res.push_back(KPs[i]);
        }
    }
    return res;
}

void useSIFT(string nImA, string nImB, double ndist){
    Ptr<Feature2D>detector= xfeatures2d::SIFT::create();
    
    Mat ImA = imread(str2pchar(nImA+".png"), IMREAD_COLOR);
    Mat ImB = imread(str2pchar(nImB+".png"), IMREAD_COLOR);
    
    vector<KeyPoint> keypointsA, keypointsB;
    detector->detect(ImA, keypointsA);
    detector->detect(ImB, keypointsB);
    
    Mat descriptorsA, descriptorsB;
    detector->compute(ImA, keypointsA, descriptorsA );
    detector->compute(ImB, keypointsB, descriptorsB );
    
    Mat img_keypointsA,img_keypointsB;
    drawKeypoints(ImA,keypointsA,img_keypointsA,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(ImB,keypointsB,img_keypointsB,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    imwrite(str2pchar(nImA+"_SIFT.jpg"), img_keypointsA);
    imwrite(str2pchar(nImB+"_SIFT.jpg"), img_keypointsB);
    
    int oct = 2;
    vector<KeyPoint> keypointsA1,keypointsB1;
    keypointsA1 = SelectKeyPointOct(keypointsA, oct);
    keypointsB1 = SelectKeyPointOct(keypointsB, oct);
    cout<<keypointsA1.size()<<"   "<<keypointsB1.size()<<endl;
    drawKeypoints(ImA,keypointsA1,img_keypointsA,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(ImB,keypointsB1,img_keypointsB,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    imwrite(str2pchar(nImA+"_oct_"+to_string(oct)+"_SIFT.jpg"), img_keypointsA);
    imwrite(str2pchar(nImB+"_oct_"+to_string(oct)+"_SIFT.jpg"), img_keypointsB);
    
    std::vector<DMatch> matches, good_matches;
    BFMatcher matcher;
    matcher.match(descriptorsA, descriptorsB, matches);
    SelectGoodMatch(matches, good_matches, ndist);

    Mat img_match;
    Mat img_goodmatch;
    drawMatches(ImA, keypointsA, ImB, keypointsB, matches, img_match);
    drawMatches(ImA, keypointsA, ImB, keypointsB, good_matches, img_goodmatch);
    imwrite(str2pchar(nImA+"SIFT_allmatch.jpg"), img_match);
    imwrite(str2pchar(nImA+"SIFT_allmatch.jpg"), img_goodmatch);
}

void useORB(string nImA, string nImB, double ndist){
    Mat ImA = imread(str2pchar(nImA+".png"), IMREAD_COLOR);
    Mat ImB = imread(str2pchar(nImB+".png"), IMREAD_COLOR);
    
    std::vector<KeyPoint> keypointsA, keypointsB;
    Mat descriptorsA, descriptorsB;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    
    detector->detect(ImA,keypointsA );
    detector->detect(ImB,keypointsB );
    
    descriptor->compute(ImA, keypointsA, descriptorsA);
    descriptor->compute(ImB, keypointsB, descriptorsB);
    
    Mat img_keypointsA, img_keypointsB;
    drawKeypoints(ImA, keypointsA, img_keypointsA, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    drawKeypoints(ImB, keypointsB, img_keypointsB, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imwrite(str2pchar(nImA+"_ORB.jpg"), img_keypointsA);
    imwrite(str2pchar(nImB+"_ORB.jpg"), img_keypointsB);
    
    int oct = 1;
    vector<KeyPoint> keypointsA1,keypointsB1;
    keypointsA1 = SelectKeyPointOct(keypointsA, oct);
    keypointsB1 = SelectKeyPointOct(keypointsB, oct);
    cout<<keypointsA1.size()<<"   "<<keypointsB1.size()<<endl;
    drawKeypoints(ImA,keypointsA1,img_keypointsA,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(ImB,keypointsB1,img_keypointsB,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    imwrite(str2pchar(nImA+"_oct_"+to_string(oct)+"_ORB.jpg"), img_keypointsA);
    imwrite(str2pchar(nImB+"_oct_"+to_string(oct)+"_ORB.jpg"), img_keypointsB);
    
    std::vector< DMatch > matches, good_matches;
    matcher->match(descriptorsA, descriptorsB, matches );
    SelectGoodMatch(matches, good_matches, ndist);
    
    Mat img_match, img_goodmatch;
    drawMatches(ImA, keypointsA, ImB, keypointsB, matches, img_match);
    drawMatches(ImA, keypointsA, ImB, keypointsB, good_matches, img_goodmatch);
    imwrite(str2pchar(nImA+"ORB_allmatch.jpg"), img_match);
    imwrite(str2pchar(nImA+"ORB_allmatch.jpg"), img_goodmatch);
}

void useSURF(string nImA, string nImB, double ndist){
    Ptr<Feature2D>detector= xfeatures2d::SURF::create();

    Mat ImA = imread(str2pchar(nImA+".png"), IMREAD_COLOR);
    Mat ImB = imread(str2pchar(nImB+".png"), IMREAD_COLOR);
    
    vector<KeyPoint> keypointsA,keypointsB;
    detector->detect(ImA, keypointsA);
    detector->detect(ImB, keypointsB);
    
    Mat descriptorsA,descriptorsB;
    detector->compute(ImA, keypointsA, descriptorsA);
    detector->compute(ImB, keypointsB, descriptorsB);
    
    Mat img_keypointsA,img_keypointsB;
    drawKeypoints(ImA,keypointsA,img_keypointsA,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    drawKeypoints(ImB,keypointsB,img_keypointsB,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    imwrite(str2pchar(nImA+"_SURF.jpg"), img_keypointsA);
    imwrite(str2pchar(nImB+"_SURF.jpg"), img_keypointsB);
    
    std::vector< DMatch > matches, good_matches;
    BFMatcher matcher;
    matcher.match( descriptorsA, descriptorsB, matches );
    SelectGoodMatch(matches, good_matches, ndist);
    
    Mat img_match, img_goodmatch;
    drawMatches(ImA, keypointsA, ImB, keypointsB, matches, img_match);
    drawMatches(ImA, keypointsA, ImB, keypointsB, good_matches, img_goodmatch);
    imwrite(str2pchar(nImA+"SURF_allmatch.jpg"), img_match);
    imwrite(str2pchar(nImA+"SURF_allmatch.jpg"), img_goodmatch);
}

void DetectSIFT(vector<vector<KeyPoint>>& KPs, vector<Mat>& DPs){
    Ptr<Feature2D>detector= xfeatures2d::SIFT::create();
    Vec1_s name({"zero", "one"});
    for(int j(0); j < Dim1(name); ++j){
        for(int i(1); i <= 5; ++i){
            string name1 = name[j]+"_"+to_string(i)+".png";
            Mat Img = imread(str2pchar(name1), CV_8U);
            vector<KeyPoint> keypoints;
            Mat descriptors;
            detector->detect(Img, keypoints);
            detector->compute(Img, keypoints, descriptors);
            KPs.push_back(keypoints);
            DPs.push_back(descriptors);
        }
    }
}

void KMeansOpencV(Mat& data, Mat& label, Mat& center, C_i& K){
    const int attemps{ 100 };
    const cv::TermCriteria term_criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 100, 0.01);
    cv::kmeans(data, K, label, term_criteria, attemps, cv::KMEANS_RANDOM_CENTERS, center);
}

void useBagOfWords(){
    vector<vector<KeyPoint>> KPs;
    vector<Mat> DPs;
    DetectSIFT(KPs, DPs);
    Mat cDPs = DPs[0].clone();
    for(int i(1); i < Dim1(DPs); ++i){
        //Mat tmp;
        vconcat(cDPs, DPs[i], cDPs);
     //   tmp.copyTo(cDPs);
    }
    Mat label, center;
    KMeansOpencV(cDPs, label, center, 2);
    Ptr<Feature2D>detector= xfeatures2d::SIFT::create();
    string name = "zero_3.png";
    Mat testImg = imread(str2pchar(name), CV_8U);
    vector<KeyPoint> testKP;
    detector->detect(testImg, testKP);
    Mat testDP;
    detector->compute(testImg, testKP, testDP);
    
    Mat img_keypoints;
    drawKeypoints(testImg,testKP,img_keypoints,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    imwrite(str2pchar("KP_"+name), img_keypoints);
  
    
    Vec1_i bin(2,0);
    vector<KeyPoint> bin0;
    vector<KeyPoint> bin1;
    for(int i(0); i < testDP.rows; ++i){
        double mdist(INT_MAX);
        int label(0);
        for(int j(0); j < center.rows; ++j){
            double dist(0);
            for(int p(0); p < center.cols; ++p){
                dist += Sq(testDP.at<float>(i,p)-center.at<float>(j,p));
            }
            if(dist < mdist){
                mdist = dist;
                label = j;
            }
        }
        if(label == 1){
            bin1.push_back(testKP[i]);
        }
        else{
            bin0.push_back(testKP[i]);
        }
        ++bin[label];
    }
    drawKeypoints(testImg,bin0,img_keypoints,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    imwrite(str2pchar("KP_vote0_"+name), img_keypoints);
    drawKeypoints(testImg,bin1,img_keypoints,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    imwrite(str2pchar("KP_vote1_"+name), img_keypoints);
    cout<<bin[0]<<"  "<<bin[1]<<endl;
}
