#ifndef __FUNTION_H__
#define __FUNTION_H__

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "config.h"

extern void user_register(USER *usermsg);
extern void user_login(USER *usermsg);

#endif
