#include "ProgramManager.h"
namespace programs 
{

utl::vector<program, false> programs;

unsigned int AddProgram()
{
	return programs.emplace_tombstone();
}

program* GetProgram(unsigned int id)
{
	return &programs[id];
}

}
