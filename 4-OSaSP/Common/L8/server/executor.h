#pragma once

#include "command.h"
#include "session.h"
#include "message.h"

void execute(command_t* cmd, session_state_t* state, message_t* response);
