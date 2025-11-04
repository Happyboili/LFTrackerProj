#ifndef LEFTOVER_TRACKER_H_
#define LEFTOVER_TRACKER_H_


// 4/14/25
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
