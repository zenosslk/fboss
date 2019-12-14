/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include "fboss/agent/hw/sai/api/SaiApi.h"
#include "fboss/agent/hw/sai/api/SaiAttribute.h"
#include "fboss/agent/hw/sai/api/SaiAttributeDataTypes.h"
#include "fboss/agent/hw/sai/api/Types.h"

#include <folly/logging/xlog.h>

#include <optional>
#include <tuple>

extern "C" {
#include <sai.h>
}

namespace facebook::fboss {

class HashApi;

struct SaiHashTraits {
  static constexpr sai_object_type_t ObjectType = SAI_OBJECT_TYPE_HASH;
  using SaiApiT = HashApi;
  struct Attributes {
    using EnumType = sai_hash_attr_t;
    using NativeHashFieldList = SaiAttribute<
        EnumType,
        SAI_HASH_ATTR_NATIVE_HASH_FIELD_LIST,
        std::vector<sai_native_hash_field_t>>;
    using UdfGroupList = SaiAttribute<
        EnumType,
        SAI_HASH_ATTR_UDF_GROUP_LIST,
        std::vector<sai_object_id_t>>;
  };
  using AdaptorKey = HashSaiId;
  using AdaptorHostKey = Attributes::NativeHashFieldList;
  using CreateAttributes =
      std::tuple<Attributes::NativeHashFieldList, Attributes::UdfGroupList>;
};

class HashApi : public SaiApi<HashApi> {
 public:
  static constexpr sai_api_t ApiType = SAI_API_HASH;
  HashApi() {
    auto status = sai_api_query(ApiType, reinterpret_cast<void**>(&api_));
    saiApiCheckError(status, ApiType, "Failed to query for hash api");
  };

 private:
  sai_status_t _create(
      HashSaiId* id,
      sai_object_id_t switch_id,
      size_t count,
      sai_attribute_t* attr_list) {
    return api_->create_hash(rawSaiId(id), switch_id, count, attr_list);
  }

  sai_status_t _remove(HashSaiId id) {
    return api_->remove_hash(id);
  }

  sai_status_t _getAttribute(HashSaiId id, sai_attribute_t* attr) const {
    return api_->get_hash_attribute(id, 1, attr);
  }

  sai_status_t _setAttribute(HashSaiId id, const sai_attribute_t* attr) {
    return api_->set_hash_attribute(id, attr);
  }

  sai_hash_api_t* api_;
  friend class SaiApi<HashApi>;
};

} // namespace facebook::fboss