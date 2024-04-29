#include "exec.hpp"
#include <array>
#include <functional>
#include <memory>

#ifdef _WIN32
#define popen  _popen
#define pclose _pclose
#endif

std::string exec(const char *command) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, std::function<void(FILE *)>> pipe(
    popen(command, "r"),
    [](FILE *file) { pclose(file); }
  );
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
         nullptr) {
    result += buffer.data();
  }
  return result;
}
