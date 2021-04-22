//
//  HW3.h
//  CSCI576
//
//  Created by Alex on 2021/3/15.
//
// dct and idct are modified from libjpeg

#ifndef HW3_h
#define HW3_h
#include "HW1.h"

#define DCTSIZE 8
#define DCTSIZE2 64

const int zigzag[DCTSIZE2] = {0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,12,19,
                              26,33,40,48,41,34,27,20,13,6,7,14,21,28,35,42,
                              49,56,57,50,43,36,29,22,15,23,30,37,44,51,58,
                              59,52,45,38,31,39,46,53,60,61,54,47,55,62,63};

// 5/3 DWT
const double lowpass[] = {-0.125, 0.25, 0.75, 0.25, -0.125};
const double highpass[] = {-0.5, 1, -0.5};

void fdct_8x8(double* data){
  double tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
  double tmp10, tmp11, tmp12, tmp13;
  double z1, z2, z3, z4, z5, z11, z13;
  double *dataptr;
  int ctr;
  /* Pass 1: process rows. */
  dataptr = data;
  for (ctr = DCTSIZE-1; ctr >= 0; ctr--) {
    tmp0 = dataptr[0] + dataptr[7];
    tmp7 = dataptr[0] - dataptr[7];
    tmp1 = dataptr[1] + dataptr[6];
    tmp6 = dataptr[1] - dataptr[6];
    tmp2 = dataptr[2] + dataptr[5];
    tmp5 = dataptr[2] - dataptr[5];
    tmp3 = dataptr[3] + dataptr[4];
    tmp4 = dataptr[3] - dataptr[4];
    /* Even part */
    tmp10 = tmp0 + tmp3;    /* phase 2 */
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;
    dataptr[0] = tmp10 + tmp11; /* phase 3 */
    dataptr[4] = tmp10 - tmp11;
    z1 = (tmp12 + tmp13) * ((double) 0.707106781); /* c4 */
    dataptr[2] = tmp13 + z1;    /* phase 5 */
    dataptr[6] = tmp13 - z1;
    
    /* Odd part */
    tmp10 = tmp4 + tmp5;    /* phase 2 */
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;
    /* The rotator is modified from fig 4-8 to avoid extra negations. */
    z5 = (tmp10 - tmp12) * ((double) 0.382683433); /* c6 */
    z2 = ((double) 0.541196100) * tmp10 + z5; /* c2-c6 */
    z4 = ((double) 1.306562965) * tmp12 + z5; /* c2+c6 */
    z3 = tmp11 * ((double) 0.707106781); /* c4 */
    z11 = tmp7 + z3;        /* phase 5 */
    z13 = tmp7 - z3;
    dataptr[5] = z13 + z2;    /* phase 6 */
    dataptr[3] = z13 - z2;
    dataptr[1] = z11 + z4;
    dataptr[7] = z11 - z4;
    dataptr += DCTSIZE;        /* advance pointer to next row */
  }
  /* Pass 2: process columns. */
  dataptr = data;
  for (ctr = DCTSIZE-1; ctr >= 0; ctr--) {
    tmp0 = dataptr[DCTSIZE*0] + dataptr[DCTSIZE*7];
    tmp7 = dataptr[DCTSIZE*0] - dataptr[DCTSIZE*7];
    tmp1 = dataptr[DCTSIZE*1] + dataptr[DCTSIZE*6];
    tmp6 = dataptr[DCTSIZE*1] - dataptr[DCTSIZE*6];
    tmp2 = dataptr[DCTSIZE*2] + dataptr[DCTSIZE*5];
    tmp5 = dataptr[DCTSIZE*2] - dataptr[DCTSIZE*5];
    tmp3 = dataptr[DCTSIZE*3] + dataptr[DCTSIZE*4];
    tmp4 = dataptr[DCTSIZE*3] - dataptr[DCTSIZE*4];
    /* Even part */
    tmp10 = tmp0 + tmp3;    /* phase 2 */
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;
    dataptr[DCTSIZE*0] = (tmp10 + tmp11)/(double)(DCTSIZE); /* phase 3 */
    dataptr[DCTSIZE*4] = (tmp10 - tmp11)/(double)(DCTSIZE);
    z1 = (tmp12 + tmp13) * ((double) 0.707106781); /* c4 */
    dataptr[DCTSIZE*2] = (tmp13 + z1)/(double)(DCTSIZE); /* phase 5 */
    dataptr[DCTSIZE*6] = tmp13 - z1;
    /* Odd part */
    tmp10 = tmp4 + tmp5;    /* phase 2 */
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;
    /* The rotator is modified from fig 4-8 to avoid extra negations. */
    z5 = (tmp10 - tmp12) * ((double) 0.382683433); /* c6 */
    z2 = ((double) 0.541196100) * tmp10 + z5; /* c2-c6 */
    z4 = ((double) 1.306562965) * tmp12 + z5; /* c2+c6 */
    z3 = tmp11 * ((double) 0.707106781); /* c4 */
    z11 = tmp7 + z3;        /* phase 5 */
    z13 = tmp7 - z3;
    dataptr[DCTSIZE*5] = (z13 + z2)/(double)(DCTSIZE); /* phase 6 */
    dataptr[DCTSIZE*3] = (z13 - z2)/(double)(DCTSIZE);
    dataptr[DCTSIZE*1] = (z11 + z4)/(double)(DCTSIZE);
    dataptr[DCTSIZE*7] = (z11 - z4)/(double)(DCTSIZE);
    dataptr++;            /* advance pointer to next column */
  }
}

