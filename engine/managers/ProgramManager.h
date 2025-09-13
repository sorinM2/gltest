#pragma once 
#include "core/common.h"

namespace programs 
{
	class program;
	
	program_id AddProgram();
	program* GetProgram(program_id id);	
}
