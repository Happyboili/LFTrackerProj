// Marco Deng
// CPSC 121L-17
// 4/14/25
// mdeng777@csu.fullerton.edu
// @Happyboili

#include "leftover_report.h"

#include <algorithm>
#include <unordered_map>

LeftoverReport::LeftoverReport(const std::vector<LeftoverRecord>& records) {
  ComputeReport(records);
}

void LeftoverReport::ComputeReport(const std::vector<LeftoverRecord>& records) {
  total_cost_ = 0.0;
  std::unordered_map<std::string, int> food_count;
  std::unordered_map<std::string, double> meal_cost;
  std::unordered_map<std::string, int> disposal_count;
  std::unordered_map<std::string, int> reason_count;

  for (const auto& record : records) {
    total_cost_ += record.GetCost();
    food_count[record.GetFoodName()]++;
    meal_cost[record.GetMeal()] += record.GetCost();
    disposal_count[record.GetDisposalMechanism()]++;
    reason_count[record.GetLeftoverReason()]++;
  }
  int max_food = 0;
  for (const auto& pair : food_count) {
    if (pair.second > max_food) max_food = pair.second;
  }
  for (const auto& pair : food_count) {
    if (pair.second == max_food) most_common_leftovers_.push_back(pair.first);
  }

  double max_meal_cost = 0.0;
  for (const auto& pair : meal_cost) {
    if (pair.second > max_meal_cost) max_meal_cost = pair.second;
  }
  for (const auto& pair : meal_cost) {
    if (pair.second == max_meal_cost) most_costly_meals_.push_back(pair.first);
  }

  int max_disposal = 0;
  for (const auto& pair : disposal_count) {
    if (pair.second > max_disposal) max_disposal = pair.second;
  }
  for (const auto& pair : disposal_count) {
    if (pair.second == max_disposal)
      most_common_disposal_mechanisms_.push_back(pair.first);
  }

  int max_reason = 0;
  for (const auto& pair : reason_count) {
    if (pair.second > max_reason) max_reason = pair.second;
  }
  for (const auto& pair : reason_count) {
    if (pair.second == max_reason)
      most_common_leftover_reasons_.push_back(pair.first);
  }

  std::string dominant_reason = {};

  for (const auto& reason : most_common_leftover_reasons_) {
    if (reason == "Expired") {
      dominant_reason = "Expired";
      break;
    }
  }
  if (dominant_reason.empty() && !most_common_leftover_reasons_.empty()) {
    dominant_reason = most_common_leftover_reasons_[0];
  }
  suggested_strategies_.clear();

  suggested_strategies_.push_back("Buy less food");
  suggested_strategies_.push_back("Cook small servings");

  if (dominant_reason == "Expired") {
    suggested_strategies_.push_back("Donate before expiration");
  } else {
    suggested_strategies_.push_back("Recycle left overs");
  }

  std::sort(suggested_strategies_.begin(), suggested_strategies_.end());
  suggested_strategies_.erase(
      std::unique(suggested_strategies_.begin(), suggested_strategies_.end()),
      suggested_strategies_.end());
}
