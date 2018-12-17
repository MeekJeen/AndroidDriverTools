[CSDN链接](https://blog.csdn.net/u010164190/article/details/83900420)
module_driver_for_android是一个简单的android驱动,可以模块编译ko文件，还包含对ko签名文件具体步骤.

1.Android7.0步骤
注意：执行make命令之前，在android源码中执行以下命令(不然Makefile找不到aarch64-linux-android-交叉编译命令) 
#source build/envsetup.sh     
#lunch xxx-userdebug

2.Android9.0需要修改下
需要改的位置：android9.0/build/envsetup.sh是软连接
真正的位置：android9.0/build/make/envsetup.sh中修改: function make(){...} 
function make() 
{
    local start_time=$(date +"%s")
    $(get_make_command) "$@"
    local ret=$?
    local end_time=$(date +"%s")
    local tdiff=$(($end_time-$start_time))
    local hours=$(($tdiff / 3600 ))
    local mins=$((($tdiff % 3600) / 60))
    local secs=$(($tdiff % 60))
    local ncolors=$(tput colors 2>/dev/null)
    if [ -n "$ncolors" ] && [ $ncolors -ge 8 ]; then
        color_failed=$'\E'"[0;31m"
        color_success=$'\E'"[0;32m"
        color_reset=$'\E'"[00m"
    else
        color_failed=""
        color_success=""
        color_reset=""
    fi
    echo
    if [ $ret -eq 0 ] ; then
        echo -n "${color_success}#### make completed successfully "
    else
        echo -n "${color_failed}#### make failed to build some targets "
    fi
    if [ $hours -gt 0 ] ; then
        printf "(%02g:%02g:%02g (hh:mm:ss))" $hours $mins $secs
    elif [ $mins -gt 0 ] ; then
        printf "(%02g:%02g (mm:ss))" $mins $secs
    elif [ $secs -gt 0 ] ; then
        printf "(%s seconds)" $secs
    fi
    echo " ####${color_reset}"
    echo
    return $ret
}


