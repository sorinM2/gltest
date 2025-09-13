#include "ProgramManager.h"
#include "shaders/program.h"

namespace programs 
{

namespace {
	utl::vector<program, false> programs;
}

program_id AddProgram()
{
	return programs.emplace_tombstone();
}

program* GetProgram(program_id id)
{
	assert(id < programs.size());
	return &programs[id];
}

}
