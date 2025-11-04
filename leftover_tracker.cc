// 4/14/25
// @Happyboili
#include "leftover_tracker.h"

#include <algorithm>

bool LeftoverTracker::AddRecord(const LeftoverRecord& record) {
  auto it = std::find(records_.begin(), records_.end(), record);
  if (it != records_.end()) return false;
  records_.push_back(record);
  return true;
}

bool LeftoverTracker::DeleteRecord(const LeftoverRecord& record) {
  auto it = std::find(records_.begin(), records_.end(), record);
  if (it != records_.end()) {
    records_.erase(it);
    return true;
  }
  return false;
}

const std::vector<LeftoverRecord>& LeftoverTracker::GetRecords() const {
  return records_;
}

LeftoverReport LeftoverTracker::GenerateReport() const {
  return LeftoverReport(records_);
}
