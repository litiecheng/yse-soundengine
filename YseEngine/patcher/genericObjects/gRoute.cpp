#include "gRoute.h"
#include "../pObjectList.hpp"

using namespace YSE::PATCHER;
#define className gRoute

CONSTRUCT() {
  ADD_IN_0;
  REG_BANG_IN(SetBangValue);
  REG_INT_IN(SetIntValue);
  REG_FLOAT_IN(SetFloatValue);
  REG_LIST_IN(SetListValue);

  REG_PARM_CLEAR;
  REG_PARM_PARSE;

  ADD_PARAM(list);
}

PARM_CLEAR() {
  outputs.clear();
}

PARM_PARSE() {
  while (outputs.size() < list.size() + 1) {
    ADD_OUT_ANY;
  }
}

BANG_IN(SetBangValue) {
  for (int i = 0; i < list.size(); i++) {
    if (list[i].compare("bang") == 0) {
      outputs[i].SendBang();
      return;
    }
  }
  outputs.back().SendBang();
}

INT_IN(SetIntValue) {
  std::string s = std::to_string(value);
  for (int i = 0; i < list.size(); i++) {
    if (list[i].compare(s) == 0) {
      outputs[i].SendInt(value);
      return;
    }
  }
  outputs.back().SendInt(value);
}

FLOAT_IN(SetFloatValue) {
  std::string s = std::to_string(value);
  for (int i = 0; i < list.size(); i++) {
    if (list[i].compare(s) == 0) {
      outputs[i].SendFloat(value);
      return;
    }
  }
  outputs.back().SendFloat(value);
}

LIST_IN(SetListValue) {
  // only compare first list item (string until first space)
  std::string token;
  size_t pos = 0;
  if ((pos = value.find(" ")) != std::string::npos) {
    token = value.substr(0, pos);
  }
  else {
    token = value;
  }

  for (int i = 0; i < list.size(); i++) {
    if (list[i].compare(token) == 0) {
      outputs[i].SendList(value);
      return;
    }
  }
  outputs.back().SendList(value);
}