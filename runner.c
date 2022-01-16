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

	struct RClass* mod = mrb_module_get(mrb, "TmuxLauncher");
	mrb_funcall(mrb, mrb_obj_value(mod), "call", 1, rb_argv);

	mrb_close(mrb);
	return 0;
}
