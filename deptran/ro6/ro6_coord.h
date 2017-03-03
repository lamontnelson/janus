#pragma once
#include "../rcc/coord.h"

namespace rococo {
class RO6Coord : public RccCoord {
  using RccCoord::RccCoord;
public:

  struct deptran_batch_start_t {
    std::vector<RequestHeader>      headers;
    std::vector<std::vector<Value> >inputs;
    std::vector<int>                output_sizes;
    std::vector<int>                pis;
    rrr::FutureAttr                 fuattr;
  };
  std::set<i64> ro_txn_;

  virtual void do_one(TxnRequest&);

  void         deptran_start(Procedure *ch);

  void         deptran_finish(Procedure *ch);

  void         ro6_start_ro(Procedure *ch);
};
} // namespace rococo
