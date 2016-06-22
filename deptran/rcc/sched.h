
#pragma once

#include "../__dep__.h"
#include "../scheduler.h"
#include "graph.h"
#include "dep_graph.h"

namespace rococo {

class SimpleCommand;
class RccGraph;
class RccCommo;
class TxnInfo;
class WaitlistChecker;

class RccSched : public Scheduler {
 public:
  static map<txnid_t, int32_t> __debug_xxx_s;
  static std::recursive_mutex __debug_mutex_s;
  static void __DebugCheckParentSetSize(txnid_t tid, int32_t sz);

 public:
  RccGraph *dep_graph_ = nullptr;
  WaitlistChecker* waitlist_checker_ = nullptr;
  set<RccVertex*> waitlist_ = {};
  set<RccVertex*> fridge_ = {};
  std::recursive_mutex mtx_{};
  uint32_t curr_epoch_{1};
  std::time_t last_upgrade_time_{0};
  map<parid_t, int32_t> epoch_replies_{};
  bool in_upgrade_epoch_{false};
  const int EPOCH_DURATION = 5;
  map<epoch_t, int64_t> active_epoch_{};
  map<epoch_t, unordered_set<RccDTxn*>> epoch_dtxn_{};

  RccSched();
  virtual ~RccSched();

  DTxn* GetOrCreateDTxn(txnid_t tid, bool ro = false) override ;

  virtual void SetPartitionId(parid_t par_id) {
    partition_id_ = par_id;
    dep_graph_->partition_id_ = par_id;
  }

  int OnDispatch(const vector<SimpleCommand> &cmd,
                 rrr::i32 *res,
                 TxnOutput* output,
                 RccGraph *graph,
                 const function<void()> &callback);

  int OnCommit(cmdid_t cmd_id,
               const RccGraph &graph,
               TxnOutput* output,
               const function<void()> &callback);

  virtual int OnInquire(cmdid_t cmd_id,
                        RccGraph *graph,
                        const function<void()> &callback);

//  void to_decide(Vertex<TxnInfo> *v,
//                 rrr::DeferredReply *defer);
  DTxn* CreateDTxn(i64 tid, bool ro) override {
    verify(0);
  }

  DTxn* GetDTxn(i64 tid) override {
    verify(0);
  }


  void InquireAbout(Vertex<RccDTxn> *av);
  void InquireAboutIfNeeded(RccDTxn &info);
  void AnswerIfInquired(RccDTxn &tinfo);
  void CheckWaitlist();
  void InquireAck(cmdid_t cmd_id, RccGraph& graph);
  void TriggerCheckAfterAggregation(RccGraph &graph);
  void AddChildrenIntoWaitlist(RccVertex* v);
  bool AllAncCmt(RccVertex *v);
  bool FullyDispatched(const RccScc& scc);
  void Decide(const RccScc&);
  bool HasICycle(const RccScc& scc);
  bool HasAbortedAncestor(const RccScc& scc);
  bool AllAncFns(const RccScc&);
  void Execute(const RccScc&);
  void Execute(RccDTxn&);
  void Abort(const RccScc&);

  void OnTruncateEpoch(uint32_t old_epoch);

  void __DebugExamineFridge();
  RccVertex* __DebugFindAnOngoingAncestor(RccVertex* vertex);
  void __DebugExamineGraphVerify(RccVertex *v);
  RccCommo* commo();
};

} // namespace rococo