void idct_8x8(double* coef_block, double* output_buf){
    double tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    double tmp10, tmp11, tmp12, tmp13;
    double z5, z10, z11, z12, z13;
    double* inptr;
    double* outptr;
    double * wsptr;
    int ctr;
    double workspace[DCTSIZE2]; /* buffers data between passes */
    /* Pass 1: process columns from input, store into work array. */
    inptr = coef_block;
    wsptr = workspace;
    for (ctr = DCTSIZE; ctr > 0; ctr--) {
        /* Even part */
        tmp0 = inptr[DCTSIZE*0];
        tmp1 = inptr[DCTSIZE*2];
        tmp2 = inptr[DCTSIZE*4];
        tmp3 = inptr[DCTSIZE*6];
        tmp10 = tmp0 + tmp2;    /* phase 3 */
        tmp11 = tmp0 - tmp2;
        tmp13 = tmp1 + tmp3;    /* phases 5-3 */
        tmp12 = (tmp1 - tmp3) * ((double) 1.414213562) - tmp13; /* 2*c4 */
        tmp0 = tmp10 + tmp13;    /* phase 2 */
        tmp3 = tmp10 - tmp13;
        tmp1 = tmp11 + tmp12;
        tmp2 = tmp11 - tmp12;
        /* Odd part */
        tmp4 = inptr[DCTSIZE*1];
        tmp5 = inptr[DCTSIZE*3];
        tmp6 = inptr[DCTSIZE*5];
        tmp7 = inptr[DCTSIZE*7];
        z13 = tmp6 + tmp5;        /* phase 6 */
        z10 = tmp6 - tmp5;
        z11 = tmp4 + tmp7;
        z12 = tmp4 - tmp7;
        tmp7 = z11 + z13;        /* phase 5 */
        tmp11 = (z11 - z13) * ((double) 1.414213562); /* 2*c4 */
        z5 = (z10 + z12) * ((double) 1.847759065); /* 2*c2 */
        tmp10 = ((double) 1.082392200) * z12 - z5; /* 2*(c2-c6) */
        tmp12 = ((double) -2.613125930) * z10 + z5; /* -2*(c2+c6) */
        tmp6 = tmp12 - tmp7;    /* phase 2 */
        tmp5 = tmp11 - tmp6;
        tmp4 = tmp10 + tmp5;
        wsptr[DCTSIZE*0] = tmp0 + tmp7;
        wsptr[DCTSIZE*7] = tmp0 - tmp7;
        wsptr[DCTSIZE*1] = tmp1 + tmp6;
        wsptr[DCTSIZE*6] = tmp1 - tmp6;
        wsptr[DCTSIZE*2] = tmp2 + tmp5;
        wsptr[DCTSIZE*5] = tmp2 - tmp5;
        wsptr[DCTSIZE*4] = tmp3 + tmp4;
        wsptr[DCTSIZE*3] = tmp3 - tmp4;
        inptr++;            /* advance pointers to next column */
        wsptr++;
    }
               
    /* Pass 2: process rows from work array, store into output array. */
    /* Note that we must descale the results by a factor of 8 == 2**3. */
    wsptr = workspace;
    for (ctr = 0; ctr < DCTSIZE; ctr++) {
        outptr = &output_buf[ctr*DCTSIZE];
        /* Even part */
        tmp10 = wsptr[0] + wsptr[4];
        tmp11 = wsptr[0] - wsptr[4];
        tmp13 = wsptr[2] + wsptr[6];
        tmp12 = (wsptr[2] - wsptr[6]) * ((double) 1.414213562) - tmp13;
        tmp0 = tmp10 + tmp13;
        tmp3 = tmp10 - tmp13;
        tmp1 = tmp11 + tmp12;
        tmp2 = tmp11 - tmp12;
        /* Odd part */
        z13 = wsptr[5] + wsptr[3];
        z10 = wsptr[5] - wsptr[3];
        z11 = wsptr[1] + wsptr[7];
        z12 = wsptr[1] - wsptr[7];
        tmp7 = z11 + z13;
        tmp11 = (z11 - z13) * ((double) 1.414213562);
        z5 = (z10 + z12) * ((double) 1.847759065); /* 2*c2 */
        tmp10 = ((double) 1.082392200) * z12 - z5; /* 2*(c2-c6) */
        tmp12 = ((double) -2.613125930) * z10 + z5; /* -2*(c2+c6) */
        tmp6 = tmp12 - tmp7;
        tmp5 = tmp11 - tmp6;
        tmp4 = tmp10 + tmp5;
        /* Final output stage: scale down by a factor of 8 and range-limit */
        outptr[0] = (double)(tmp0 + tmp7)/(double)(DCTSIZE);
        outptr[7] = (double)(tmp0 - tmp7)/(double)(DCTSIZE);
        outptr[1] = (double)(tmp1 + tmp6)/(double)(DCTSIZE);
        outptr[6] = (double)(tmp1 - tmp6)/(double)(DCTSIZE);
        outptr[2] = (double)(tmp2 + tmp5)/(double)(DCTSIZE);
        outptr[5] = (double)(tmp2 - tmp5)/(double)(DCTSIZE);
        outptr[4] = (double)(tmp3 + tmp4)/(double)(DCTSIZE);
        outptr[3] = (double)(tmp3 - tmp4)/(double)(DCTSIZE);
        wsptr += DCTSIZE;        /* advance pointer to next row */
    }
}

