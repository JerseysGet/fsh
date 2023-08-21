#ifndef HEADERS_H_
#define HEADERS_H_

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#define MAX_COMMAND_PER_LINE 32
// #define DEBUG

#include "executing.h"
#include "parsing.h"
#include "pastevents.h"
#include "process_handling.h"
#include "proclore.h"
#include "prompt.h"
#include "util/command.h"
#include "util/history.h"
#include "util/process.h"
#include "util/queue.h"
#include "warp.h"

bool isNum(char* str);

#endif