## CSCI 576 Homework
***
@Yifan Wang   
3038-1849-83   
wang608@usc.edu   

***
1. Xcode project provided;

2. Cmake can be used to compile the source file, tested on OS X (11.2.2). Usage (in `PROJECT_SOURCE_DIR`):
```bash
# before run each homework, need to uncomment the correspoding run function in <main.cpp> and re-build;
mkdir build
cd build
cmake .. 
make 
cd ..
```

3. Provided display part is removed from the code since OS X doesn't support MFC specified heads. 

4. The raw file can be opened either using ImageJ or PhotoShop.

***
### HW1 2020.02.01

```
./main <input image> \
       <Y stride>    \
       <U stride>    \
       <V stride>    \
       <Q>           \
       <(optinal, bool) Width Only subsampling (default:TRUE>
```
***

The result image will be save to:
```
<input image (removed suffix)>_<Y stride>_<U stride>_<V stride>_<Q>.raw
```
Bi-linear Interploatation is used to perfrom the upsampling task.

***
### HW2 2021.03.04
```
./main <input image> \
       <TH1>         \
       <TH2> 
```
Output file will be saved to:
```
<input image (removed suffix)>_<TH1>_<TH2>.raw
```

***
### HW3 2021.03.16
```
./main <input image> \
       <N_retained>         
```
Output file will be saved to:
```
<input image (removed suffix)>_<N_retained>.raw
```