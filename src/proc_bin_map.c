#include "elfparse.h"
#include "proc_bin_map.h"
#include "procfs.h"

#include <err.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


bool elfimg_unmap(s_elfimg *img)
{
    bool res = munmap(img->data, img->size) != 0;
    if (res)
        warn("munmap failed");

    return res;
}


bool proc_map_bin(s_proc *proc, s_elfimg *res)
{
    int fd = proc_open_procfs(proc, "exe", O_RDONLY);
    if (fd < 0)
        return true;

    struct stat stat;
    if (fstat(fd, &stat) < 0)
    {
        close(fd);
        return true;
    }

    res->size = stat.st_size;

    void *img = mmap(0, res->size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    if (img == MAP_FAILED)
    {
        warn("mmap failed");
        return true;
    }

    res->data = img;
    return false;
}
