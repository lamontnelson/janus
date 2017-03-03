#pragma once

#include "coordinator.h"
#include "./bench/micro/piece.h"

namespace deptran {

class MicroTxnCmd : public Procedure {

public:

    MicroTxnCmd();

    virtual void Init(TxnRequest &req);

    virtual void InitR(TxnRequest &req);

    virtual void InitW(TxnRequest &req);

    virtual bool start_callback(int pi, int res,
                                map<int32_t, Value> &output);

    virtual bool IsReadOnly();

    virtual void Reset() override;

    virtual ~MicroTxnCmd();

};

} // namespace deptran
