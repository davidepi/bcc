#include "statement.hpp"

Statement::Statement() : offset(0x0), args_at(0)
{
}

int Statement::get_offset() const
{
    return offset;
}
std::string Statement::get_command() const
{
    return instruction;
}

std::string Statement::get_mnemonic() const
{
    return instruction.substr(0, args_at);
}

std::string Statement::get_args() const
{
    if(args_at >= instruction.length())
    {
        return std::string();
    }

    return instruction.substr(args_at + 1, std::string::npos);
}

Statement::Statement(uint64_t offset, std::string opcode)
    : offset(offset), instruction(std::move(opcode))
{
    args_at = instruction.find_first_of(' ');
    if(args_at == std::string::npos)
    {
        args_at = instruction.length();
    }
}
