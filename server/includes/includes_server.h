#ifndef __INCLUDES_SERVER_H__
#define __INCLUDES_SERVER_H__

#define MAX_LEN 1024
#define NICKNAME_MAX_LEN 128

#include <stdlib.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include "color.h"
#include "display.h"
#include "libmy.h"
#include "common.h"

#include "client.h"
#include "clients_list.h"
#include "server.h"
#include "sending.h"
#include "command.h"
#include "server_command.h"
#include "channel.h"

#endif
