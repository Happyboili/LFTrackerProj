#ifndef LEFTOVER_TRACKER_H_
#define LEFTOVER_TRACKER_H_

// Marco Deng
// CPSC 121L-17
// 4/14/25
// mdeng777@csu.fullerton.edu
// @Happyboili
#include <vector>

#include "leftover_record.h"
#include "leftover_report.h"

class LeftoverTracker {
 public:
  bool AddRecord(const LeftoverRecord& record);

  bool DeleteRecord(const LeftoverRecord& record);

  const std::vector<LeftoverRecord>& GetRecords() const;

  LeftoverReport GenerateReport() const;

 private:
  std::vector<LeftoverRecord> records_;
};

#endif
