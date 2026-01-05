#include "pico/stdlib.h"
#include "tusb.h"

static uint8_t rx[64];
static const char ack[] = "ACK";

int main() {
  stdio_init_all();
  tusb_init();

  while (true) {
    tud_task();

    if (tud_vendor_available()) {
      uint32_t n = tud_vendor_read(rx, sizeof(rx));
      (void)n;
      tud_vendor_write((void*)ack, sizeof(ack)-1);
      tud_vendor_flush();
    }

    sleep_ms(1);
  }
}
