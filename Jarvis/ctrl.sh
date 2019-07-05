#!/bin/bash

JARVIS=$(pwd)
Jarvis=$JARVIS/bin/Jarvis

function usage() {
	printf "Usage: %s [-s(start) | -q(stop) | -r(restart)]\n" "$0"
}
	
function startJarvis-server() {
	pid=$(pidof $Jarvis)
	if [ $? -eq 0 ];then
        echo  "`date +%Y-%m-%d,%H:%m:%s`:Jarvis-server is already runing ···"
	else
		$Jarvis
        echo "`date +%Y-%m-%d,%H:%m:%s`:Jarvis-server start success ···"
	fi
}

function stopJarvis-server() {
	pid=$(pidof $Jarvis)
	if [ $? -eq 0 ];then
		kill -9 $pid
        echo "`date +%Y-%m-%d,%H:%m:%s`:Jarvis-server stop success ···"
	else
        echo "`date +%Y-%m-%d,%H:%m:%s`:Jarvis-server is already stop ···"
	fi
}

function restartJarvis-server() {
	stopJarvis-server
    echo "`date +%Y-%m-%d,%H:%m:%s`:Jarvis-server is already stop ···"
	startJarvis-server
    echo "`date +%Y-%m-%d,%H:%m:%s`:Jarvis-server is already restart ···"
}


if [ $# -ne 1 ] ; then
	usage
fi 

case $1 in 
	-s | 'start' )
	startJarvis-server
	;;
	-q | 'stop' )
	stopJarvis-server
	;;
	-r | 'restart')
	restartJarvis-server
	;;
esac
