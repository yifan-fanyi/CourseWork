cjpeg(){
    for((k=1;k<6;k++)) 
    do   
        for((i=0;i<8;++i))
        do
            for((j=0;j<8;++j))
            do
            ./cjpeg -quality S2 $1"$k"_"$i"_"$j".bmp>$1"$k"_"$i"_"$j".jpg
            done
        done
    done 
}

cjpeg boat 80
cjpeg jelly_beans 80

