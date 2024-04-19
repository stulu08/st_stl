#pragma once

#include <stulu/networking.h>
#include <iostream>
#include <thread>

namespace net = stulu::networking;

inline void client_test(const stulu::string& name) {
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

inline void server_test(const stulu::string& name) {
	net::address add = net::address(name, "80");
	net::socket sock = net::socket(add);

	int s1 = sock.create();
	int s2 = sock.bind();
	int s3 = sock.listen();


	net::socket client;
	int s4 = sock.accept(&client);

	net::buffer data;
	data.resize(2000);
	int rec_size = 0;
	int s5 = client.receive(data, &rec_size);

	Sleep(300);
	std::cout << "Received " << rec_size << " bytes: ";
	for (int i = 0; i < rec_size; i++) {
		std::cout << data[i];
	}
}

inline void socket_tests() {
	net::SSA::Startup();
	stulu::string name;
	int status = net::address::GetHostName(name);
	if (status != 0)
		std::cout << net::SSA::GetLastError();

	auto& se = std::thread(server_test, name);
	auto& cl = std::thread(client_test, name);

	cl.join();
	se.join();
	net::SSA::Cleanup();

}