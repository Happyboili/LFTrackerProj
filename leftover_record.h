#ifndef LEFTOVER_RECORD_H_
#define LEFTOVER_RECORD_H_

// Marco Deng
// CPSC 121L-17
// 4/14/25
// mdeng777@csu.fullerton.edu
// @Happyboili
#include <string>
class LeftoverRecord {
 public:
  LeftoverRecord() : qty_in_oz_(0.0), cost_(0.0) {}
  LeftoverRecord(const std::string& date, const std::string& meal,
                 const std::string& food_name, double qty_in_oz,
                 const std::string& leftover_reason,
                 const std::string& disposal_mechanism, double cost)
      : date_(date),
        meal_(meal),
        food_name_(food_name),
        qty_in_oz_(qty_in_oz),
        leftover_reason_(leftover_reason),
        disposal_mechanism_(disposal_mechanism),
        cost_(cost) {}

  std::string GetDate() const { return date_; }
  std::string GetMeal() const { return meal_; }
  std::string GetFoodName() const { return food_name_; }
  double GetQuantity() const { return qty_in_oz_; }
  std::string GetLeftoverReason() const { return leftover_reason_; }
  std::string GetDisposalMechanism() const { return disposal_mechanism_; }
  double GetCost() const { return cost_; }

  void SetDate(const std::string& date) { date_ = date; }
  void SetMeal(const std::string& meal) { meal_ = meal; }
  void SetFoodName(const std::string& food_name) { food_name_ = food_name; }
  void SetQuantity(double qty) { qty_in_oz_ = qty; }
  void SetLeftoverReason(const std::string& reason) {
    leftover_reason_ = reason;
  }
  void SetDisposalMechanism(const std::string& mechanism) {
    disposal_mechanism_ = mechanism;
  }
  void SetCost(double cost) { cost_ = cost; }

  bool operator==(const LeftoverRecord& other) const {
    return date_ == other.date_ && meal_ == other.meal_ &&
           food_name_ == other.food_name_ && qty_in_oz_ == other.qty_in_oz_ &&
           leftover_reason_ == other.leftover_reason_ &&
           disposal_mechanism_ == other.disposal_mechanism_ &&
           cost_ == other.cost_;
  }

 private:
  std::string date_;
  std::string meal_;
  std::string food_name_;
  double qty_in_oz_;
  std::string leftover_reason_;
  std::string disposal_mechanism_;
  double cost_;
};

#endif
