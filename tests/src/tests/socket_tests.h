#pragma once

#include <stulu/networking.h>
#include <iostream>
#include <thread>
#include <chrono>

namespace net = stulu::networking;

static stulu::unique_ptr<net::SSA> ssa;

inline int simple_tcp_echo_test() {
	ssa = net::SSA::Create();

	net::socket socket;

	int status = socket.connect("github.com", "80");
	if (status != 0) {
		printf("Failed connecting: %d", ssa->getLastError());
		ssa->cleanup();
		return -1;
	}
	int bytesSent = socket.send("GET / HTTP/1.1\r\n\r\n");
	if (bytesSent == 0) {
		printf("Failed sending: %d", ssa->getLastError());
		ssa->cleanup();
		return -1;
	}

	stulu::string response = socket.receive();
	printf("Received answer (%d bytes):\n%s", (int)response.size(), response.c_str());
	
	ssa->cleanup();
}

inline static stulu::vector<stulu::string> net_msg_pool;
inline static bool net_running = false;

#define NET_LOG(msg) net_msg_pool.push_back(msg)

inline void server_test(const net::address& add) {
	std::stringstream stream;
	int error = 0;

	NET_LOG("Starting server");

	net::socket sock = net::socket(add);

	error = sock.bind();
	if (error != 0) {
		stream << "Server bind failed: " << net::SSA::GetLastError();
		NET_LOG(stream.str().c_str());
		return;
	}

	error = sock.listen();
	if (error != 0) {
		stream << "Server listen failed: " << net::SSA::GetLastError();
		NET_LOG(stream.str().c_str());
		return;
	}

	net::buffer data;
	data.resize(UINT16_MAX);

	NET_LOG("Server running");
	while (net_running) {
		stream = std::stringstream();
		// connect
		net::socket client;
		error = sock.accept(&client);
		if (error != 0) {
			stream << "Server accept failed: " << net::SSA::GetLastError();
		}
		else {
			stream << "Accepted client: " << client.get_address().hostname().c_str();
		}
		NET_LOG(stream.str().c_str());

		// receive data
		while (client) {
			int bytesReceived = client.receive(data);
			stream = std::stringstream();

			if (bytesReceived == 1) {
				// eot -> end of transmission
				if (data[0] == '\4') {
					break;
				}
				// ping
				if (data[0] == '\7') {
					int bytesSent = client.send<1>({ '\7' });
					if (bytesSent == 0) {
						NET_LOG("Server could answer ping from client");
					}
					else if (bytesSent == INVALID_SOCKET) {
						stream << "Error while answering ping to client: " << net::SSA::GetLastError();
						NET_LOG(stream.str().c_str());
					}
					continue;
				}
			}

			if (bytesReceived > 0) {
				stream << "Received " << bytesReceived << " bytes: " << std::string(data.data());
			}
			else if (bytesReceived == 0) {
				stream << "Server could not receive data from client";
			}
			else if (bytesReceived == INVALID_SOCKET) {
				stream << "Error while receiving data: " << net::SSA::GetLastError();
			}
			NET_LOG(stream.str().c_str());
		}

		// close connection
		stream = std::stringstream();
		stream << "Client disconnected: " << client.get_address().hostname().c_str();
		NET_LOG(stream.str().c_str());
	}
}

