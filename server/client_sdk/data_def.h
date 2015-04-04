#ifndef __DATADEF_H__
#define __DATADEF_H__
#include <string>
#include "jsoncpp/json.h"

struct FootballerData {
	int strength;
	int speed;
	int accuracy;
};

struct UserData {
	std::string user_name;
	int potency;
	FootballerData player[4];
};

struct FrameSyncData {
	char status;
	int control[8];
	int a_shoot, a_pass;
	int b_shoot, b_pass;
	int a_tar_x, a_tar_y;
	int b_tar_x, b_tar_y;

	unsigned int buff[2];
	int to_binary() {
		buff[0] = 0;
		buff[1] = 0;
		for(int i = 0; i < 8; i++) {
			buff[0] = buff[0] | (control[i] << (i*4));
			
		}
		if(status == 'e') {
			buff[1] = 1;
		}
		if(a_shoot) {
			buff[1] = buff[1] | (1 << 1);
		}
		if(a_pass) {
			buff[1] = buff[1] | (1 << 2);
		}
		if(b_shoot) {
			buff[1] = buff[1] | (1 << 3);
		}
		if(b_pass) {
			buff[1] = buff[1] | (1 << 4);
		}
		int tmp;
		if(a_tar_x == 0 && a_tar_y == 0) {
			buff[1] = buff[1] | (1 << 5);
			tmp = b_tar_x * 10000 + b_tar_y;
		}  else 
			tmp = a_tar_x * 10000 + a_tar_y;
		buff[1] = buff[1] | (tmp << 6);
		
		return 0;
	}

	int to_real() {
		for(int i = 0; i < 8; i++) {
			control[i] = ((15 << (4*i)) & buff[0]) >> (i*4);
		}
		if(buff[1] & 1) {
			status = 'e';
		} else {
			status = 'r';
		}
		buff[1] = buff[1] >> 1;
		if(buff[1] & 1) {
			a_shoot = 1;
		} else 
			a_shoot = 0;

		buff[1] = buff[1] >> 1;
		if(buff[1] & 1) {
			a_pass = 1;
		} else 
			a_pass = 0;

		buff[1] = buff[1] >> 1;
		if(buff[1] & 1) 
			b_shoot = 1;
		else 
			b_shoot = 0;

		buff[1] = buff[1] >> 1;
		if(buff[1] & 1) 
			b_pass = 1;
		else 
			b_pass = 0;

		buff[1] = buff[1] >> 1;
		if((buff[1] & 1) == 0) {
			buff[1] = buff[1] >> 1;
			a_tar_x = buff[1] / 10000;
			a_tar_y = buff[1] % 10000;
			b_tar_x = 0;
			b_tar_y = 0;
		} else {
			buff[1] = buff[1] >> 1;
			a_tar_x = 0;
			a_tar_y = 0;
			b_tar_x = buff[1] / 10000;
			b_tar_y = buff[1] % 10000;
		}
		return 0;
	}


};

struct FrameUploData {
	int control[4];
	int a_shoot;
	int a_pass;
	char status;
	int tar_x;
	int tar_y;

	unsigned int buff[2];
	int to_binary() {
		buff[0] = 0;
		buff[1] = 0;
		for (int i = 0; i < 4; i++) {
			buff[0] = buff[0] | (control[i] << (i*4));
		}

		if(status == 'e') {
			buff[1] = 1;
		}
		if(a_shoot) {
			buff[1] = buff[1] | (1 << 1);
			if(tar_x > 0 && tar_x <= 5000 && tar_y > 0 && tar_y <= 5000) {
				int tmp = tar_x * 10000 + tar_y;
				buff[1] = buff[1] | (tmp << 3);
			}
		}
		if(a_pass) {
			buff[1] = buff[1] | (1 << 2);
		}
		return 0;
	}

	int to_real() {
		for(int i = 0; i < 4; i++) {
			control[i] = ((15 << (4*i)) & buff[0]) >> (i*4);
		}
		if(buff[1] & 1) {
			status = 'e';
		} else {
			status = 'r';
		}
		buff[1] = buff[1] >> 1;
		if(buff[1] & 1) {
			a_shoot = 1;
		} else 
			a_shoot = 0;

		buff[1] = buff[1] >> 1;
		if(buff[1] & 1) {
			a_pass = 1;
		} else 
			a_pass = 0;
		buff[1] = buff[1] >> 1;
		tar_y = buff[1] % 10000;
		tar_x = buff[1] / 10000;
		return 0;
	}
};

#endif
