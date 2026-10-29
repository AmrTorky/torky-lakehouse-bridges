CXX ?= g++
CXXFLAGS = -O3 -std=c++20 -march=native -Wall -Wextra -Iinclude -pthread

TARGET = build/torky-lakehouse-cli
BENCH_BIN = build/lakehouse_arrow_bench

all: $(TARGET) $(BENCH_BIN)

$(TARGET): src/torky_cli_kernel.cpp include/torky_continuum_core.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/torky_cli_kernel.cpp -o $(TARGET)

$(BENCH_BIN): src/bridge/lakehouse_arrow_bench.cpp include/torky_continuum_core.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/bridge/lakehouse_arrow_bench.cpp -o $(BENCH_BIN)

test: all
	./$(TARGET)
	./$(BENCH_BIN)
	python3 tests/test_lakehouse_duckdb.py
	./run_audit.sh

clean:
	rm -rf build local_verification_certificate.json
