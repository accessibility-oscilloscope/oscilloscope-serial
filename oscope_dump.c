#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const uint8_t INIT_SEQ[] = {
    0,   20,                         // board ID 0
    135, 0,  100,                    // serial delay of 100
    122, 0,  10,                     // 10 samples per packet
    123, 0,                          // increment
    124, 3,                          // downsample 3
    125, 1,                          // 1 tick wait
    136, 2,  32,  0,  0,   255, 200, // io expander on
    136, 2,  32,  1,  0,   255, 200, //  io expander on
    136, 2,  33,  0,  0,   255, 200, // io expander on
    136, 2,  33,  1,  0,   255, 200, // io expander on
    136, 2,  32,  18, 240, 255, 200, // init
    136, 2,  32,  19, 15,  255, 200, // init ADC
    136, 2,  33,  18, 0,   255, 200, // init
    136, 2,  33,  19, 0,   255, 200, // init
    131, 8,  0,                      // ADC offset
    131, 6,  16,                     // binary offset
    131, 4,  36,                     // 300Ohm termination A
    131, 5,  36,                     // 300Ohm termination B
    131, 1,  0,                      // disable multiplexing
    136, 3,  96,  80, 136, 22,  0,   // channel 0 calibration
    136, 3,  96,  82, 136, 22,  0,   // channel 1 calibration
    136, 3,  96,  84, 136, 22,  0,   // channel 2 calibration
    136, 3,  96,  86, 136, 22,  0};  // channel 3 calibration

const uint8_t SAMPLE_SEQ[] = {100, 10};

int main(int ac, char *av[]) {
  assert(ac == 2 && "usage: serial_dump $TTY");
  int fd = open(av[1], O_SYNC | O_RDWR);

  if (fd < 0) {
    switch (errno) {
    case EACCES:
      fprintf(stderr, "bad access of $TTY\n");
      break;
    case ENOENT:
      fprintf(stderr, "$TTY does not exist\n");
      break;
    default:
      fprintf(stderr, "errno %d\n", errno);
      break;
    }
    exit(errno);
  }

  for (long unsigned int ii = 0; ii < sizeof(INIT_SEQ); ii++) {
    int w = write(fd, &INIT_SEQ[ii], 1);
    assert(w == 1 && "failed to open $TTY");
    usleep(10000); // config settle time
  }
  printf("init sequence wrote\n");

  for (;;) {
    int w = write(fd, SAMPLE_SEQ, sizeof(SAMPLE_SEQ));
    assert(w == 2 && "failed to request sample");

    usleep(100000); // sample acquisition time

    uint8_t read_buf[40];
    int r = read(fd, read_buf, sizeof(read_buf));
    printf("%d\n", r);
    for (long unsigned int ii = 0; ii < sizeof(read_buf) / 4; ii++) {
      printf("%x ", read_buf[ii]);
    }
    printf("\n");
  }

  close(fd);

  return 0;
}
