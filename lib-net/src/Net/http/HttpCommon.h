#pragma once
#include "glog/logging.h"
#include <memory>
#include <list>
#include <map>
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>

namespace Net {

using HandleReq = std::function<std::string(std::map<std::string, std::string> params)>;

} // namespace Net
