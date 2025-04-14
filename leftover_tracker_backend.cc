#include "leftover_tracker_backend.h"

#include <filesystem>

#include "leftover_record.h"
#include "leftover_report.h"
#include "server_utils/rapidjson/document.h"      // rapidjson's DOM-style API
#include "server_utils/rapidjson/prettywriter.h"  // for stringify JSON
#include "server_utils/rapidjson/rapidjson.h"
#include "server_utils/rapidjson/stringbuffer.h"  // wrapper of C stream for prettywriter as output
#include "server_utils/rapidjson/writer.h"

// Util function to convert a LeftoverRecord class object into a serialized
// JSON object. Used in saving data to JSON files
void SerializeLeftoverRecordToJSON(
    const LeftoverRecord& record,
    rapidjson::Writer<rapidjson::StringBuffer>* writer) {
  writer->StartObject();

  writer->String(kFileDataDate);  // DO NOT MODIFY
  std::string date = record.GetDate();
  writer->String(date.c_str());

  writer->String(kFileDataMeal);  // DO NOT MODIFY
  std::string meal = record.GetMeal();
  writer->String(meal.c_str());

  writer->String(kFileDataFoodName);  // DO NOT MODIFY
  std::string food_name = record.GetFoodName();
  writer->String(food_name.c_str());

  writer->String(kFileDataQtyInOz);  // DO NOT MODIFY
  double quantity = record.GetQuantity();
  writer->Double(quantity);

  writer->String(kFileDataLeftoverReason);  // DO NOT MODIFY
  std::string leftover_reason = record.GetLeftoverReason();
  writer->String(leftover_reason.c_str());

  writer->String(kFileDataFoodDisposalMechanism);  // DO NOT MODIFY
  std::string disposal_mechanism = record.GetDisposalMechanism();
  writer->String(disposal_mechanism.c_str());

  writer->String(kFileDataCost);  // DO NOT MODIFY
  double cost = record.GetCost();
  writer->Double(cost);

  writer->EndObject();
}

// Util function to convert a serialized JSON object in to a LeftoverRecord
// class object. Used in loading data from JSON files
LeftoverRecord DeserializeLeftoverRecordFromJSON(
    const rapidjson::Value& json_obj) {
  LeftoverRecord record;
  record.SetDate(json_obj[kFileDataDate].GetString());
  record.SetMeal(json_obj[kFileDataMeal].GetString());
  record.SetFoodName(json_obj[kFileDataFoodName].GetString());
  record.SetQuantity(json_obj[kFileDataQtyInOz].GetDouble());
  record.SetLeftoverReason(json_obj[kFileDataLeftoverReason].GetString());
  record.SetDisposalMechanism(
      json_obj[kFileDataFoodDisposalMechanism].GetString());
  record.SetCost(json_obj[kFileDataCost].GetDouble());

  return record;
}

// Util function to convert a LeftoverRecord class object into a Crow JSON
// object. Used in sending data over the web. Data is formatted in the JSON
// format.
crow::json::wvalue LeftoverRecordToCrowJSON(const LeftoverRecord& record) {
  crow::json::wvalue record_json({});

  std::string date = record.GetDate();
  record_json[kWebDataDate] = date;

  std::string meal = record.GetMeal();
  record_json[kWebDataMeal] = meal;

  std::string food_name = record.GetFoodName();
  record_json[kWebDataFoodName] = food_name;

  double quantity = record.GetQuantity();
  record_json[kWebDataQtyInOz] = quantity;

  std::string leftover_reason = record.GetLeftoverReason();
  record_json[kWebDataLeftoverReason] = leftover_reason;

  std::string disposal_mechanism = record.GetDisposalMechanism();
  record_json[kWebDataDisposalMechanism] = disposal_mechanism;

  double cost = record.GetCost();
  record_json[kWebDataCost] = cost;

  return record_json;
}

crow::json::wvalue LeftoverReportToCrowJSON(const LeftoverReport& report) {
  crow::json::wvalue report_json({});

  std::vector<std::string> most_common_disposal_mechanisms{
      report.GetMostCommonDisposalMechanisms()};

  report_json[kWebReportDataCommonDisposalMechanism] =
      most_common_disposal_mechanisms;

  std::vector<std::string> most_common_leftovers{
      report.GetMostCommonLeftovers()};

  report_json[kWebReportDataCommonLeftOver] = most_common_leftovers;

  std::vector<std::string> most_common_leftover_reasons{
      report.GetMostCommonLeftoverReasons()};

  report_json[kWebReportDataCommonLeftOverReason] =
      most_common_leftover_reasons;

  std::vector<std::string> most_costly_leftover_producing_meals{
      report.GetMostCostlyLeftoverProducingMeals()};

  report_json[kWebReportDataMostLeftOverProducingMeal] =
      most_costly_leftover_producing_meals;

  std::vector<std::string> suggested_strategies_to_reduce_leftover{
      report.GetSuggestedStrategies()};

  report_json[kWebReportDataSuggestedReductLeftover] =
      suggested_strategies_to_reduce_leftover;

  double total_cost_of_leftover = report.GetTotalCost();

  report_json[kWebReportDataLeftOverCost] = total_cost_of_leftover;

  return report_json;
}

