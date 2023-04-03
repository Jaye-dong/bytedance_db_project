#include "column_table.h"
#include <cstring>
#include <iostream>

namespace bytedance_db_project {
ColumnTable::ColumnTable() {}

ColumnTable::~ColumnTable() {
  if (columns_ != nullptr) {
    delete columns_;
    columns_ = nullptr;
  }
}

//
// columnTable, which stores data in column-major format.
// That is, data is laid out like
//   col 1 | col 2 | ... | col m.
//

void ColumnTable::Load(BaseDataLoader *loader) {
  num_cols_ = loader->GetNumCols();
  std::vector<char *> rows = loader->GetRows();
  num_rows_ = rows.size();
  columns_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_];

  for (int32_t row_id = 0; row_id < num_rows_; row_id++) {
    auto cur_row = rows.at(row_id);
    for (int32_t col_id = 0; col_id < num_cols_; col_id++) {
      int32_t offset = FIXED_FIELD_LEN * ((col_id * num_rows_) + row_id);
      std::memcpy(columns_ + offset, cur_row + FIXED_FIELD_LEN * col_id,
                  FIXED_FIELD_LEN);
    }
  }
}

int32_t ColumnTable::GetIntField(int32_t row_id, int32_t col_id) {
  // TODO: Implement this!
  int offset = num_rows_;
  int idx = row_id * offset + col_id;
  return *(int32_t*)&columns_[idx * FIXED_FIELD_LEN];
}

void ColumnTable::PutIntField(int32_t row_id, int32_t col_id, int32_t field) {
  // TODO: Implement this!
}

int64_t ColumnTable::ColumnSum() {
  // TODO: Implement this!
  int sum = 0;
  for(int i=0;i<num_rows_;i++){
    sum += columns_[i * FIXED_FIELD_LEN] + (columns_[i * FIXED_FIELD_LEN + 1] << 8) + (columns_[i * FIXED_FIELD_LEN + 2] << 16) + (columns_[i * FIXED_FIELD_LEN + 3]<< 24);
  }
  return sum;
}

int64_t ColumnTable::PredicatedColumnSum(int32_t threshold1,
                                         int32_t threshold2) {
  // TODO: Implement this!
  int res = 0;
  for(int i = 0; i < num_rows_; i++){
    int32_t value1 = GetIntField(1, i);
    int32_t value2 = GetIntField(2, i);
    if(value1 > threshold1 && value2 < threshold2) {
      res += GetIntField(0, i);
    }
  }
  return res;
}

int64_t ColumnTable::PredicatedAllColumnsSum(int32_t threshold) {
  // TODO: Implement this!
  int res = 0;
  for(int i = 0; i < num_rows_; i++){
    int32_t value = GetIntField(0, i);
    if(value > threshold) {
      for(int j = 0; j < num_cols_; j++){
        res += GetIntField(j, i);
      }
    }
  }
  return res;
}

int64_t ColumnTable::PredicatedUpdate(int32_t threshold) {
  // TODO: Implement this!
  return 0;
}
} // namespace bytedance_db_project