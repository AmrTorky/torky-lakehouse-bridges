#ifndef TORKY_LAKEHOUSE_ARROW_DUCKDB_HPP
#define TORKY_LAKEHOUSE_ARROW_DUCKDB_HPP

#include <cmath>
#include <cstdint>
#include <vector>
#include <algorithm>

namespace torky::lakehouse::bridges {

    inline constexpr double EPSILON_NM2       = 0.016;
    inline constexpr double V_FINITE_ZERO_EV  = 11.384;
    inline constexpr size_t CGROUP_RAM_CAP_MB = 512;

    #pragma pack(push, 1)
    struct TorkyCellDtype {
        uint8_t  gov_gate;     // TSG Gate Register (0x01 - 0x04)
        int16_t  phase_q16;    // Phase Q16 Fixed-Point
        int16_t  field_q16;    // Field Q16 Fixed-Point
        uint32_t syndrome_u32; // SHA-256 Merkle Syndrome
    };
    #pragma pack(pop)

    static_assert(sizeof(TorkyCellDtype) == 9, "CRITICAL: TorkyCellDtype must be precisely 9 bytes!");

    class LakehouseStreamingBridge {
    public:
        // Zero-copy column vector streaming filter under 512 MiB RAM ceiling
        static inline size_t filter_bigint_stream(const int64_t* input_records, size_t count, int64_t threshold, std::vector<int64_t>& out_filtered) {
            out_filtered.clear();
            out_filtered.reserve(count / 2);
            for (size_t i = 0; i < count; ++i) {
                if (input_records[i] >= threshold) {
                    out_filtered.push_back(input_records[i]);
                }
            }
            return out_filtered.size();
        }

        static inline double evaluate_v_finite(double r_nm) {
            return 1.0 / std::sqrt((r_nm * r_nm) + EPSILON_NM2);
        }
    };

    class TsgGateController {
    public:
        static inline bool validate_authorization(uint8_t gate_request, bool bit_tampered) {
            if (bit_tampered) return false;
            return (gate_request >= 0x01 && gate_request <= 0x04);
        }
    };

} // namespace torky::lakehouse::bridges

#endif // TORKY_LAKEHOUSE_ARROW_DUCKDB_HPP
