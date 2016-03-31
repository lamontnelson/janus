#pragma once

#include "../__dep__.h"
#include "../constants.h"
#include "../command.h"
#include "../rcc/coord.h"
#include "dep_graph.h"
#include "brq-common.h"

namespace rococo {
class BrqCommo;
class BrqCoord : public RccCoord {
public:
  cooid_t  coo_id_;
//  phase_t  phase_; // a phase identifier
  uint32_t thread_id_;
  uint32_t cmdid_prefix_c_;
  Recorder *recorder_;
  ballot_t ballot_; // the ballot I am holding
  cmdid_t cmd_id_;
  // data structures for saving replies.
  struct reply_cnt_t {int yes; int no;};
  std::map<groupid_t, reply_cnt_t> n_fast_accept_reply_;
  std::map<groupid_t, reply_cnt_t> n_accept_reply_;
  std::map<groupid_t, reply_cnt_t> n_prepare_reply_;
  std::map<groupid_t, reply_cnt_t> n_commit_reply_;

  using RccCoord::RccCoord;

  virtual ~BrqCoord() {}

  BrqCommo* commo();
  // Dispatch inherits from RccCoord;
  void DispatchAck(phase_t phase,
                   int res,
                   SimpleCommand &cmd,
                   RccGraph &graph);

  void PreAccept();
  void PreAcceptAck(phase_t phase,
                    parid_t par_id,
                    int res,
                    RccGraph& graph);

  // do_one inherits from RccCoord;

  void restart() {verify(0);};
  // functions needed in the fast accept phase.
  bool FastpathPossible() {verify(0);};
  bool FastQuorumsAchieved() {verify(0);};
  bool SlowpathPossible() {
    verify(0);
    return false;
  };
  bool SlowQuorumsAchieved() {
    verify(0);
    return false;
  };

  void prepare();
  // functions needed in the accept phase.
  void accept();
  void accept_ack(groupid_t, AcceptReply*, phase_t);
  bool check_accept_possible() {
    verify(0);
    return false;
  };
  bool check_accept() {
    verify(0);
    return false;
  };

  void Commit();
  void CommitAck(phase_t phase,
                 parid_t par_id,
                 map<innid_t, map<int32_t, Value>>& output);
  bool check_commit() {
    verify(0);
    return false;
  };

//  void launch(Command &cmd);
  void launch_recovery(cmdid_t cmd_id);

  ballot_t magic_ballot() {
    ballot_t ret = 0;
    ret = (ret << 32) | coo_id_;
    return ret;
  }

  cmdid_t next_cmd_id() {
    cmdid_t ret = cmdid_prefix_c_++;
    ret = (ret << 32 | coo_id_);
    return ret;
  }
};
} // namespace rococo