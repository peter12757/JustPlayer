currentpath=$(pwd)


#sed -i "s/libavutil/\.\.\/libavutil/g" 'grep "libavutil" -rl $cacheFile/libavutil'
#grep "\"libavutil\/" -rl ./libavutil
grep "\"libavutil\/" -rl ./libavutil | xargs sed -i "" "s/\"libavutil\//\"\.\.\/libavutil\//g"