void copy_to_buf64(const double* img, double* dst, const int start, const int W){
    int st = start;
    for (int i = 0; i < 8; i++, st+=W){
        for (int j = 0; j < 8; j++){
            dst[i*8+j] = img[st+j];
        }
    }
}

void copy_from_buf64(double* img, const double* buf, const int start, const int W){
    for (int i = 0, st = start; i < 8; i++, st+=W){
        for (int j = 0; j < 8; j++){
            img[st+j] = buf[i*8+j];
        }
    }
}

void block_dct(const double* img, double* coef, const int H, const int W){
    for (int i = 0; i < H; i+=8){
        for (int j = 0; j < W; j+=8){
            double* buf = new double[64];
            copy_to_buf64(img, buf, i*W+j, W);
            fdct_8x8(buf);
            copy_from_buf64(coef, buf, i*W+j, W);
            delete[] buf;
        }
    }
}

void discard_one_block(const double* buf, double* nbuf, const int N_retain){
    for (int i = 0; i < N_retain; i++){
        nbuf[zigzag[i]] = buf[zigzag[i]];
    }
    for (int i = N_retain; i < 64; i++){
        nbuf[zigzag[i]] = 0;
    }
}

void discard(const double* img, double* dcoef, const int N_retain, const int H, const int W){
    for (int i = 0; i < H; i+=8){
        for (int j = 0; j < W; j+=8){
            double* buf = new double[64];
            double* nbuf = new double[64];
            copy_to_buf64(img, buf, i*W+j, W);
            discard_one_block(buf, nbuf, N_retain);
            copy_from_buf64(dcoef, nbuf, i*W+j, W);
            delete[] buf;
        }
    }
}

