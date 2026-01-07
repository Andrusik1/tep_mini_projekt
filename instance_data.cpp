#include "instance_data.hpp"

#include <iostream>
#include <sstream>

#include "utils/error.hpp"

InstanceData::InstanceData(int capacity, std::vector<std::unique_ptr<Node>>&& nodes, std::vector<int>&& permutation,
                           int depot_id)
    : capacity_(capacity),
      nodes_(std::move(nodes)),
      permutation_(std::move(permutation)),
      depot_id_(depot_id),
      number_of_clients_(static_cast<int>(permutation.size())),
      dimension_(static_cast<int>(nodes.size())) {}

std::expected<InstanceData, Error> InstanceData::Create(int capacity, std::vector<Point>& coordinates,
                                                        std::vector<int>& demands, int depot_id,
                                                        std::vector<int>&& initial_permutation) {
  using enum ParseError::Type;
  if (!IsValid(capacity, coordinates, demands, depot_id)) {
    return std::unexpected(Error(ParseError(INVALID_DATA, "Invalid instance data")));
  }

  std::vector<std::unique_ptr<Node>> nodes;
  size_t number_of_nodes = coordinates.size();
  nodes.reserve(number_of_nodes);

  for (size_t i = 0; i < number_of_nodes; ++i) {
    auto node = std::make_unique<Node>();
    node->location = coordinates[i];
    node->demand = demands[i];
    nodes.push_back(std::move(node));
  }

  return InstanceData(capacity, std::move(nodes), std::move(initial_permutation), depot_id);
}

bool InstanceData::IsValid(int capacity, const std::vector<Point>& coordinates, const std::vector<int>& demands,
                           int depot_id) {
  if (capacity <= 0) {
    return false;
  }

  if (coordinates.size() != demands.size()) {
    return false;
  }

  if (depot_id < 0 || depot_id >= static_cast<int>(coordinates.size())) {
    return false;
  }

  return true;
}

std::string InstanceData::ToString() const {
  std::ostringstream oss;
  oss << "InstanceData:\n";
  oss << "  Capacity: " << capacity_ << "\n";
  oss << "  Nodes: " << nodes_.size() << "\n";
  oss << "  Initial permutation size: " << permutation_.size() << "\n";

  for (size_t i = 0; i < nodes_.size(); ++i) {
    oss << "  Node " << i << ": ";
    oss << "pos=(" << nodes_[i]->location.x << ", " << nodes_[i]->location.y << "), ";
    oss << "demand=" << nodes_[i]->demand;
    oss << "\n";
  }

  return oss.str();
}