// Util function to convert a crow query string into a LeftoverRecord class
// object.
LeftoverRecord QueryStringToLeftoverRecord(
    const crow::query_string& query_string) {
  LeftoverRecord record{};

  const char* date = query_string.get(kWebDataDate);
  if (date) record.SetDate(date);
  const char* meal = query_string.get(kWebDataMeal);
  if (meal) record.SetMeal(meal);
  const char* food_name = query_string.get(kWebDataFoodName);
  if (food_name) record.SetFoodName(food_name);
  const char* qtr_str = query_string.get(kWebDataQtyInOz);
  if (qtr_str) record.SetQuantity(std::stod(qtr_str));
  const char* reason = query_string.get(kWebDataLeftoverReason);
  if (reason) record.SetLeftoverReason(reason);
  const char* disposal = query_string.get(kWebDataDisposalMechanism);
  if (disposal) record.SetDisposalMechanism(disposal);
  const char* cost_str = query_string.get(kWebDataCost);
  if (cost_str) record.SetCost(std::stod(cost_str));

  return record;
}

bool LeftoverTrackerBackend::LoadRecordsFromJSONFile() {
  std::ifstream records_file{kLeftoverRecordsFilepath};
  if (!records_file.is_open()) {
    std::filesystem::remove(kLeftoverRecordsFilepath);
    return false;
  }

  std::stringstream buffer;
  buffer << records_file.rdbuf();

  std::string s = buffer.str();
  rapidjson::Document doc;

  if (s.empty()) {
    std::filesystem::remove(kLeftoverRecordsFilepath);
    return false;
  }

  std::string validJson(s);

  bool doc_parse_error = doc.Parse(validJson.c_str()).HasParseError();
  if (doc_parse_error || !doc.IsArray()) {
    std::filesystem::remove(kLeftoverRecordsFilepath);
    return false;
  }

  for (rapidjson::Value::ConstValueIterator itr = doc.Begin(); itr != doc.End();
       ++itr) {
    LeftoverRecord record = DeserializeLeftoverRecordFromJSON(*itr);

    leftover_tracker_.AddRecord(record);
  }

  records_file.close();
  return true;
}

bool LeftoverTrackerBackend::WriteRecordsToJSONFile() const {
  std::ofstream records_file{kLeftoverRecordsFilepath};
  // Get the records from leftover_tracker_, convert to JSON and
  // save to file.
  rapidjson::StringBuffer ss;
  rapidjson::Writer<rapidjson::StringBuffer> writer(ss);
  writer.StartArray();

  std::vector<LeftoverRecord> records = leftover_tracker_.GetRecords();

  for (LeftoverRecord record : records) {
    SerializeLeftoverRecordToJSON(record, &writer);
  }
  writer.EndArray();

  records_file << ss.GetString();

  records_file.flush();
  records_file.close();
  return true;
}

crow::json::wvalue LeftoverTrackerBackend::AddRecord(
    const crow::query_string& query_string) {
  LeftoverRecord record = QueryStringToLeftoverRecord(query_string);
  crow::json::wvalue status;

  bool add_result = leftover_tracker_.AddRecord(record);

  status[kStatusSuccess] = add_result;

  return status;
}

crow::json::wvalue LeftoverTrackerBackend::DeleteRecord(
    const crow::query_string& query_string) {
  LeftoverRecord record = QueryStringToLeftoverRecord(query_string);
  crow::json::wvalue status;

  bool delete_result = leftover_tracker_.DeleteRecord(record);

  status[kStatusSuccess] = delete_result;

  return status;
}

crow::json::wvalue LeftoverTrackerBackend::GetRecords() const {
  std::vector<LeftoverRecord> records = leftover_tracker_.GetRecords();

  crow::json::wvalue records_json({});
  records_json[kNumRecords] = records.size();

  std::vector<crow::json::wvalue> record_json_vector;
  for (const LeftoverRecord& record : records) {
    record_json_vector.push_back(LeftoverRecordToCrowJSON(record));
  }
  records_json[kRecordArray] = std::move(record_json_vector);
  return records_json;
}

crow::json::wvalue LeftoverTrackerBackend::GetLeftoverReport() const {
  LeftoverReport generated_report = leftover_tracker_.GenerateReport();

  return LeftoverReportToCrowJSON(generated_report);
}

// Constants used in File Reading/Writing
const char* kFileDataDate = "date_";
const char* kFileDataMeal = "meal_";
const char* kFileDataFoodName = "food_name_";
const char* kFileDataQtyInOz = "qty_in_oz_";
const char* kFileDataLeftoverReason = "leftover_reason_";
const char* kFileDataFoodDisposalMechanism = "disposal_mechanism_";
const char* kFileDataCost = "cost_";

// Constants used in sending/receiving data from the web
const char* kWebDataDate = "date";
const char* kWebDataMeal = "meal";
const char* kWebDataFoodName = "food_name";
const char* kWebDataQtyInOz = "qty_in_oz";
const char* kWebDataLeftoverReason = "leftover_reason";
const char* kWebDataDisposalMechanism = "disposal_mechanism";
const char* kWebDataCost = "cost";

// Constants used in sending/receiving report data from the web.
const char* kWebReportDataCommonDisposalMechanism =
    "most_common_disposal_mechanism_";
const char* kWebReportDataCommonLeftOver = "most_common_leftover_";
const char* kWebReportDataCommonLeftOverReason = "most_common_leftover_reason_";
const char* kWebReportDataMostLeftOverProducingMeal =
    "most_leftover_producing_meal_";
const char* kWebReportDataSuggestedReductLeftover =
    "suggested_strategies_to_reduce_leftover_";
const char* kWebReportDataLeftOverCost = "total_cost_of_leftover_";
