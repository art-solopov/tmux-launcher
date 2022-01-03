#include <mruby.h>
#include <mruby/irep.h>
#include <mainrb.c>

int
main(void)
{
  mrb_state *mrb = mrb_open();
  if (!mrb) { /* handle error */ }
  mrb_load_irep(mrb, mainrb);
  mrb_close(mrb);
  return 0;
}
