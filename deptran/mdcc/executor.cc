//
// Created by lamont on 1/11/16.
//
#include <deptran/dtxn.h>
#include <deptran/txn_reg.h>
#include "deptran/txn_chopper.h"
#include "deptran/frame.h"
#include "executor.h"

namespace mdcc {
  using rococo::TxnRequest;
  using rococo::Frame;
  using rococo::SimpleCommand;

  MdccExecutor::MdccExecutor(txnid_t txn_id, Scheduler* sched) :
      Executor(txn_id, sched), sched_(static_cast<MdccScheduler*>(sched)) {
    Log_debug("mdcc executor created for txn_id %ld\n", txn_id);
  }

  void MdccExecutor::StartPiece(const SimpleCommand& cmd, int32_t* result, DeferredReply *defer) {
    Log_info("%s type: %d; piece_id: %d;", __FUNCTION__, cmd.type_, cmd.inn_id_);
    auto handler_pair = this->txn_reg_->get(cmd);
    auto& c = const_cast<SimpleCommand&>(cmd);
    handler_pair.txn_handler(this, dtxn_, c, result, c.output);
    sched_->SendUpdateProposal(cmd_id_, cmd, result, defer);
  }
}
