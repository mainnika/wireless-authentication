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
	LOG(INFO) << "Trying to connect to 192.168.1.7";
	TcpClient::connect(&this->get_loop(), this, "192.168.1.7", 12343);
}

void ThreadedClient::on_stop()
{
	LOG(INFO) << "Client Thread stopped";

}