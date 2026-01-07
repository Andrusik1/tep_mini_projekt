#pragma once

#include <expected>
#include <memory>
#include <string>
#include <vector>

#include "utils/error.hpp"

struct Point {
  double x;
  double y;
};

struct Node {
  Point location = {.x = 0.0, .y = 0.0};
  int demand = 0;
};

// TODO: wyjebać inline definicje powinny być w .cpp
class InstanceData {
 public:
  static std::expected<InstanceData, Error> Create(int capacity, std::vector<Point>& coordinates,
                                                   std::vector<int>& demands, int depot_id,
                                                   std::vector<int>&& permutation);

  [[nodiscard]] int GetCapacity() const { return capacity_; }

  [[nodiscard]] const std::vector<std::unique_ptr<Node>>& GetNodes() const { return nodes_; }

  [[nodiscard]] const std::vector<int>& GetInitialPermutation() const { return permutation_; }

  [[nodiscard]] int GetDepotId() const { return depot_id_; }

  [[nodiscard]] int GetNumberOfClients() const { return number_of_clients_; }

  [[nodiscard]] int GetDimension() const { return dimension_; }

  [[nodiscard]] std::string ToString() const;

 private:
  InstanceData(int capacity, std::vector<std::unique_ptr<Node>>&& nodes, std::vector<int>&& permutation, int depot_id);

  static bool IsValid(int capacity, const std::vector<Point>& coordinates, const std::vector<int>& demands,
                      int depot_id);

  int capacity_;
  std::vector<std::unique_ptr<Node>> nodes_;
  std::vector<int> permutation_;
  int depot_id_;
  int number_of_clients_;
  int dimension_;
};
