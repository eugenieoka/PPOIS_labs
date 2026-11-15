#pragma once

namespace transport {

class Cargo;

class Trailer {
public:
    Trailer() = default;
    Trailer(double capacityKg, double capacityM3);

    [[nodiscard]] double capacityKg() const noexcept { return capacityKg_; }
    [[nodiscard]] double capacityM3() const noexcept { return capacityM3_; }
    [[nodiscard]] bool isAvailable() const noexcept { return !attached_; }

    void attach();
    void detach();
    [[nodiscard]] bool fitsCargo(const Cargo& cargo) const;

private:
    double capacityKg_ = 0.0;
    double capacityM3_ = 0.0;
    bool attached_ = false;
};

} // namespace transport
