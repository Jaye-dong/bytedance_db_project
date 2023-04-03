#include "indexed_row_table.h"
#include <cstring>
#include <iostream>
namespace bytedance_db_project {
IndexedRowTable::IndexedRowTable(int32_t index_column) {
  index_column_ = index_column;
  
}

void IndexedRowTable::Load(BaseDataLoader *loader) {
  // TODO: Implement this!
  num_cols_ = loader->GetNumCols();
  auto rows = loader->GetRows();
  num_rows_ = rows.size();
  rows_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_];
  for (auto row_id = 0; row_id < num_rows_; row_id++) {
    auto cur_row = rows.at(row_id);
    std::memcpy(rows_ + row_id * (FIXED_FIELD_LEN * num_cols_), cur_row,
                FIXED_FIELD_LEN * num_cols_);

    //add index
    int32_t value = *(int32_t*)&rows_[row_id * FIXED_FIELD_LEN];
    // std::unordered_map<int32_t, std::vector<int32_t>> index_;
    if(!index_.count(value)){
      index_[value] = std::vector<int>{row_id};
    }else{
      index_[value].push_back(row_id);
    }
  }
  
}

int32_t IndexedRowTable::GetIntField(int32_t row_id, int32_t col_id) {
  // TODO: Implement this!
  int offset = num_cols_;
  int idx = 0;
  idx = offset * row_id + col_id;
  return *(int32_t*)&rows_[idx * FIXED_FIELD_LEN];
}

void IndexedRowTable::PutIntField(int32_t row_id, int32_t col_id,
                                  int32_t field) {
  // TODO: Implement this!
}

int64_t IndexedRowTable::ColumnSum() {
  // TODO: Implement this!
    int64_t sum = 0;
    for (auto row_id = 0; row_id < num_rows_; row_id++)
    {
      auto offset = FIXED_FIELD_LEN * ((row_id * num_cols_) + 0);
      int32_t col0_at_row = *(int32_t *)(rows_ + offset);
      sum = sum + col0_at_row;
    }
    return sum;
}

int64_t IndexedRowTable::PredicatedColumnSum(int32_t threshold1,
                                             int32_t threshold2) {
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

int64_t IndexedRowTable::PredicatedAllColumnsSum(int32_t threshold) {
  // TODO: Implement this!
  int res = 0;
  for(auto it = index_.begin(); it != index_.end(); it++){
    if(it->first > threshold){
      for(int i : it->second) {
        for(int j = 0; j < num_rows_; j++){
          res += GetIntField(i, j);
        }
      }
    }
  }
  return res;
}

int64_t IndexedRowTable::PredicatedUpdate(int32_t threshold) {
  // TODO: Implement this!
  return 0;
}
} // namespace bytedance_db_project