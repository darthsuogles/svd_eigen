#!/bin/bash

#ver=3.1.4
ver=3.2.1

script_name=$0

function quit_with()
{
    echo "Error: $script_name"    
    printf ">> $[@]\n"
    exit
}

mkdir eigen; cd eigen

if [ ! -d $ver ]; then
    fname=$ver
    tarball=$fname.tar.bz2
    if [ ! -f $tarball ]; then
	wget http://bitbucket.org/eigen/eigen/get/$tarball
    fi
    echo "Uncompressing ... might take a while ... please be patient"
    #fname=`tar tjf $tarball | sed -e 's@/.*@@' | uniq`
    fname=`tar -jxvf $tarball | sed -e 's@/.*@@' | uniq`
    if [ $fname != $ver ]; then 
	mv $fname $ver
    fi

    if [ ! -d $ver ]; then quit_with "failed to uncompress $tarball"; fi
    rm $tarball
fi

if [ ! -z "$flag_build_eigen" ]; then
    cd $ver
    mkdir -p build-tree; cd build-tree
    cmake .. -DCMAKE_INSTALL_PREFIX=../install-dir
    make install
    cd ..
fi

cd ..
