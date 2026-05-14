# CIS 279: Term Project

## Build & run (brief)

**Sample tests (interpreter harness)** — from the project directory:

```bash
g++ -std=c++11 -O2 -o weighted_test Weighted_graph_driver.cpp
./weighted_test < test.in.txt
```

**Speed benchmark** (optional):

```bash
g++ -std=c++11 -O2 -o speed_test speed.cpp
./speed_test
```

**Dense “full graph” driver** — pass graph size `N` as argv[1], e.g. `./full_test 5`:

```bash
g++ -std=c++11 -O2 -o full_test full.cpp
./full_test 5
```