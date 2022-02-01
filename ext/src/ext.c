#include <mruby.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/variable.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mruby/ext/io.h>

const char* MODULE_NAME = "TmuxLauncher";

struct RClass* getMod(mrb_state * mrb);

static mrb_value
run_tmux_command(mrb_state *mrb, mrb_value self) {
	const mrb_value* argv;
	int argc;
	int pipefd[2];
	int i;
	pid_t cpid;
	char ** tmux_args;

	mrb_get_args(mrb, "*", &argv, &argc);
	
	tmux_args = (char **)mrb_calloc(mrb, argc + 2, sizeof(char *));
	tmux_args[0] = "tmux";
	for(i = 0; i < argc; i++) {
		tmux_args[i + 1] = mrb_string_cstr(mrb, argv[i]);
	}
	tmux_args[argc + 1] = NULL;

	pipe(pipefd);
	cpid = fork();
	if (cpid < 0) {
		mrb_raise(mrb, E_RUNTIME_ERROR, "Fork unsuccessful");
	}
	if (cpid == 0) {
		// Child process
		close(pipefd[0]);
		dup2(pipefd[1], STDERR_FILENO);
		execvp("tmux", tmux_args);
	} else {
		int cstatus;
		mrb_value io;
		const struct RClass *io_cls = mrb_class_get(mrb, "IO");
		mrb_value io_args[1];

		const struct RClass *result_cls = mrb_class_get_under(mrb, getMod(mrb), "Result");
		mrb_value result_args[3];

		close(pipefd[1]);
		waitpid(cpid, &cstatus, 0);

		io_args[0] = mrb_fixnum_value(pipefd[0]);
		io = mrb_obj_new(mrb, io_cls, 1, io_args);

		result_args[0] = mrb_fixnum_value(cpid);
		result_args[1] = mrb_fixnum_value(cstatus);
		result_args[2] = io;
		return mrb_obj_new(mrb, result_cls, 3, result_args);
	}

	return mrb_nil_value();
}

static mrb_value
run_tmux_attach(mrb_state *mrb, mrb_value self) {
	const char * session_name = NULL;

	mrb_get_args(mrb, "z", &session_name);

	execlp("tmux", "tmux", "a", "-t", session_name, NULL);
	return mrb_nil_value();
}

void
mrb_tmux_launcher_ext_gem_init(mrb_state *mrb) {
	struct RClass *mod = mrb_define_module(mrb, MODULE_NAME);
	mrb_define_class_method(
			mrb, mod,
			"_run_tmux_command", run_tmux_command,
			MRB_ARGS_REST()
			);
	mrb_define_class_method(
			mrb, mod, "attach_session", run_tmux_attach,
			MRB_ARGS_REQ(1)
			);
}

void
mrb_tmux_launcher_ext_gem_final(mrb_state *mrb) {};

struct RClass* getMod(mrb_state *mrb) {
	return mrb_module_get(mrb, MODULE_NAME);
}
