#include "row_table.h"
#include <cstring>
#include <iostream>
namespace bytedance_db_project {
RowTable::RowTable() {}

RowTable::~RowTable() {
  if (rows_ != nullptr) {
    delete rows_;
    rows_ = nullptr;
  }
}

void RowTable::Load(BaseDataLoader *loader) {
  num_cols_ = loader->GetNumCols();
  auto rows = loader->GetRows();
  num_rows_ = rows.size();
  rows_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_];
  for (auto row_id = 0; row_id < num_rows_; row_id++) {
    auto cur_row = rows.at(row_id);
    std::memcpy(rows_ + row_id * (FIXED_FIELD_LEN * num_cols_), cur_row,
                FIXED_FIELD_LEN * num_cols_);
  }
}

int32_t RowTable::GetIntField(int32_t row_id, int32_t col_id) {
  // TODO: Implement this!
  int offset = num_cols_;
  int idx = 0;
  idx = offset * row_id + col_id;
  
  return *(int32_t*)&rows_[idx * FIXED_FIELD_LEN];
}

void RowTable::PutIntField(int32_t row_id, int32_t col_id, int32_t field) {
  // TODO: Implement this!
  rows_[num_cols_ * row_id * FIXED_FIELD_LEN + col_id] = field;
}

int64_t RowTable::ColumnSum() {
  // TODO: Implement this!
  int64_t sum = 0;
  for(int i=0;i < num_rows_;i++){
      sum += *(int32_t*)&rows_[i* num_cols_ * FIXED_FIELD_LEN];
  }
  return sum;
}

int64_t RowTable::PredicatedColumnSum(int32_t threshold1, int32_t threshold2) {
  // TODO: Implement this!
  int64_t sum = 0;
  for(int i = 0; i < num_rows_; i++){
    int32_t value1 = GetIntField(i, 1);
    int32_t value2 = GetIntField(i, 2);
    if(value1 > threshold1 && value2 < threshold2){
      sum += GetIntField(i, 0);
    }
  }
  return sum;
}

int64_t RowTable::PredicatedAllColumnsSum(int32_t threshold) {
  // TODO: Implement this!
  int64_t sum = 0;
  for(int i = 0; i < num_rows_; i++){
    if(GetIntField(i, 0) > threshold){
      for(int j = 0; j < num_cols_; j++){
        int32_t col_value = GetIntField(i, j);
        sum += col_value;
      }
    }
  }
  return sum;
}

int64_t RowTable::PredicatedUpdate(int32_t threshold) {
  // TODO: Implement this!
  return 0;
}
} // namespace bytedance_db_project