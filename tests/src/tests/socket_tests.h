#pragma once

#include <stulu/Networking.h>
#include <iostream>
#include <thread>
#include <chrono>

#include <shellapi.h>

namespace Net = Stulu::Networking;

#include <stulu/http/parser.h>

inline int simple_tcp_echo_test() {
	auto SSAData = Net::SSA::Startup();

	Net::Socket Socket;

	int Status = Socket.Connect("google.com", "80");
	if (Status != 0) {
		printf("Failed connecting: %d", Net::SSA::GetLastError());
		Net::SSA::Cleanup();
		return -1;
	}
	int bytesSent = Socket.Send("GET / HTTP/1.1\r\n\r\n");
	if (bytesSent == 0) {
		printf("Failed sending: %d", Net::SSA::GetLastError());
		Net::SSA::Cleanup();
		return -1;
	}

	Stulu::string Response = Socket.Receive();
	printf("Received answer (%d bytes):\n%s", (int)Response.size(), Response.c_str());
	
	Status = Socket.Close();

	Net::SSA::Cleanup();
	return Status;
}

inline void client_loop(const Net::Address& add) {
	Net::Socket client;

	while (true) {
		std::string cmd;
		std::cout << "client> ";
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
			CHAR path[MAX_PATH];
			GetModuleFileNameA(NULL, path, MAX_PATH);

			ShellExecuteA(NULL, "open", path, "-server", NULL, SW_SHOWDEFAULT);
		}

		if (cmd.rfind("connect", 0) == 0) {
			if (client) {
				std::cout << "client> Error client is already connected to: " << client.GetAddress().Hostname().c_str() << std::endl;
				continue;
			}
			int error = 0;
			client = Net::Socket();
			error = client.Connect(add);
			if (error != 0) {
				std::cout << "client> Client connect failed: " << Net::SSA::GetLastError() << std::endl;
				continue;
			}
			std::cout << "client> Client connected to: " << client.GetAddress().Hostname().c_str() << ':' << client.GetAddress().Port().c_str() << std::endl;
		}

		if (cmd.rfind("send", 0) == 0) {
			if (!client) {
				std::cout << "client> Error client socket is invalid" << std::endl;
				continue;
			}

			std::string msg;
			std::cout << "client> Msg: ";
			std::cin >> msg;

			int bytesSent = client.Send(msg.data(), (int)msg.size());

			if (bytesSent > 0) {
				std::cout << "client> Sent " << bytesSent << " bytes" << std::endl;
			}
			else if (bytesSent == 0) {
				std::cout << "client> Client could not send to server" << std::endl;
			}
			else if (bytesSent == INVALID_SOCKET) {
				std::cout << "client> Error while sending data" << Net::SSA::GetLastError() << std::endl;
			}
		}

		if (cmd.rfind("close", 0) == 0) {
			if (!client) {
				std::cout << "client> Error client socket is invalid" << std::endl;
				continue;
			}

			int error = client.Send<1>({ '\4' });

			if (error == 0) {
				std::cout << "client> Client could not send EOT to server" << std::endl;
			}
			else if (error == INVALID_SOCKET) {
				std::cout << "client> Error while sending EOT" << Net::SSA::GetLastError() << std::endl;
			}

			error = client.Close();
			if (error != 0) {
				std::cout << "client> Client close failed: " << Net::SSA::GetLastError() << std::endl;
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

				int error = client.Send<1>({ '\7' });
				if (error == 0) {
					std::cout << "client> Client could not send ping to server" << std::endl;
					break;
				}
				else if (error == INVALID_SOCKET) {
					std::cout << "client> Error while sending ping" << Net::SSA::GetLastError() << std::endl;
					break;
				}
				Net::array<1> data;
				error = client.Receive(data);
				if (error == 0) {
					std::cout << "client> Client could not get ping answer from server" << std::endl;
					break;
				}
				else if (error == INVALID_SOCKET) {
					std::cout << "client> Error while receiving ping answer from server" << Net::SSA::GetLastError() << std::endl;
					break;
				}

				if (data[0] != '\7') {
					std::cout << "client> Error: wrong data Received from ping answer" << std::endl;
					break;
				}
				auto timeEnd = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
				std::cout << "client> Reply from " << client.GetAddress().Hostname().c_str() << ": bytes=" << data.size() << " time=" << duration << "ms" << std::endl;
				Stulu::sleep(50);
			}

		}
	}
}

inline void server_loop(const Net::Address& add) {
	int error = 0;

	std::cout << "server> Starting server" << std::endl;
	Net::Socket sock = Net::Socket(add);

	error = sock.Bind();
	if (error != 0) {
		std::cout << "server> Server bind failed: " << Net::SSA::GetLastError() << std::endl;
		return;
	}

	error = sock.Listen();
	if (error != 0) {
		std::cout << "server> Server listen failed: " << Net::SSA::GetLastError() << std::endl;
		return;
	}


	Net::buffer data;
	data.resize(UINT16_MAX);

	std::cout << "server> Server running" << std::endl;

	while (true) {
		// connect
		Net::Socket client;
		error = sock.Accept(&client);
		if (error != 0) {
			std::cout << "server> Server accept failed: " << Net::SSA::GetLastError() << std::endl;
		}
		else {
			std::cout << "server> Accepted client: " << client.GetAddress().Hostname().c_str() << std::endl;
		}

		// Receive data
		while (client) {
			int bytesReceived = client.Receive(data);

			if (bytesReceived == 1) {
				// eot -> end of transmission
				if (data[0] == '\4') {
					break;
				}
				// ping
				if (data[0] == '\7') {
					int bytesSent = client.Send<1>({ '\7' });
					if (bytesSent == 0) {
						std::cout << "server> Server could not answer ping from client" << std::endl;
					}
					else if (bytesSent == INVALID_SOCKET) {
						std::cout << "server> Error while answering ping to client: " << Net::SSA::GetLastError() << std::endl;
					}
					std::cout << "server> Server answered ping with " << bytesSent << " bytes" << std::endl;
					continue;
				}
			}

			if (bytesReceived > 0) {
				std::cout << "server> Received " << bytesReceived << " bytes: " << std::string(data.data()) << std::endl;
			}
			else if (bytesReceived == 0) {
				std::cout << "server> Server could not Receive data from client" << std::endl;
			}
			else if (bytesReceived == INVALID_SOCKET) {
				std::cout << "server> Error while receiving data: " << Net::SSA::GetLastError() << std::endl;
				break;
			}
		}

		// close connection
		std::cout << "server> Client disconnected: " << client.GetAddress().Hostname().c_str() << std::endl;
		std::cin.get();
	}
}

inline int socket_tests(int argc) {
	auto SSAData = Net::SSA::Startup();

	std::cout << SSAData->szSystemStatus << std::endl;
	std::cout << SSAData->szDescription << std::endl;

	Stulu::string localHostName;
	int Status = Net::Address::GetHostName(localHostName);
	if (Status != 0)
		std::cout << Net::SSA::GetLastError();
	
	const Stulu::string remoteAddress = localHostName;
	const Stulu::string port = "80";
	
	const Net::Address add(remoteAddress, port);

	if (argc > 1) {
		server_loop(add);
	}
	else {
		client_loop(add);
	}

	Net::SSA::Cleanup();

	return 0;
}