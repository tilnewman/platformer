#ifndef JSON_WRAPPER_HPP_INCLUDED
#define JSON_WRAPPER_HPP_INCLUDED
//
// json-wrapper.hpp
//
#ifdef WIN32
#include "json.hpp"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wdocumentation-unknown-command"
#pragma GCC diagnostic ignored "-Wdocumentation-pedantic"
#include "json.hpp"
#pragma GCC diagnostic pop
#endif

#endif // JSON_WRAPPER_HPP_INCLUDED
