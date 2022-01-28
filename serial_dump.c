#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

const uint8_t init_seq[] = {
    0,  20,  135, 0,   100, 122, 0,   10,  123, 0,   124, 3,   125, 1,   136,
    2,  32,  0,   0,   255, 200, 136, 2,   32,  1,   0,   255, 200, 136, 2,
    33, 0,   0,   255, 200, 136, 2,   33,  1,   0,   255, 200, 136, 2,   32,
    18, 240, 255, 200, 136, 2,   32,  19,  15,  255, 200, 136, 2,   33,  18,
    0,  255, 200, 136, 2,   33,  19,  0,   255, 200, 131, 8,   0,   131, 6,
    16, 131, 4,   36,  131, 5,   36,  131, 1,   0,   136, 3,   96,  80,  136,
    22, 0,   136, 3,   96,  82,  136, 22,  0,   136, 3,   96,  84,  136, 22,
    0,  136, 3,   96,  86,  136, 22,  0};

const uint8_t sample_seq[] = {100, 10};

int main(int ac, char *av[]) {
  int fd = open("/dev/ttyUSB1", O_SYNC | O_RDWR);

  for (int ii = 0; ii < sizeof(init_seq); ii++) {
    int w = write(fd, &init_seq[ii], 1);
    assert(w == 1);
    usleep(10000); // config settle time
  }
  printf("init sequence wrote\n");

  for (;;) {
    int w = write(fd, sample_seq, sizeof(sample_seq));
    assert(w == 2);

    usleep(10000); // sample acquisition time

    uint8_t read_buf[40];
    int r = read(fd, read_buf, sizeof(read_buf));
    printf("%d\n", r);
    for (int ii = 0; ii < sizeof(read_buf) / 4; ii++) {
      printf("%x ", read_buf[ii]);
    }
    printf("\n");
  }

  return 0;
}
