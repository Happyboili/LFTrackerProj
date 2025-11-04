#ifndef LEFTOVER_REPORT_H_
#define LEFTOVER_REPORT_H_

// 4/14/25
// @Happyboili

#include <string>
#include <vector>

#include "leftover_record.h"

class LeftoverReport {
 public:
  LeftoverReport() : total_cost_(0.0) {}

  LeftoverReport(const std::vector<LeftoverRecord>& records);

  std::vector<std::string> GetMostCommonLeftovers() const {
    return most_common_leftovers_;
  }
  std::vector<std::string> GetMostCostlyLeftoverProducingMeals() const {
    return most_costly_meals_;
  }
  std::vector<std::string> GetMostCommonDisposalMechanisms() const {
    return most_common_disposal_mechanisms_;
  }
  std::vector<std::string> GetMostCommonLeftoverReasons() const {
    return most_common_leftover_reasons_;
  }
  double GetTotalCost() const { return total_cost_; }
  std::vector<std::string> GetSuggestedStrategies() const {
    return suggested_strategies_;
  }

 private:
  std::vector<std::string> most_common_leftovers_;
  std::vector<std::string> most_costly_meals_;
  std::vector<std::string> most_common_disposal_mechanisms_;
  std::vector<std::string> most_common_leftover_reasons_;
  double total_cost_;
  std::vector<std::string> suggested_strategies_;

  void ComputeReport(const std::vector<LeftoverRecord>& records);
};

#endif
