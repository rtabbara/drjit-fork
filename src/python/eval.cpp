#include "eval.h"
#include "apply.h"

struct ScheduleCallback : TraverseCallback {
    bool result = false;
    void operator()(nb::handle h) override {
        const ArraySupplement &s = supp(h.type());
        if (s.index)
            result |= jit_var_schedule(s.index(nb::inst_ptr<dr::ArrayBase>(h))) != 0;
    }
};

bool schedule(nb::handle h) {
    ScheduleCallback s;
    traverse("drjit.schedule", s, h);
    return s.result;
}


void eval(nb::handle h) {
    if (schedule(h))
        jit_eval();
}

void export_eval(nb::module_ &m) {
    m.def("schedule", &schedule, nb::raw_doc(doc_schedule));
    m.def("eval", &eval, nb::raw_doc(doc_eval));
}
