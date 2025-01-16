#pragma once

#include "../../../others/version/version.h"
#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Config Reader */

#define MAX_LINE_LENGTH 1024
#define MAX_SECTION_LENGTH 128
#define MAX_KEY_LENGTH 128
#define MAX_PATH_LENGTH 256

void Strip_Whitespace(char* str);

void Read_Config(const char* filename);

int Mkdir_Recursive(const char* path);

void Init();

/* Default Parameters */

#define def_l1_cap 1024
extern size_t l1_cap;
#define def_l2_cap 1024
extern size_t l2_cap;

#define def_listen_ip "127.0.0.1"
extern char listen_ip[20];
#define def_listen_port 20000
extern uint16_t listen_port;

#define def_log_path "/var/run/litelog/"
extern char log_path[MAX_PATH_LENGTH];
#define def_log_prefix ""
extern char log_prefix[MAX_PATH_LENGTH];
#define def_log_lines 65535
extern int log_lines;

#define def_ctl_recv_ip "127.0.0.1"
extern char ctl_recv_ip[20];
#define def_ctl_recv_port 20001
extern uint16_t ctl_recv_port;
#define def_ctl_send_ip "127.0.0.1"
extern char ctl_send_ip[20];
#define def_ctl_send_port 12347
extern uint16_t ctl_send_port;

#define def_keep_log_nums 10
extern int keep_log_nums;

#ifdef __cplusplus
}
#endif