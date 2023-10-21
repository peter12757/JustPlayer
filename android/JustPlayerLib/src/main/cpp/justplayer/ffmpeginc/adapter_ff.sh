#!/bin/zsh

currentpath=$(pwd)

#echo "[start]currentpath is :$currentpath \n"
if [ ! -f "libavutil/fftime.h" ]; then
mv libavcodec/jni.h libavcodec/ffjni.h
mv libavutil/time.h libavutil/fftime.h
#libavutil
grep "\"libavutil\/" -rl ./libavutil | xargs sed -i ".bak" "s/\"libavutil\//\"\.\.\/libavutil\//g"
grep "\"libavutil\/" -rl ./libavcodec | xargs sed -i ".bak" "s/\"libavutil\//\"\.\.\/libavutil\//g"
grep "\"libavutil\/" -rl ./libavdevice | xargs sed -i ".bak" "s/\"libavutil\//\"\.\.\/libavutil\//g"
grep "\"libavutil\/" -rl ./libavfilter | xargs sed -i ".bak" "s/\"libavutil\//\"\.\.\/libavutil\//g"
grep "\"libavutil\/" -rl ./libavformat | xargs sed -i ".bak" "s/\"libavutil\//\"\.\.\/libavutil\//g"
grep "\"libavutil\/" -rl ./libswresample | xargs sed -i ".bak" "s/\"libavutil\//\"\.\.\/libavutil\//g"
grep "\"libavutil\/" -rl ./libswscale | xargs sed -i ".bak" "s/\"libavutil\//\"\.\.\/libavutil\//g"

#libavcodec
grep "\"libavcodec\/" -rl ./libavutil | xargs sed -i ".bak" "s/\"libavcodec\//\"\.\.\/libavcodec\//g"
grep "\"libavcodec\/" -rl ./libavcodec | xargs sed -i ".bak" "s/\"libavcodec\//\"\.\.\/libavcodec\//g"
grep "\"libavcodec\/" -rl ./libavdevice | xargs sed -i ".bak" "s/\"libavcodec\//\"\.\.\/libavcodec\//g"
grep "\"libavcodec\/" -rl ./libavfilter | xargs sed -i ".bak" "s/\"libavcodec\//\"\.\.\/libavcodec\//g"
grep "\"libavcodec\/" -rl ./libavformat | xargs sed -i ".bak" "s/\"libavcodec\//\"\.\.\/libavcodec\//g"
grep "\"libavcodec\/" -rl ./libswresample | xargs sed -i ".bak" "s/\"libavcodec\//\"\.\.\/libavcodec\//g"
grep "\"libavcodec\/" -rl ./libswscale | xargs sed -i ".bak" "s/\"libavcodec\//\"\.\.\/avutil\//g"

#libavdevice
grep "\"libavdevice\/" -rl ./libavutil | xargs sed -i ".bak" "s/\"libavdevice\//\"\.\.\/libavdevice\//g"
grep "\"libavdevice\/" -rl ./libavdevice | xargs sed -i ".bak" "s/\"libavdevice\//\"\.\.\/libavdevice\//g"
grep "\"libavdevice\/" -rl ./libavcodec | xargs sed -i ".bak" "s/\"libavdevice\//\"\.\.\/libavdevice\//g"
grep "\"libavdevice\/" -rl ./libavfilter | xargs sed -i ".bak" "s/\"libavdevice\//\"\.\.\/libavdevice\//g"
grep "\"libavdevice\/" -rl ./libavformat | xargs sed -i ".bak" "s/\"libavdevice\//\"\.\.\/libavdevice\//g"
grep "\"libavdevice\/" -rl ./libswresample | xargs sed -i ".bak" "s/\"libavdevice\//\"\.\.\/libavdevice\//g"
grep "\"libavdevice\/" -rl ./libswscale | xargs sed -i ".bak" "s/\"libavdevice\//\"\.\.\/avutil\//g"

