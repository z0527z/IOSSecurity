//
//  jailbrokenDetection.cpp
//  cmdTest
//
//  Created by dingql on 14-6-12.
//  Copyright (c) 2014年 dingql. All rights reserved.
//

#include "jailbrokenDetection.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <mach-o/loader.h>
#include <mach-o/dyld.h>
#include <mach-o/arch.h>

#pragma - 沙盒完整性校验
void jailbrokenDetection::methodOne()
{
    // 1.沙盒完整性校验
    // -- 根据fork()的返回值判断创建子进程是否成功, 返回-1，表示没有创建新的进程, 在子进程中返回0, 父进程返回子进程PID
    // -- 沙盒被破坏, fork返回大于等于0
    
    int result = fork();
    printf("result:%d\n", result);
}

#pragma - 文件系统检测
void jailbrokenDetection::methodTwo()
{
    // 1.检查常见的越狱文件是否存在, state -- 返回0, 表示指定的文件存在, 返回-1, 表示执行失败, 错误代码存在于errno中
    // -- 包括: /Library/MobileSubstrate/MobileSubstrate.dylib 最重要的越狱文件，几乎所有的越狱机都会安装
    // --      /Applications/Cydia.app/, /var/lib/cydia/, /var/cache/apt/, /var/lib/apt/, /etc/apt/,
    // --      /bin/bash/, /bin/sh/, /usr/sbin/sshd/, /usr/libexec/ssh-keysign/, /etc/ssh/sshd_config/,
    
    struct stat s;
    // 需要检查stat的完整性, 路径名做编码处理（不要使用base64编码），千万不要使用NSFileManager类，会被hook掉
    int result = stat("/Applications/Cydia.app", &s);
    printf("result:%d\n", result);
    
    // 2.检查/etc/fstab文件的大小, state
    
    // 3.检查特定的文件是否是符号链接文件, lstat检查 /Applications, /Library/Ringtones, /Library/Wallpaper,
    //   /usr/include, /usr/libexec, /usr/share
    
    // 4.检查特定目录或文件的文件访问权限, 采用C函数statfs();
    
    // 5.检测能否创建子进程, 采用C函数fork(), popen()等
    
    // 6.检测能否执行ssh本地连接, 执行 ssh 127.0.0.1 -p 22 看是否成功
    
    // 7.检测system()函数的返回值, 不要任何参数, 在越狱设备上返回1, 正常设备返回0
    
    // 8.检测dylib(动态链接库)的内容, 调用_dyld_image_count()和_dyld_get_image_name()来看当前有哪些dylib被加载
    uint32_t count = _dyld_image_count();
    for (uint32_t i = 0; i < count; i ++) {
        const char * dyld = _dyld_get_image_name(i);
        size_t slength = strlen(dyld);
        size_t j;
        for (j = slength - 1; j > 0; --j){
            if (dyld[j] == '/') break;
        }
        printf("%s\n", dyld);
    }
    printf("\n");
}

void jailbrokenDetection::methodThree()
{
    
}