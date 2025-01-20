#pragma once

#include <Stulu/Stulu.h>

namespace Net = Stulu::Networking;

inline int http_parse_test() {
	auto data = Net::SSA::Startup();

	Stulu::string hostname;
	if (Net::Address::GetHostName(hostname) != Net::Success) {
		hostname = "localhost";
	}

	auto server = Net::Socket({ hostname, "80" });
	if (server.Bind() != Net::Success) {
		printf("Failed binding server socket!");
		return Net::SSA::GetLastError();
	}
	if (server.Listen() != Net::Success) {
		printf("Failed Listening to server address!");
		return Net::SSA::GetLastError();
	}
	printf("Started server on %s:%s\n", server.GetAddress().Hostname().c_str(), server.GetAddress().Port().c_str());

	Net::Socket client;
	if (server.Accept(&client) != Net::Success) {
		printf("Failed accepting client socket!");
		return Net::SSA::GetLastError();
	}
	printf("Accepted client: %s:%s\n", client.GetAddress().Hostname().c_str(), client.GetAddress().Port().c_str());

	Stulu::HTTP::Request Request = client.Receive(UINT16_MAX);

	printf("Request info:\n");
	printf("\tMethod: %s\n", Stulu::HTTP::MethodToString(Request.GetMethod()).c_str());
	printf("\tURI: %s\n", Request.GetURI().c_str());
	printf("\tVersion: %s\n", Request.GetVersion().c_str());
	printf("\tContent-Length: %llu\n", Request.GetContentLength());
	printf("\tConnection: %s\n", Request.GetLine("Connection").c_str());
	printf("\nRaw request:\n%s\n\n", Request.Build().c_str());


	Stulu::HTTP::Response Response;
	Response.SetVersion("HTTP/1.1");
	Response.SetStatus(Stulu::HTTP::Status::NotFound);
	Response.SetDate();
	Response.AddLine("Server", "Stulu/1.0.0 (Win64)");
	Response.AddLine("Connection", "Closed");
	Response.AddLine("Content-Type", "text/html; charset=iso-8859-1");
	Response.SetContent(R"(<html>
	<head>
		<title>404 Not Found</title>
	</head>
	<body>
		<div>
			<center>
				<p> 404 - Not Found </p>
			</center>
		</div>
	</body>
</html>
)");

	auto response_str = Response.Build();
	printf("Response:\n%s\n", response_str.c_str());

	if (client.Send(response_str) != response_str.length()) {
		printf("Failed sending response to client socket!");
		return Net::SSA::GetLastError();
	}

	if (client.Close() != Net::Success) {
		printf("Failed closing client socket!");
		return Net::SSA::GetLastError();
	}
	if (server.Close() != Net::Success) {
		printf("Failed closing server socket!");
		return Net::SSA::GetLastError();
	}

	Net::SSA::Cleanup();
	return 0;
}