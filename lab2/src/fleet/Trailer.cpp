#include "transport/fleet/Trailer.h"

#include "transport/cargo/Cargo.h"
#include "transport/exceptions/TransportException.h"

namespace transport {

Trailer::Trailer(double capacityKg, double capacityM3)
    : capacityKg_(capacityKg), capacityM3_(capacityM3) {
    if (capacityKg_ <= 0.0 || capacityM3_ <= 0.0) {
        throw TransportException("Trailer capacities must be positive");
    }
}

void Trailer::attach() {
    if (attached_) {
        throw TransportException("Trailer is already attached");
    }
    attached_ = true;
}

void Trailer::detach() {
    if (!attached_) {
        throw TransportException("Trailer is not attached");
    }
    attached_ = false;
}

bool Trailer::fitsCargo(const Cargo& cargo) const {
    return cargo.weightKg() <= capacityKg_ && cargo.volumeM3() <= capacityM3_;
}

} // namespace transport
