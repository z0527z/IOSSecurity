//
//  main.cpp
//  cmdTest
//
//  Created by dingql on 14-4-1.
//  Copyright (c) 2014年 dingql. All rights reserved.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/ptrace.h>
#include <stdlib.h>

#include "jailbrokenDetection.h"

/********************************************
 * 1. 反调试
 * 2. 反注入, 检查函数完整性，是否被替换
 * 3. 反汇编, inline编译, static函数, -O3优化将具体的计算逻辑隐藏, 使用strip去除目标文件中的制定符号, 
 *           -funroll-loops完全展开循环结构, 增加汇编代码的长度
 */

static int check_debugger() __attribute__((always_inline)); // 强制 inline
// -finline-functions告诉编译器尽可能的采用inline编译方式；-Winline告诉编译器在不能使用inline编译方式编译时报错

// 检查进程的状态是否为 P_TRACED —— 即是否有debugger在trace进程
int check_debugger()
{
    size_t size = sizeof(struct kinfo_proc);
    struct kinfo_proc info;
    int ret, name[4];
    memset(&info, 0, sizeof(struct kinfo_proc));
    name[0] = CTL_KERN;
    name[1] = KERN_PROC;
    name[2] = KERN_PROC_PID;
    name[3] = getpid();
    if (ret = (sysctl(name, 4, &info, &size, NULL, 0))) {
        return ret;
    }
    
    return (info.kp_proc.p_flag & P_TRACED) ? 1 : 0;
}

typedef int (*FUNC_SYSCTL)(int *, u_int, void *, size_t *, void *, size_t);

// 检查函数完整性
void checkCompleteness(void)
{
    int ret;
    Dl_info dylib_info;
    FUNC_SYSCTL func_sysctl  = sysctl;
    if ((ret = dladdr((const void *)func_sysctl, &dylib_info))) {
        
    }
    printf("lib:%s\n", dylib_info.dli_fname);
}

#define PT_DENY_ATTACH 31
int main(int argc, const char * argv[])
{
    checkCompleteness();
    
    // 方法一
//    int ret = check_debugger();
//    if (ret == 1) {
//        printf("process deny debug\n");
//        exit(1);
//    }
//    printf("ret:%d\n", ret);
    
    // 方法二
    // 调用ptrace请求来检查进程是否被调试
    //ptrace(PT_DENY_ATTACH, 0, 0, 0);
    printf("Hello---IOS securety!\n");
    
    jailbrokenDetection * jail = new jailbrokenDetection();
    jail->methodTwo();
    return 0;
}