void block_idct(double* img, const double* coef, const int H, const int W){
    for (int i = 0; i < H; i+=8){
        for (int j = 0; j < W; j+=8){
            double* buf = new double[64];
            double* bimg = new double[64];
            copy_to_buf64(coef, buf, i*W+j, W);
            idct_8x8(buf, bimg);
            copy_from_buf64(img, bimg, i*W+j, W);
            delete[] buf;
            delete[] bimg;
        }
    }
}

void DCT_pipline_one_channel(const double* Y, double* iY, const int H, const int W, const int N_retain){
    double* Ycoef = new double[H*W];
    block_dct(Y, Ycoef, H, W);
    double* dYcoef = new double[H*W];
    discard(Ycoef, dYcoef, N_retain, H, W);
    delete[] Ycoef;
    block_idct(iY, dYcoef, H, W);
    delete[] dYcoef;
}

void DCT_pipline(const string name, const int H, const int W, const int total_N_retain){
    int N_retain = total_N_retain / ((H >> 3) * (W >> 3));
    MyImage IMG;
    IMG.setImagePath(str2pchar(name));
    IMG.setWidth(W);
    IMG.setHeight(H);
    IMG.ReadImage();
    
    double* YUV = new double[H*W*3];
    RGB2YUV(IMG.getImageData(), YUV, H, W);
    double* Y = new double[H*W];
    double* U = new double[H*W];
    double* V = new double[H*W];
    split_channel(YUV, Y, U, V, H, W);
    delete[] YUV;
    double* iY = new double[H*W];
    double* iU = new double[H*W];
    double* iV = new double[H*W];

    DCT_pipline_one_channel(Y, iY, H, W, N_retain);
    delete[] Y;
    DCT_pipline_one_channel(U, iU, H, W, N_retain);
    delete[] U;
    DCT_pipline_one_channel(V, iV, H, W, N_retain);
    delete[] V;

    double* pYUV = new double[H*W*3];
    combine_channel(pYUV, iY, iU, iV, H, W);
    delete[] iY;
    delete[] iU;
    delete[] iV;
    
    long int pos = name.find(".");
    string save = name.substr (0,pos) + "_" + \
                   to_string(total_N_retain) + "_dct.raw";
    save_YUV(pYUV, save, H, W);
}

int symmetric_flip(int pos, int boarder){
    if (pos < 0){
        return -1 * pos;
    }
    if (pos > boarder-1){
        return (boarder << 1) - pos - 1;
    }
    return pos;
}

void conv_h(const double* img, double* coef, const double* K, const int tap, const int H, const int W){
    int half_tap = tap >> 1;
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            double val = 0;
            for (int k = 0; k < tap; k++){
                val += K[k] * img[i*W+symmetric_flip(j-half_tap+k, W)];
            }
            coef[i*W+j] = val;
        }
    }
}

void conv_v(double* img, double* coef, const double* K, const int tap, const int H, const int W){
    int half_tap = tap / 2;
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            double val = 0;
            for (int k = 0; k < tap; k++){
                val += K[k] * img[symmetric_flip(i-half_tap+k, H)*W+j];
            }
            coef[i*W+j] = val;
        }
    }
}

void pooling(const double* img, double* subimg, const int H, const int W){
    int tH = H >> 1;
    int tW = W >> 1;
    for (int i = 0; i < tH; i++){
        for (int j = 0; j < tW; j++){
            subimg[i*tW+j] = img[(i<<1)*W+(j<<1)];
        }
    }
}

void unpooling(const double* subimg, double* img, const int H, const int W){
    int tW = W << 1;
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            int ii = i << 1;
            int jj = j << 1;
            img[ii*tW+jj] = subimg[i*W+j];
            img[ii*tW+jj+1] = subimg[i*W+j];
            img[(ii+1)*tW+jj] = subimg[i*W+j];
            img[(ii+1)*tW+jj+1] = subimg[i*W+j];
        }
    }
}

