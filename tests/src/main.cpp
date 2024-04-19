#include "tests/vector_tests.h"
#include "tests/array_tests.h"
#include "tests/unique_ptr_tests.h"
#include "tests/string_tests.h"

#include <stulu/stulu.h>

#include <vector>
#include <string>
#include <iostream>
#include <thread>

namespace net = stulu::networking;

void client(const stulu::string& name) {
	Sleep(1000);
	net::address add = net::address(name, "80");
	net::socket client = net::socket(add);
	int s1 = client.create();
	int s2 = client.connect(add);
	int sent = 0;
	
	net::buffer buffer;
	buffer.push_back('H');
	buffer.push_back('E');
	buffer.push_back('L');
	buffer.push_back('L');
	buffer.push_back('O');
	buffer.push_back('\0');
	int s3 = client.send(buffer, &sent);

	std::cout << "Sent: " << sent << " bytes" << "\n";
}

int main(int argc, char** argv) {
	net::SSA::Startup();
	stulu::string name;
	int status = net::address::GetHostName(name);
	if (status != 0)
		std::cout << net::SSA::GetLastError();


	net::address add = net::address(name, "80");
	net::socket sock = net::socket(add);
	
	int s1 = sock.create();
	int s2 = sock.bind();
	int s3 = sock.listen();

	auto& thread = std::thread(client, name);

	net::socket client;
	int s4 = sock.accept(&client);

	net::buffer data;
	data.resize(2000);
	int rec_size = 0;
	int s5 = client.receive(data, &rec_size);

	Sleep(1000);
	std::cout << "Received " << rec_size << " bytes: ";
	for (int i = 0; i < rec_size; i++) {
		std::cout << data[i];
	}

	thread.join();

	return 0;
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}