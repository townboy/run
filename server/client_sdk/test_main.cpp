#include <stdio.h>
#include <iostream>
#include "network_tool.h"

NetworkTool *p;
int connect() {
	p = NetworkTool::get_instance();
	p->connect_server();
	return 0;
}
int test_login() {
	std::string user_name;
	std::cin >> user_name;
	UserData user_data;
	p->login(user_name, &user_data);
	std::cout << "user_name " << user_data.user_name << std::endl;
	std::cout << "potency " << user_data.potency << std::endl;
	for (int i = 0; i < 4; i++) {
		std::cout << i+1 << " speed " << user_data.player[i].speed << std::endl;
		std::cout << i+1 << " strength " << user_data.player[i].strength << std::endl;
		std::cout << i+1 << " accuracy " << user_data.player[i].accuracy << std::endl;
	}
	return 0;
}
int test_start_game() {
	std::string user_name;
	std::cin >> user_name;
	p->start_game_req(user_name);
	printf("waiting...\n");
	std::string position;
	UserData user_data;
	p->start_game_resp(&position, &user_data);
	printf("position : %s\n", position.c_str());
	printf("match successful!\n");
	return 0;
}
int test_get_frame_sync_data() {
	FrameSyncData sync_data;
	p->get_frame_sync_data(&sync_data);
	for(int i = 0 ; i < 8; i++) {
		printf("sync_data %d , %d \n", i, sync_data.control[i]);
	}
	printf("%c %d %d %d %d\n",sync_data.status, sync_data.a_shoot, sync_data.a_pass, sync_data.b_shoot, sync_data.b_pass);
	return 0;
}
int test_push_frame_uplo_data() {
	FrameUploData uplo_data;
	for(int i = 0; i < 4; i++) {
		std::cin >> uplo_data.control[i];
	}
	getchar();
	std::cin >> uplo_data.status;
	std::cin >> uplo_data.a_shoot;
	std::cin >> uplo_data.a_pass;
	p->push_frame_uplo_data(&uplo_data);
	return 0;
}
int test_push_user_data() {
	UserData user_data;
	std::cin >> user_data.user_name;
	user_data.potency = 30;	
	for(int i = 0; i < 4; i++) {
		user_data.player[i].speed = 30;
		user_data.player[i].strength = 40;
		user_data.player[i].accuracy = 50;
	}
	p->push_user_data(&user_data);
	return 0;
}
int main() {
	connect();
	test_login();
	//test_push_user_data();
	test_start_game();
	while(1) {
	
		test_get_frame_sync_data();
		test_push_frame_uplo_data();
		//test_get_frame_sync_data();
	}
	return 0;
}
