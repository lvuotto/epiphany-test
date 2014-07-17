
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>


#define E_ROWS   4
#define E_COLS   4
#define E_CORES 16

#define BUFFER_OFFSET 0x00100000


int main () {
  
  int fb;
  unsigned int x, y, fbl;
  unsigned char *fbm;
  
  struct fb_fix_screeninfo fbfsi;
  struct fb_var_screeninfo fbvsi;
  
  fb = open("/dev/fb0", O_RDWR);
  if (fb > 0) {
    if (ioctl(fb, FBIOGET_FSCREENINFO, &fbfsi) == 0) {
      printf("smem_start: %#lx\n", fbfsi.smem_start);
      printf("smem_len: %d\n", fbfsi.smem_len);
      printf("line_length: %d\n", fbfsi.line_length);
    } else {
      printf("AAAAGGH! NO PUDE ABRIR!!!\n");
      exit(1);
    }
    if (ioctl(fb, FBIOGET_VSCREENINFO, &fbvsi) == 0) {
      printf("xres: %d\n", fbvsi.xres);
      printf("yres: %d\n", fbvsi.yres);
      printf("xres_virtual: %d\n", fbvsi.xres_virtual);
      printf("yres_virtual: %d\n", fbvsi.yres_virtual);
      printf("xoffset: %d\n", fbvsi.xoffset);
      printf("yoffset: %d\n", fbvsi.yoffset);
      printf("bits_per_pixel: %d\n", fbvsi.bits_per_pixel);
    } else {
      printf("AAAAGGH! NO PUDE ABRIR!!!\n");
      exit(1);
    }
  } else {
    printf("AAAAGGH! NO PUDE ABRIR!!!\n");
    exit(1);
  }
  
  fbm = (unsigned char *) mmap(NULL,
                               fbfsi.smem_len,
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED,
                               fb,
                               0);
  
  for (y = 0; y < fbvsi.yres / 4; y++) {
    for (x = 0; x < fbvsi.xres / 4; x++) {
      fbl = (x + fbvsi.xoffset)*(fbvsi.bits_per_pixel / 8) +
            (y + fbvsi.yoffset)*fbfsi.line_length;
      *(fbm + fbl + 0) = 0xff;
      *(fbm + fbl + 1) = 0xff;
      *(fbm + fbl + 2) = 0xff;
      *(fbm + fbl + 3) = 0x00;
    }
  }
  
  getchar();
  
  munmap(fbm, fbfsi.smem_len);
  close(fb);
  
  return 0;
  
}
