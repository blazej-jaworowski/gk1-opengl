#pragma once

#include <cstdint>
#include <string>

class Shader {
  private:
    uint32_t id;
    uint32_t type;

  public:
    Shader(std::string filename, uint32_t type);
    uint32_t get_id() const;
};