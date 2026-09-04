# Safety / SIL Self-Assessment — Banking System

**Document Status:** Informational self-assessment. **This is not a
certification.** No Safety Integrity Level (SIL, per IEC 61508) or ASIL
(per ISO 26262) can be legitimately claimed for this project. Those
ratings are issued only by an accredited, independent assessor after a
formal safety case, hazard analysis, and audit of the full development
process — none of which has occurred here. This document exists to
honestly show which of this project's *existing* engineering practices
happen to overlap with what a real SIL assessment would look for, and
where the real gaps are.

---

## 1. Why This Document Exists

This project deliberately follows several practices associated with
safety-critical software development (V-Model SDLC, MISRA C compliance,
full requirement traceability). It is reasonable to ask "how close is
this to an actual safety-rated system?" — this document answers that
honestly, rather than either ignoring the question or overstating the
project's actual assurance level.

## 2. Practices Already in Place That Overlap With SIL Requirements

| Practice | Where | Relevance to a real SIL assessment |
|---|---|---|
| V-Model SDLC (BRS→SRS→HLD→LLD→Code→Test→RTM) | `docs/` | IEC 61508 explicitly requires a documented, traceable lifecycle — this project's structure matches that shape |
| Requirement Traceability Matrix, 32/32 SRS covered | `docs/traceability/RTM.xlsx` | Directly analogous to the traceability evidence a safety assessor would demand |
| MISRA C static analysis | `ci-cd.yml` (`static_analysis` job) | MISRA C is explicitly referenced by ISO 26262 as an acceptable coding standard for automotive software |
| 120 unit tests + 15 functional tests, all passing | `docs/testing/` | Matches the "verification evidence" a safety case requires, at the unit and system level |
| I/O Separation Principle (testable business logic, no I/O side effects) | `docs/design/LLD.md`, Section 1.2 | Matches safety-critical design guidance to isolate and minimize untestable/non-deterministic code paths |
| Documented, deliberate design trade-offs (e.g., `double` for money, hardcoded admin credentials) | `docs/design/HLD.md`, `LLD.md` | A real safety case requires every design decision to be explicitly justified, not merely implemented — this project already does that in spirit |

## 3. What a Real SIL/ASIL Rating Would Additionally Require (Not Present Here)

- **Independent, accredited assessment** — by a certification body (e.g., TÜV, exida), not self-declared
- **Formal hazard and risk analysis** (HARA for ASIL, or the IEC 61508 risk graph/LOPA process) — this project has never had a hazard analysis performed, because it is not a system operating in a physical/safety-relevant environment (it is a console banking demo, not embedded control software)
- **Tool qualification** — for a real SIL claim, every tool in the toolchain (compiler, static analyzer, test framework) must itself be qualified/certified for that SIL; `gcc`, `cppcheck`, and `Unity` here are not qualified tools
- **Independent verification & validation (IV&V)** — testing performed by a team independent of development, with formal sign-off; here, all testing was performed by the same author as the code
- **Configuration management and change control audits** — beyond what git/GitHub provides by default
- **Structural code coverage targets tied to SIL level** (e.g., MC/DC coverage for higher ASILs) — this project tracks functional test pass/fail, not MC/DC or branch coverage against a mandated threshold

## 4. Honest Conclusion

This project demonstrates **safety-oriented engineering discipline** —
the same discipline that underlies real SIL-rated development — applied
to a non-safety-critical console application, as a learning and portfolio
exercise. It should be described as such: *"built using practices
consistent with safety-critical software development (V-Model, MISRA C,
full requirement traceability)"* — not as SIL-certified or SIL-compliant,
since no such certification exists or was pursued for this project.
