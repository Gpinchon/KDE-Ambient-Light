#pragma once

#include <string>
#include <iostream>

std::string exec(const std::string &a_Cmd);

// logs message to standard output for DEBUG build
std::ostream &Log();

// logs message to standard output for DEBUG build
std::ostream &Error();