void dwt_one_level(const double* img, double* coef, const int H, const int W){
    double* img_L = new double[H*W];
    conv_h(img, img_L, lowpass, 5, H, W);
    double* img_LL = new double[H*W];
    conv_v(img_L, img_LL, lowpass, 5, H, W);
    pooling(img_LL, coef, H, W);
    delete[] img_LL;
}

void idwt_one_level(const double* img, double* coef, const int H, const int W){
    unpooling(img, coef, H, W);
}

void dwt(double* img, double* coef, const int level, const int H, const int W){
    double* workspace = new double[H*W];
    for (int i = 0; i < H*W; i++){
        workspace[i] = img[i];
    }
    int tH = H, tW = W;
    for (int i = 0; i < level; i++){
        double* coef_t = new double[(tH*tW)>>2];
        dwt_one_level(workspace, coef_t, tH, tW);
        tH = tH >> 1;
        tW = tW >> 1;
        for (int ii = 0; ii < tH*tW; ii++){
            workspace[ii] = coef_t[ii];
        }
        delete[] coef_t;
    }
    for (int i = 0; i < tH*tW; i++){
        coef[i] = workspace[i];
    }
}

void idwt(double* img, double* coef, const int level, const int H, const int W){
    double* workspace = new double[H*W<<(2*level)];
    for (int i = 0; i < H*W; i++){
        workspace[i] = img[i];
    }
    int tH = H, tW = W;
    for (int i = 0; i < level; i++){
        double* coef_t = new double[(tH*tW)<<2];
        idwt_one_level(workspace, coef_t, tH, tW);
        tH = tH << 1;
        tW = tW << 1;
        for (int ii = 0; ii < tH*tW; ii++){
            workspace[ii] = coef_t[ii];
        }
        delete[] coef_t;
    }
    for (int i = 0; i < tH*tW; i++){
        coef[i] = workspace[i];
    }
}

void DWT_pipline_one_channel(double* Y, double* iY, const int H, const int W, const int level){
    double* Ycoef = new double[(H>>level)*(W>>level)];
    dwt(Y, Ycoef, level, H, W);
    idwt(Ycoef, iY, level, H>>level, W>>level);
    delete[] Ycoef;
}

void DWT_pipline(const string name, const int H, const int W, const int total_N_retain){
    int level = log2(H * W / total_N_retain) / 2;
    MyImage IMG;
    IMG.setImagePath(str2pchar(name));
    IMG.setWidth(W);
    IMG.setHeight(H);
    IMG.ReadImage();
    
    double* YUV = new double[H*W*3];
    RGB2YUV(IMG.getImageData(), YUV, H, W);
    double* Y = new double[H*W];
    double* U = new double[H*W];
    double* V = new double[H*W];
    split_channel(YUV, Y, U, V, H, W);
    delete[] YUV;
    double* iY = new double[H*W];
    double* iU = new double[H*W];
    double* iV = new double[H*W];

    DWT_pipline_one_channel(Y, iY, H, W, level);
    delete[] Y;
    DWT_pipline_one_channel(U, iU, H, W, level);
    delete[] U;
    DWT_pipline_one_channel(V, iV, H, W, level);
    delete[] V;

    double* pYUV = new double[H*W*3];
    combine_channel(pYUV, iY, iU, iV, H, W);
    delete[] iY;
    delete[] iU;
    delete[] iV;
    
    long int pos = name.find(".");
    string save = name.substr (0,pos) + "_" + \
                   to_string(total_N_retain) + "_dwt.raw";
    save_YUV(pYUV, save, H, W);
}

void run_HW3(int argc, const char * argv[], const int H, const int W){
    if (argc < 3){
        printf("\nERROR! Too less arguments! \n");
        printf("   HW3 Usage: ./main <image> <N_retain>\n\n");
        exit(1);
    }
    DCT_pipline(argv[1], H, W, str2int(argv[2]));
    DWT_pipline(argv[1], H, W, str2int(argv[2]));
}
#endif /* HW3_h */
