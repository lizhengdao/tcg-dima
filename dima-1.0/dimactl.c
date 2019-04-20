#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/ioctl.h>  
#include <imaevm.h>
#include "subcommand.h"

static void usage()
{
    info("dimactl: usage \n");
    info("dimactl: -p <pid>\n");
    info("dimactl: -m <module name>\n");
}

static int 
dyn_measurement_process(int pid)
{
    int fd, err;

    fd = open("/dev/dima",O_RDWR);
    if(fd < 0){
        err("Fail to open  /dev/dima  %s (%d)\n",strerror(errno),errno);
        return -1;
    }

    ioctl(fd,DIMA_SET_MEASUREMENT_UNLOCK_MODE_CMD);

    err = ioctl(fd,DIMA_MEASUREMENT_PROCESS_CMD,(unsigned long)&pid);
    info("Measurement pid=%d status=%d \n",pid,err);

    ioctl(fd,DIMA_SET_MEASUREMENT_LOCK_MODE_CMD);

    close(fd);

    return 0;
}

static int 
dyn_measurement_module(const char* mname)
{
    int fd, err;

    fd = open("/dev/dima",O_RDWR);
    if(fd < 0){
        err("Fail to open  /dev/dima  %s (%d)\n",strerror(errno),errno);
        return -1;
    }

    ioctl(fd,DIMA_SET_MEASUREMENT_UNLOCK_MODE_CMD);

    err = ioctl(fd,DIMA_MEASUREMENT_MODULE_CMD,(unsigned long)mname);
    info("Measurement module name=%s status=%d \n",mname,err);

    ioctl(fd,DIMA_SET_MEASUREMENT_LOCK_MODE_CMD);

    close(fd);

    return 0;
}

int dimactl_main(int argc, char **argv)
{
    int ch;
    
    while((ch = getopt(argc, argv, "p:m:"))!=-1)
    {
        switch(ch)
        {
            case 'm':
            {
                if(!optarg || !strlen(optarg)){
                    usage();
                    break;
                }

                dyn_measurement_module(optarg);
                break;
            }
            case 'p':
            {
                if(!optarg || atoi(optarg) < 1){
                    usage();
                    break;
                }

                dyn_measurement_process(atoi(optarg));
                break;
            }
            default:
            {
                usage();
                break;
            }
        }
    }
    
    return 0;    
}

int main(int argc, char **argv)
{
    return dimactl_main(argc, argv);
}
