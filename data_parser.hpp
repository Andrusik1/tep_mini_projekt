#pragma once

#include <expected>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string_view>

#include "instance_data.hpp"
#include "utils/error.hpp"

class DataParser {
 public:
  std::expected<InstanceData, Error> Parse(const std::filesystem::path& file_path);

 private:
  static constexpr std::string_view DIMENSION_TEXT = "DIMENSION";
  static constexpr std::string_view CAPACITY_TEXT = "CAPACITY";
  static constexpr std::string_view PERMUTATION_TEXT = "PERMUTATION";
  static constexpr std::string_view NODE_COORD_SECTION_TEXT = "NODE_COORD_SECTION";
  static constexpr std::string_view DEMAND_SECTION_TEXT = "DEMAND_SECTION";
  static constexpr std::string_view DEPOT_SECTION_TEXT = "DEPOT_SECTION";
  static constexpr std::string_view END_TEXT = "EOF";

  std::expected<void, Error> ValidateFile(const std::filesystem::path& file_path);
  std::expected<std::ifstream, Error> OpenFile(const std::filesystem::path& file_path);

  int ParseDimension(std::istringstream& iss);
  int ParseCapacity(std::istringstream& iss);
  std::vector<int> ParsePermutation(std::istringstream& iss);
  void ParseNodeCoord(std::ifstream& file, std::vector<Point>& coordinates, int dimension);
  void ParseDemand(std::ifstream& file, std::vector<int>& demands, int dimension);
  int ParseDepot(std::ifstream& file);
};