#libavfilter
grep "\"libavfilter\/" -rl ./libavutil | xargs sed -i ".bak" "s/\"libavfilter\//\"\.\.\/libavfilter\//g"
grep "\"libavfilter\/" -rl ./libavfilter | xargs sed -i ".bak" "s/\"libavfilter\//\"\.\.\/libavfilter\//g"
grep "\"libavfilter\/" -rl ./libavcodec | xargs sed -i ".bak" "s/\"libavfilter\//\"\.\.\/libavfilter\//g"
grep "\"libavfilter\/" -rl ./libavdevice | xargs sed -i ".bak" "s/\"libavfilter\//\"\.\.\/libavfilter\//g"
grep "\"libavfilter\/" -rl ./libavformat | xargs sed -i ".bak" "s/\"libavfilter\//\"\.\.\/libavfilter\//g"
grep "\"libavfilter\/" -rl ./libswresample | xargs sed -i ".bak" "s/\"libavfilter\//\"\.\.\/libavfilter\//g"
grep "\"libavfilter\/" -rl ./libswscale | xargs sed -i ".bak" "s/\"libavfilter\//\"\.\.\/libavfilter\//g"

#libavformat
grep "\"libavformat\/" -rl ./libavutil | xargs sed -i ".bak" "s/\"libavformat\//\"\.\.\/libavformat\//g"
grep "\"libavformat\/" -rl ./libavformat | xargs sed -i ".bak" "s/\"libavformat\//\"\.\.\/libavformat\//g"
grep "\"libavformat\/" -rl ./libavcodec | xargs sed -i ".bak" "s/\"libavformat\//\"\.\.\/libavformat\//g"
grep "\"libavformat\/" -rl ./libavdevice | xargs sed -i ".bak" "s/\"libavformat\//\"\.\.\/libavformat\//g"
grep "\"libavformat\/" -rl ./libavfilter | xargs sed -i ".bak" "s/\"libavformat\//\"\.\.\/libavformat\//g"
grep "\"libavformat\/" -rl ./libswresample | xargs sed -i ".bak" "s/\"libavformat\//\"\.\.\/libavformat\//g"
grep "\"libavformat\/" -rl ./libswscale | xargs sed -i ".bak" "s/\"libavformat\//\"\.\.\/libavformat\//g"

#libswresample
grep "\"libswresample\/" -rl ./libavutil | xargs sed -i ".bak" "s/\"libswresample\//\"\.\.\/libswresample\//g"
grep "\"libswresample\/" -rl ./libswresample | xargs sed -i ".bak" "s/\"libswresample\//\"\.\.\/libswresample\//g"
grep "\"libswresample\/" -rl ./libavcodec | xargs sed -i ".bak" "s/\"libswresample\//\"\.\.\/libswresample\//g"
grep "\"libswresample\/" -rl ./libavdevice | xargs sed -i ".bak" "s/\"libswresample\//\"\.\.\/libswresample\//g"
grep "\"libswresample\/" -rl ./libavfilter | xargs sed -i ".bak" "s/\"libswresample\//\"\.\.\/libswresample\//g"
grep "\"libswresample\/" -rl ./libavformat | xargs sed -i ".bak" "s/\"libswresample\//\"\.\.\/libswresample\//g"
grep "\"libswresample\/" -rl ./libswscale | xargs sed -i ".bak" "s/\"libswresample\//\"\.\.\/libswresample\//g"

#libswscale
grep "\"libswscale\/" -rl ./libavutil | xargs sed -i ".bak" "s/\"libswscale\//\"\.\.\/libswscale\//g"
grep "\"libswscale\/" -rl ./libswscale | xargs sed -i ".bak" "s/\"libswscale\//\"\.\.\/libswscale\//g"
grep "\"libswscale\/" -rl ./libavcodec | xargs sed -i ".bak" "s/\"libswscale\//\"\.\.\/libswscale\//g"
grep "\"libswscale\/" -rl ./libavdevice | xargs sed -i ".bak" "s/\"libswscale\//\"\.\.\/libswscale\//g"
grep "\"libswscale\/" -rl ./libavfilter | xargs sed -i ".bak" "s/\"libswscale\//\"\.\.\/libswscale\//g"
grep "\"libswscale\/" -rl ./libavformat | xargs sed -i ".bak" "s/\"libswscale\//\"\.\.\/libswscale\//g"
grep "\"libswscale\/" -rl ./libswresample | xargs sed -i ".bak" "s/\"libswscale\//\"\.\.\/libswscale\//g"

else
  echo "has adaptered  \n"
fi
