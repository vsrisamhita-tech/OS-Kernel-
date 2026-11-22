# Safe-State Analysis: How Conservative Is Deadlock Avoidance?

**Author:** Vuppalapati Sri Samhita  
**USN:** 25BTRCL285 | **Branch:** CSE-AIML-E  
**Institution:** Jain (Deemed-to-be University), FET Campus  
**Course:** 25PCCS201 - Operating Systems Research Activity

---

## Abstract
Operating systems textbooks standardly introduce safe-state analysis via Dijkstra's Banker's algorithm, asserting that every request leading to an unsafe state must be denied to prevent potential deadlock. However, an unsafe state is not inherently a deadlock; it merely implies that the worst-case sequence of requests may cause a cycle. This paper quantitatively examines the conservativeness of safe-state avoidance across synthetic multi-resource contention models. By comparing three fundamental allocation policies—No Avoidance, Banker's Safe-State Avoidance, and Havender's All-At-Once Prevention—we measure deadlock incidence, process completion rate, makespan, mean turnaround time, and useful resource utilization. Furthermore, we vary claim accuracy through a usage ratio $g \in \{1.0, 0.9, 0.75, 0.5\}$ and conduct counterfactual grant branching on refused requests. Our findings demonstrate that when declared claims are exact ($g = 1.0$), safe-state refusals are 100% necessary. However, under realistic pessimistic claim declarations ($g = 0.75$), between 95.6% and 99.3% of Banker's refusals were completely harmless, revealing an over-conservative avoidance penalty.

---

## 1. Introduction & Problem Statement
The Banker's algorithm enforces safety by ensuring that at all times there exists at least one sequential ordering $\langle P_1, P_2, \dots, P_n \rangle$ such that every active process can acquire its declared maximum claim and terminate. While theoretically sound, the algorithm requires static declaration of maximum requirements and assumes every process will greedily request its upper bound simultaneously.

In production environments (e.g., Linux kernel lock acquisition, distributed database concurrency), processes rarely declare exact needs in advance. This raises the fundamental research questions:
1. **Primary**: How often does an unsafe state truly terminate in deadlock?
2. **Secondary 1**: How do contention (claim fraction $f$ and process count $n$) and claim accuracy ($g$) alter deadlock risk and safe-state avoidance overhead?
3. **Secondary 2**: Is Banker's avoidance more efficient than all-at-once prevention, and how does the $O(m \cdot n^2)$ safety check scale?

---

## 2. Theoretical Framework & Hypotheses
- **H1 (Deadlock Elimination)**: Banker's avoidance will guarantee zero deadlocks across all configurations, whereas No Avoidance deadlock rates will scale monotonically with claim fraction $f$ and process count $n$.
- **H2 (Conservativeness & Claim Accuracy)**: Under pessimistic claims ($g < 1.0$), the majority of unsafe states will successfully resolve without deadlocking, rendering Banker's denials mostly unnecessary.
- **H3 (Utilization Trade-off)**: Banker's avoidance will provide higher useful utilization than All-At-Once allocation by avoiding upfront resource hoarding.
- **H4 (Computational Complexity)**: Worst-case safety verification will require $m \cdot \frac{n(n+1)}{2}$ vector comparisons, while practical observed checks will scale substantially lower ($O(n^{1.4})$).

---

## 3. Experimental Methodology
A discrete-event simulation engine with repeatable PRNG seeding was implemented in `/usr/local/bin/deadlock-lab`.
- **Resource Environment**: 3 distinct resource types ($M=3$) with 10 instances each ($[10, 10, 10]$).
- **Process Parameters**:
  - Arrival times: $U(0, 100)$ ticks.
  - Compute time once fully acquired: $U(5, 20)$ ticks.
  - Maximum claim fraction: $f \in \{0.3, 0.5, 0.7\}$.
  - Actual-to-declared ratio: $g \in \{1.0, 0.9, 0.75, 0.5\}$.
  - Process population: $n \in \{5, 10, 20, 40, 80\}$.
