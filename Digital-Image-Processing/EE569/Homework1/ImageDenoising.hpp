//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  ImageDenoising.hpp
//  EE569
//
//  Created by Alex on 2019/1/9.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef ImageDenoising_hpp
#define ImageDenoising_hpp

#include "HW1Basic.hpp"

//get noise by comparing with noise free image
Vec3_ui8 getNoise(C_Vec3_ui8& Im3D, C_Vec3_ui8& NoiseImageData);

//# Unifrom Filter
//# default use zero padding
//# gray and color image
Vec3_ui8 UniformFilter(C_Vec3_ui8& Im3D, C_i& kernel_size);

//# Gaussian Filter
//# default use zero padding
//# gray and color image
Vec3_ui8 GaussianFilter(C_Vec3_ui8& Im3D, C_i& kernel_size, C_d& sigma);

//# kernel_size: size of the core, neighbour considered
//# sigmaD: sigma for closeness, (sigma in noraml Gaussian core)
//# sigmaR: sigma for gray or color distance, simality distance core
//# default use zero padding
//# gray and color image
Vec3_ui8 BilateralFilter(C_Vec3_ui8& Im3D, C_i& kernel_size, C_d& sigmaD, C_d& sigmaR);

//search window
//reflect padding inside
Vec3_ui8 NLMeansFilter(C_Vec3_ui8& Im3D, C_d& sigma, C_i& search_window, C_i& compare_window);
Vec3_d NLMeansFilter(C_Vec3_d& Im3D, C_d& sigma, C_i& search_window, C_i& compare_window);

//shot noise denoising
Vec3_ui8 ShotNoise(C_Vec3_ui8& Im3D, C_i& kernel_size, C_d& sigma);

//NLMeans filter using gaussian weight to blur
Vec3_ui8 NLMeansFilterG(C_Vec3_ui8& Im3D, C_d& sigma, C_d& sigmaG, C_i& search_window, C_i& compare_window);

#endif /* ImageDenoising_hpp */
