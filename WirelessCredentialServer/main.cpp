#include "Server.h"
#include "Thread.h"
#include "Config.h"

#include "tools/easylogging++.h"

#include <google/protobuf/stubs/common.h>

#include <iostream>
#include <getopt.h>
#include <uv.h>

INITIALIZE_EASYLOGGINGPP

void signal_handler(uv_signal_t *handle, int signum)
{
	uv_loop_t *main = static_cast<uv_loop_t*> (handle->data);
	uv_stop(main);
}

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	google::protobuf::LogSilencer no_protobuf_errors;

	LOG(INFO) << "Using libuv " << UV_VERSION_MAJOR << "." << UV_VERSION_MINOR;

	Config config(
		"0.0.0.0",
		12343
	);

	char opt;
	while ((opt = getopt(argc, argv, "ph:")) != -1)
	{
		switch (opt)
		{
			case 'p':
				config.port = std::stoul(optarg);
				break;

			case 'h':
				config.host.assign(optarg);
				break;

			default:
				LOG(ERROR) << "Invalid usage";
				return -1;
		}
	}

	Server server(config);
	server.start();

	uv_loop_t main;
	uv_signal_t sigint, sigterm;
	uv_loop_init(&main);
	uv_signal_init(&main, &sigint);
	uv_signal_init(&main, &sigterm);

	sigint.data = &main;
	sigterm.data = &main;

	uv_signal_start(&sigint, &signal_handler, SIGINT);
	uv_signal_start(&sigterm, &signal_handler, SIGTERM);

	uv_run(&main, UV_RUN_DEFAULT);

	uv_signal_stop(&sigint);
	uv_signal_stop(&sigterm);

	server.stop();

	google::protobuf::ShutdownProtobufLibrary();

	LOG(INFO) << "Done";
}