inline void socket_tests() {
	ssa = net::SSA::Create();

	std::cout << ssa->get().szSystemStatus << std::endl;
	std::cout << ssa->get().szDescription << std::endl;

	stulu::string localHostName;
	int status = net::address::GetHostName(localHostName);
	if (status != 0)
		std::cout << ssa->getLastError();
	
	const stulu::string remoteAddress = localHostName;
	const stulu::string port = "80";
	
	std::thread serverThread;
	net::socket client;

	while (true) {
		std::string cmd;
		std::cout << "stl_socket_test> ";
		std::cin >> cmd;

		if (cmd.rfind("help", 0) == 0) {
			std::cout << "List of available commands:" << std::endl;
			std::cout << "\tstart\t-\tStarts the server" << std::endl;
			std::cout << "\tconnect\t-\tConnects the client to the server" << std::endl;
			std::cout << "\tsend\t-\tSends a custom message to the server" << std::endl;
			std::cout << "\tclose\t-\tCloses the connection to the server" << std::endl;
			std::cout << "\tping\t-\tMeasures the latency between client and server" << std::endl;
			std::cout << std::endl;
			continue;
		}

		if (cmd.rfind("start", 0) == 0) {
			if (serverThread.joinable()) {
				std::cout << "client> " << "Error server is already running: " << client.get_address().hostname().c_str() << std::endl;
				continue;
			}
			net_running = true;
			serverThread = std::thread(server_test, net::address(localHostName, port));
		}

		if (cmd.rfind("connect", 0) == 0) {
			if (client) {
				std::cout << "client> Error client is already connected to: " << client.get_address().hostname().c_str() << std::endl;
				continue;
			}
			int error = 0;
			client = net::socket();
			error = client.connect(net::address(remoteAddress, port));
			if (error != 0) {
				std::cout << "client> Client connect failed: " << ssa->getLastError() << std::endl;
				continue;
			}
			std::cout << "client> Client connected to: " << client.get_address().hostname().c_str() << ':' << client.get_address().port().c_str() << std::endl;
		}

		if (cmd.rfind("send", 0) == 0) {
			if (!client) {
				std::cout << "client> Error client socket is invalid" << std::endl;
				continue;
			}

			std::string msg;
			std::cout << "client> Msg: ";
			std::cin >> msg;

			int bytesSent = client.send(msg.data(), (int)msg.size());

			if (bytesSent > 0) {
				std::cout << "client> Sent " << bytesSent << " bytes" << std::endl;
			}
			else if(bytesSent == 0){
				std::cout << "client> Client could not send to server" << std::endl;
			}
			else if (bytesSent == INVALID_SOCKET) {
				std::cout << "client> Error while sending data" << ssa->getLastError() << std::endl;
			}
		}

		if (cmd.rfind("close", 0) == 0) {
			if (!client) {
				std::cout << "client> Error client socket is invalid" << std::endl;
				continue;
			}

			int error = client.send<1>({ '\4' });

			if (error == 0) {
				std::cout << "client> Client could not send EOT to server" << std::endl;
			}
			else if (error == INVALID_SOCKET) {
				std::cout << "client> Error while sending EOT" << ssa->getLastError() << std::endl;
			}

			error = client.close();
			if (error != 0) {
				std::cout << "client> Client close failed: " << ssa->getLastError() << std::endl;
				continue;
			}
		}
		
		if (cmd.rfind("ping", 0) == 0) {
			if (!client) {
				std::cout << "client> Error client socket is invalid" << std::endl;
				continue;
			}
			for (int i = 0; i < 8; i++) {
				auto timeStart = std::chrono::high_resolution_clock::now();

				int error = client.send<1>({ '\7' });
				if (error == 0) {
					std::cout << "client> Client could not send ping to server" << std::endl;
					break;
				}
				else if (error == INVALID_SOCKET) {
					std::cout << "client> Error while sending ping" << ssa->getLastError() << std::endl;
					break;
				}
				net::array<1> data;
				error = client.receive(data);
				if (error == 0) {
					std::cout << "client> Client could not get ping answer from server" << std::endl;
					break;
				}
				else if (error == INVALID_SOCKET) {
					std::cout << "client> Error while receiving ping answer from server" << ssa->getLastError() << std::endl;
					break;
				}

				if (data[0] != '\7') {
					std::cout << "client> Error: wrong data received from ping answer" << std::endl;
					break;
				}
				auto timeEnd = std::chrono::high_resolution_clock::now(); 
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
				std::cout << "client> Reply from " << client.get_address().hostname().c_str() << ": bytes=" << data.size() << " time=" << duration << "ms" << std::endl;
				stulu::sleep(50);
			}
			
		}

		std::cout << "client> Waiting for server message pool" << std::endl;
		stulu::sleep(1000);
		for (int i = 0; i < net_msg_pool.size(); i++) {
			std::cout << "server> " << net_msg_pool[i].c_str() << std::endl;
		}
		net_msg_pool.clear();
	}


	ssa->cleanup();
}