#ifndef __NETWORK_TOOL_WIN_H__
#define __NETWORK_TOOL_WIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>
#include "data_def.h"
#include <iostream>
#include <winsock2.h>
#include "cocos2d.h"

#pragma comment(lib, "ws2_32.lib")

class NetworkTool {
public:
	static NetworkTool* get_instance() {
		if (s_instance == NULL) {
			s_instance = new NetworkTool();
		}
		return s_instance;
	}

	static int destroy_instance() {
		if (s_instance != NULL) {
			delete s_instance;
		}
		return 0;
	}

	int connect_server() {
/*	
		struct sockaddr_in server_address;
		if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			printf("create socket error!\n");
			return -1;
		}

		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(ip_port);
		if(inet_pton(AF_INET, ip_addr.c_str(), &server_address.sin_addr) <= 0) {
			printf("inet_pton error!\n");
			return -1;
		}

		if(connect(socket_fd, (sockaddr *)&server_address, sizeof(server_address)) == -1) {
			printf("connect error!\n");
			return -1;
		
		}
		return 0;*/

		if(WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
			printf("init windows socket failed\n");
			return -1;
		}

		client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (client_socket == INVALID_SOCKET) {
			printf("create socket failed\n");
			return -1;
		}
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
		server_addr.sin_port = htons(ip_port);
		memset(server_addr.sin_zero, 0x00, 8);
		int ret = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
		if(ret == SOCKET_ERROR) {
			printf("connect error!\n");
			return -1;
		} else {
			printf("connect successful!\n");
			return 0;
		}
	}

	int disconnect_server() {
		closesocket(client_socket);
		WSACleanup();
		return 0;
	}
	int login(std::string user_name, UserData * user_data);
	
	int get_frame_sync_data(FrameSyncData * sync_data);
	
	int push_frame_uplo_data(FrameUploData * uplo_data);

	int start_game_req(std::string user_name);

	int start_game_resp(std::string *position, UserData *compe_name);

	int push_user_data(UserData * user_data);

	int get_user_data(std::string user_name, UserData * user_data);

private:
	
	std::string ip_addr;
	u_short ip_port;
	NetworkTool() {
		//ip_addr = "192.168.3.221";
		ip_addr = "218.244.157.227";
		ip_port = 8897;
	}
	~NetworkTool() {
	}

	WSADATA ws;
	SOCKET client_socket;
	struct sockaddr_in server_addr;
	int addrlen;
	HANDLE h_thread;

	static NetworkTool* s_instance;

};


#endif
