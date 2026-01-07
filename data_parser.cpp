#include "data_parser.hpp"

#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

#include "instance_data.hpp"
#include "utils/error.hpp"

namespace fs = std::filesystem;

std::expected<InstanceData, Error> DataParser::Parse(const fs::path& file_path) {
  if (auto result = ValidateFile(file_path); !result) {
    return std::unexpected(std::move(result.error()));
  }

  auto result = OpenFile(file_path);
  if (!result) {
    return std::unexpected(std::move(result.error()));
  }
  std::ifstream file = std::move(result.value());

  int dimension = 0;
  int capacity = 0;
  int depot_id = 0;
  std::vector<Point> coordinates;
  std::vector<int> demands;
  std::vector<int> initial_permutation;
  std::string line;

  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    if (line.find(DIMENSION_TEXT) != std::string::npos) {
      std::istringstream iss(line);
      dimension = ParseDimension(iss);
      // NOTE: Node IDs are 1-based
      coordinates.resize(dimension);
      demands.resize(dimension);
    } else if (line.find(CAPACITY_TEXT) != std::string::npos) {
      std::istringstream iss(line);
      capacity = ParseCapacity(iss);
    } else if (line.find(PERMUTATION_TEXT) != std::string::npos) {
      std::istringstream iss(line);
      initial_permutation = ParsePermutation(iss);
    } else if (line.find(NODE_COORD_SECTION_TEXT) != std::string::npos) {
      ParseNodeCoord(file, coordinates, dimension);
    } else if (line.find(DEMAND_SECTION_TEXT) != std::string::npos) {
      ParseDemand(file, demands, dimension);
    } else if (line.find(DEPOT_SECTION_TEXT) != std::string::npos) {
      depot_id = ParseDepot(file);
    } else if (line.find(END_TEXT) != std::string::npos) {
      break;
    }
  }

  return InstanceData::Create(capacity, coordinates, demands, depot_id, std::move(initial_permutation));
}

std::expected<void, Error> DataParser::ValidateFile(const fs::path& file_path) {
  using enum FileError::Type;

  if (!fs::exists(file_path)) {
    return std::unexpected(Error(FileError(NOT_FOUND, file_path.string())));
  }

  if (!fs::is_regular_file(file_path)) {
    return std::unexpected(Error(FileError(INVALID_TYPE, file_path.string())));
  }

  if (fs::is_empty(file_path)) {
    return std::unexpected(Error(FileError(EMPTY, file_path.string())));
  }

  return {};
}

std::expected<std::ifstream, Error> DataParser::OpenFile(const std::filesystem::path& file_path) {
  using enum FileError::Type;
  std::ifstream file(file_path);

  if (!file.is_open()) {
    return std::unexpected(Error(FileError(ACCESS_DENIED, file_path.string())));
  }
  return file;
}

int DataParser::ParseDimension(std::istringstream& iss) {
  iss.ignore(std::numeric_limits<std::streamsize>::max(), ':');
  int dimension = 0;
  iss >> dimension;
  return dimension;
}

int DataParser::ParseCapacity(std::istringstream& iss) {
  iss.ignore(std::numeric_limits<std::streamsize>::max(), ':');
  int capacity = 0;
  iss >> capacity;
  return capacity;
}

std::vector<int> DataParser::ParsePermutation(std::istringstream& iss) {
  iss.ignore(std::numeric_limits<std::streamsize>::max(), ':');
  std::vector<int> permutation;
  int val = 0;
  while (iss >> val) {
    permutation.push_back(val - 1);  // NOTE:: NODES FROM FILE ARE CONVERTED TO [0,1,2 ...]
  }
  return permutation;
}

void DataParser::ParseNodeCoord(std::ifstream& file, std::vector<Point>& coordinates, int dimension) {
  for (int i = 0; i < dimension; ++i) {
    int node_id = 0;
    Point point{};
    file >> node_id >> point.x >> point.y;

    if (node_id > 0 && node_id <= static_cast<int>(coordinates.size())) {
      coordinates[node_id - 1] = point;  //  NODES FROM FILE ARE CONVERTED TO [0,1,2 ...]
    }
  }
}

void DataParser::ParseDemand(std::ifstream& file, std::vector<int>& demands, int dimension) {
  for (int i = 0; i < dimension; ++i) {
    int node_id = 0;
    int demand = 0;
    file >> node_id >> demand;

    if (node_id > 0 && node_id <= static_cast<int>(demands.size())) {
      demands[node_id - 1] = demand;  //  NODES FROM FILE ARE CONVERTED TO [0,1,2 ...]
    }
  }
}

int DataParser::ParseDepot(std::ifstream& file) {
  int depot_id = 0;
  file >> depot_id;

  return depot_id - 1;  //  NODES FROM FILE ARE CONVERTED TO [0,1,2 ...]
}
