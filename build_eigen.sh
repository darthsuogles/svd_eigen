#!/bin/bash

#ver=3.1.4
ver=3.2.1

mkdir eigen; cd eigen

if [ ! -d $ver ]; then
    fname=$ver
    tarball=$fname.tar.bz2
    if [ ! -f $tarball ]; then
	wget http://bitbucket.org/eigen/eigen/get/$tarball
    fi
    #fname=`tar tjf $tarball | sed -e 's@/.*@@' | uniq`
    fname=`tar -jxvf $tarball | sed -e 's@/.*@@' | uniq`
    mv $fname $ver
fi

if [ ! -z "$flag_build_eigen" ]; then
    cd $ver
    mkdir -p build-tree; cd build-tree
    cmake .. -DCMAKE_INSTALL_PREFIX=../install-dir
    make install
    cd ..
fi

cd ..
