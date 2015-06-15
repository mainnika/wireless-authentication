#include "ThreadedClient.h"

#include <easylogging++.h>

ThreadedClient::ThreadedClient()
{
}


ThreadedClient::~ThreadedClient()
{
}

void ThreadedClient::on_run()
{
	TcpClient::connect(&this->get_loop(), this, "192.168.56.101", 12343);
}

void ThreadedClient::on_stop()
{
	LOG(INFO) << "Client Thread stopped";

}