- **Counterfactual Validation**: When Banker's refuses a request, the simulation forks the state, forces the grant, and runs 20 random continuations without avoidance to measure the true empirical deadlock probability of that refusal.

---

## 4. Empirical Results & Performance Analysis

### Table 1: $n = 10$ processes, claim fraction $f = 0.7$, exact claims ($g = 1.0$)
| Policy | Runs Deadlocked (%) | Completed (%) | Makespan (ticks) | Mean Turnaround (ticks) | Useful Util (%) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **No Avoidance** | 36% | 80.2% | 126 | 23.8 | 28.0% |
| **Banker's (Safe-State)** | 0% | 100.0% | 131 | 28.7 | 33.0% |
| **All-At-Once (Prevention)** | 0% | 100.0% | 115 | 18.4 | 37.6% |

### Table 2: $n = 20$ processes, claim fraction $f = 0.5$, exact claims ($g = 1.0$)
| Policy | Runs Deadlocked (%) | Completed (%) | Makespan (ticks) | Mean Turnaround (ticks) | Useful Util (%) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **No Avoidance** | 39% | 78.0% | 142 | 25.9 | 36.1% |
| **Banker's (Safe-State)** | 0% | 100.0% | 160 | 38.7 | 39.6% |
| **All-At-Once (Prevention)** | 0% | 100.0% | 120 | 18.4 | 51.5% |

### Table 4: Effect of Claim Accuracy ($g$) & Counterfactual Verification ($n=10, f=0.7$)
| Usage Ratio $g$ | Unsafe State Entered (%) | Deadlock Rate (%) | Counterfactual Refusals Necessary (%) |
| :---: | :---: | :---: | :---: |
| **1.00 (Exact)** | 38.0% | 38.0% | **100.0%** |
| **0.90** | 80.5% | 9.0% | **12.0%** |
| **0.75 (Pessimistic)** | 81.5% | 0.0% | **0.7%** |
| **0.50 (Over-declared)** | 61.0% | 0.0% | **0.0%** |

### Table 5: Safety Verification Vector Comparisons ($M=3$)
| $n$ Processes | Observed Average Checks | Constructed Worst-Case | Theoretical $\frac{n(n+1)}{2}$ |
| :---: | :---: | :---: | :---: |
| **5** | 1.56 | 15 | 15 |
| **10** | 2.41 | 55 | 55 |
| **20** | 6.12 | 210 | 210 |
| **40** | 31.81 | 820 | 820 |
| **80** | 81.88 | 3,240 | 3,240 |

---

## 5. Key Scientific Insights
1. **Hypothesis H1 is Fully Supported**: Banker's avoidance maintained an absolute 0% deadlock rate across 2,500 test runs.
2. **Hypothesis H2 is Strongly Supported**: When claims are exact ($g = 1.0$), safe-state avoidance is optimal and 100% of refusals prevent imminent deadlock. However, when claims are pessimistic ($g = 0.75$), 99.3% of refusals were false alarms, imposing unnecessary process delays.
3. **Hypothesis H3 was NOT Supported**: All-at-once allocation outperformed Banker's in both turnaround time and useful utilization because processes in our synthetic workload do not compute between acquisitions; hence, eliminating the hold-and-wait phase completely prevented idle resource locking.
4. **Hypothesis H4 is Supported**: Observed verification costs grew as $O(n^{1.4})$, significantly faster than linear but below the constructed quadratic bound.

---

## 6. References
1. E. W. Dijkstra, "Cooperating sequential processes," Tech. Rep. EWD-123, Eindhoven, 1965.
2. A. N. Habermann, "Prevention of system deadlocks," *CACM*, vol. 12, no. 7, 1969.
3. E. G. Coffman et al., "System deadlocks," *ACM Comp. Surveys*, vol. 3, no. 2, 1971.
4. J. W. Havender, "Avoiding deadlock in multitasking systems," *IBM Syst. J.*, vol. 7, no. 2, 1968.
5. A. Silberschatz, P. B. Galvin, G. Gagne, *Operating System Concepts*, 10th ed., Wiley, 2018.
