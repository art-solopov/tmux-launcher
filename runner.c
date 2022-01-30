#include <unistd.h>
#include <mruby.h>
#include <mruby/irep.h>
#include <mruby/array.h>
#include <mruby/value.h>
#include <mainrb.c>

int
main(int argc, char ** argv)
{
	mrb_state *mrb = mrb_open();
	mrb_value rb_argv = mrb_ary_new(mrb);
	int i;

	if (!mrb) {
		fprintf(stderr, "Invalid mrb_state\n");
		return -1;
	}
	mrb_load_irep(mrb, mainrb);

	for(i = 0; i < argc; i++) {
		mrb_ary_push(mrb, rb_argv, mrb_str_new_cstr(mrb, argv[i]));
	}
	mrb_define_const(mrb, mrb->kernel_module, "ARGV", rb_argv);
	mrb_define_const(mrb, mrb->kernel_module, "CWD", mrb_str_new_cstr(mrb, getcwd(NULL, 0)));
	mrb_value rbr = mrb_load_string(mrb, "Main.call");
	mrb_int result = mrb_int(mrb, rbr);

	mrb_close(mrb);
	return result;
}
