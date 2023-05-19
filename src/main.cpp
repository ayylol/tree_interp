#include <fstream>
#include <iostream>
#include <json.hpp>
#include <string>
#include <variant>
#include <format>

using json = nlohmann::json;

json data;

struct Param {
  json &data;
  std::string name;
  std::variant<int, float> start;
  int num_steps;
  std::variant<int, float> step_size;
  bool is_int;
};

std::vector<Param> interp_params;

void init_params(json &option, std::string name);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: ./tree_interp <interp.json>" << std::endl;
    return 1;
  }

  // Parse data
  data = json::parse(std::ifstream(argv[1]));
  std::string path_prefix = data.at("image_path");
  init_params(data, "root");
  if (interp_params.size() != 2) {
    std::cout << "Must interpolate exactly 2 parameters" << std::endl;
    return 1;
  }

  // Initialize Parameters
  for (auto param : interp_params) {
    if (param.is_int) {
      param.data = std::get<int>(param.start);
    }else{
      param.data = std::get<float>(param.start);
    }
  }
  // Interpolate Parameters
#define STEP(A)                                                                \
  if (interp_params[A].is_int) {                                               \
    interp_params[A].data = (int)interp_params[A].data +                       \
                            std::get<int>(interp_params[A].step_size);         \
  } else {                                                                     \
    interp_params[A].data = (float)interp_params[A].data +                     \
                            std::get<float>(interp_params[A].step_size);       \
  }
#define RESET(A)                                                               \
  if (interp_params[A].is_int) {                                               \
    interp_params[A].data = std::get<int>(interp_params[A].start);             \
  } else {                                                                     \
    interp_params[A].data = std::get<float>(interp_params[A].start);           \
  }
  for (int i = 0; i < interp_params[0].num_steps; i++) {
    RESET(1);
    for (int j = 0; j < interp_params[1].num_steps; j++) {
      std::string val1 =
          interp_params[0].is_int
              ? std::to_string((int)interp_params[0].data)
              : std::to_string((float)interp_params[0].data);
      std::string val2 =
          interp_params[1].is_int
              ? std::to_string((int)interp_params[1].data)
              : std::to_string((float)interp_params[1].data);
      std::string file_name = interp_params[0].name + "-" + val1 + "-" +
                              interp_params[1].name + "-" + val2;

      std::ofstream out(("output/" + file_name + ".json").c_str());
      data.at("image_path") = path_prefix + file_name + "_";
      out << data;
      out.close();
      STEP(1);
    }
    STEP(0);
  }

  return 0;
}

void init_params(json &option, std::string name) {
  if (option.is_object()) {
    if (option.contains("interp")) {
      interp_params.push_back(Param{
          .data = option, .name = name, .num_steps = option.at("interp")[1]});
      if (option.at("interp")[0].is_number_integer()) {
        interp_params.back().is_int = true;
        interp_params.back().start = (int)option.at("interp")[0];
        interp_params.back().step_size = (int)option.at("interp")[2];
      } else {
        interp_params.back().is_int = false;
        interp_params.back().start = (float)option.at("interp")[0];
        interp_params.back().step_size =
            ((float)option.at("interp")[2] - (float)option.at("interp")[0]) /
            (float)option.at("interp")[1];
      }
    } else {
      for (json::iterator it = option.begin(); it != option.end(); ++it) {
        init_params(it.value(), it.key());
      }
    }
  }
}
