# Low-Level Design (LLD) — Banking System

**Document Status:** Draft
**Depends on:** `design/HLD.md`
**Related Requirements:** All SRS items in `requirements/BRS_SRS.xlsx`

---

## 1. Design Conventions

### 1.1 Naming

- Files/functions: `snake_case` — `account_db_insert_pending()`
- Types (structs/enums/typedefs): `PascalCase` — `AccountDatabase`, `StatusCode`
- Constants/macros: `UPPER_SNAKE_CASE` — `MAX_NAME_LEN`
- Function naming pattern: `<module>_<verb>_<noun>()` — e.g. `bill_queue_enqueue()`,
  `account_db_delete()`. This keeps every function's origin module obvious
  from its name alone, even out of context.

### 1.2 I/O Separation Principle

**All `scanf`/`printf` calls live in `ui_screens` only.** Every other module
(`account`, `bill_queue`, `cash_queue`, `admin`) is pure logic: it accepts
already-collected data via plain input structs, operates on it, and returns
a `StatusCode` — never reading from or writing to the console itself.

This is a deliberate architectural choice beyond what the original prototype
did (which mixed I/O and logic throughout). It directly enables the next
V-Model stage: a business-logic function with no I/O can be unit tested by
simply calling it with test data and checking the return value — no stdin
simulation required.

**Pattern:**
```
ui_screens.c:  collects raw fields  -->  fills an Input struct
main.c:        passes Input struct  -->  calls the module's logic function
module.c:      validates/stores     -->  returns a StatusCode
main.c:        passes result        -->  calls ui_screens to display outcome
```

### 1.3 Error Handling Strategy

Every logic function that can fail returns a `StatusCode` (defined in
`common.h`). Output data (when the function produces a result, e.g. an
account balance) is written through an output pointer parameter, following
the standard C convention:

```c
StatusCode account_db_get_balance(const AccountDatabase *db,
                                   const char *username,
                                   const char *password,
                                   Money *out_balance);
```

The caller always checks the returned `StatusCode` before trusting any
output parameter's value.

### 1.4 Memory Ownership

- Every `malloc` is immediately checked for `NULL`; on failure, the
  function returns `STATUS_ERROR_ALLOC_FAILED` and performs no partial
  mutation of any data structure.
- Every module that allocates provides a matching `_destroy()` function
  that frees all its own memory — no module frees memory it did not
  allocate.

---

## 2. `common` Module

**Files:** `include/common.h` (header-only, no `.c` — pure definitions)

### 2.1 Types

```c
typedef double Money;

typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR_NOT_FOUND,
    STATUS_ERROR_DUPLICATE,
    STATUS_ERROR_INSUFFICIENT_FUNDS,
    STATUS_ERROR_INVALID_INPUT,
    STATUS_ERROR_ALLOC_FAILED,
    STATUS_ERROR_QUEUE_EMPTY
} StatusCode;
```

### 2.2 Buffer Size Constants

| Constant | Value | Justification |
|---|---|---|
| `MAX_NAME_LEN` | 50 | Generous for first/last name; avoids truncating real names. |
| `MAX_USERNAME_LEN` | 30 | Matches typical username policy limits. |
| `MAX_PASSWORD_LEN` | 30 | Matches typical password policy limits. |
| `MAX_EMAIL_LEN` | 60 | Covers RFC-length emails used in practice. |
| `MAX_CITY_LEN` | 40 | Generous for city names. |
| `MAX_TOWN_LEN` | 40 | Generous for town names. |
| `MAX_BILL_TYPE_LEN` | 20 | Covers "Electricity", "Internet", etc. with headroom. |

**Satisfies:** Supports SRS-026 (modular organization) as the shared
foundation every other module depends on.

---

## 3. `platform` Module

**Files:** `include/platform.h`, `src/platform.c`

### 3.1 Types

```c
typedef struct {
    int day;
    int month;   /* 1-12 */
    int year;    /* full year, e.g. 2026 */
    int hour;
    int minute;
} SystemTime;
```

### 3.2 Functions

```c
void platform_clear_screen(void);
void platform_wait_for_keypress(void);
void platform_get_current_time(SystemTime *out_time);
```

- `platform_clear_screen()` — wraps `system("cls")` (Windows) internally;
  a Linux build swaps this single function's body for `system("clear")`.
- `platform_wait_for_keypress()` — wraps `_getch()` (Windows) internally;
  a Linux build swaps this for a `termios`-based equivalent.
- `platform_get_current_time()` — wraps `time()`/`localtime()`, filling
  the portable `SystemTime` struct so no other module touches `<time.h>`
  directly.

**Satisfies:** SRS-025 (no OS-specific API outside this module).

---

## 4. `address` Module

**Files:** `include/address.h`, `src/address.c`

### 4.1 Types

```c
typedef struct {
    int house_number;
    char town_name[MAX_TOWN_LEN];
    int street_number;
    char city[MAX_CITY_LEN];
} Address;
```

### 4.2 Functions

```c
void address_init(Address *addr);
void address_display(const Address *addr);

int  address_get_house_number(const Address *addr);
int  address_get_street_number(const Address *addr);   /* bug fixed: original returned house_number */
const char *address_get_city(const Address *addr);
const char *address_get_town_name(const Address *addr);
```

Note: `address_input()` (collecting the four fields from the user) lives in
`ui_screens`, not here, per the I/O Separation Principle — `ui_screens`
fills an `Address` struct directly and passes it onward.

**Satisfies:** Supports SRS-001 (account opening captures home address).

---

## 5. `bill_queue` Module

**Files:** `include/bill_queue.h`, `src/bill_queue.c`

### 5.1 Types

```c
typedef struct {
    char bill_type[MAX_BILL_TYPE_LEN];
    long invoice_number;
    Money amount;
} BillRequestInput;

typedef struct BillNode {
    BillRequestInput data;
    struct BillNode *next;
    struct BillNode *prev;
} BillNode;

typedef struct {
    BillNode *front;
    BillNode *rear;
} BillQueue;
```

### 5.2 Functions

```c
void       bill_queue_init(BillQueue *q);
bool       bill_queue_is_empty(const BillQueue *q);
StatusCode bill_queue_enqueue(BillQueue *q, const BillRequestInput *input);
StatusCode bill_queue_dequeue(BillQueue *q, BillRequestInput *out_processed);
StatusCode bill_queue_peek_front(const BillQueue *q, BillRequestInput *out_front);
void       bill_queue_destroy(BillQueue *q);
```

`bill_queue_display_all()` is intentionally **not** in this module — instead,
`bill_queue` exposes an iterator-friendly accessor so `ui_screens` can walk
the queue and print it without `bill_queue` itself doing any I/O:

```c
const BillNode *bill_queue_get_front_node(const BillQueue *q);
const BillNode *bill_queue_get_next_node(const BillNode *node);
```

**Satisfies:** SRS-018, SRS-019, SRS-020, SRS-021, SRS-022.

---

## 6. `cash_queue` Module

**Files:** `include/cash_queue.h`, `src/cash_queue.c`

### 6.1 Types

```c
typedef enum {
    CASH_TXN_WITHDRAW,
    CASH_TXN_DEPOSIT
} CashTransactionType;

typedef struct {
    long account_number;
    Money amount;
    CashTransactionType type;
} CashRequestInput;

typedef struct CashNode {
    CashRequestInput data;
    struct CashNode *next;
    struct CashNode *prev;
} CashNode;

typedef struct {
    CashNode *front;
    CashNode *rear;
} CashQueue;
```

### 6.2 Functions

```c
void       cash_queue_init(CashQueue *q);
bool       cash_queue_is_empty(const CashQueue *q);
StatusCode cash_queue_enqueue(CashQueue *q, const CashRequestInput *input);
StatusCode cash_queue_dequeue(CashQueue *q, CashRequestInput *out_processed);
StatusCode cash_queue_peek_front(const CashQueue *q, CashRequestInput *out_front);
void       cash_queue_destroy(CashQueue *q);

const CashNode *cash_queue_get_front_node(const CashQueue *q);
const CashNode *cash_queue_get_next_node(const CashNode *node);
```

Note: applying a dequeued request's amount to the target account's balance
(SRS-030) is performed by `main`, which calls `cash_queue_dequeue()` to
retrieve the request, then calls the appropriate `account` module function
(`account_db_apply_deposit()` / `account_db_apply_withdrawal()`) — `cash_queue`
itself has no dependency on `account` (see HLD dependency graph).

**Satisfies:** SRS-015, SRS-016, SRS-017, SRS-029, SRS-030.

---

## 7. `account` Module

**Files:** `include/account.h`, `src/account.c`

### 7.1 Types

```c
typedef enum {
    ACCOUNT_STATUS_PENDING,
    ACCOUNT_STATUS_COMPLETED,
    ACCOUNT_STATUS_DECLINED
} AccountStatus;

typedef struct {
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    Address home_address;
    long long phone_number;
    long long aadhaar_number;
    bool wants_debit_card;
    int  debit_card_pin;      /* valid only if wants_debit_card == true */
    bool wants_cheque_book;
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
} NewAccountInput;

typedef struct AccountNode {
    NewAccountInput details;
    AccountStatus status;
    Money balance;
    long account_number;      /* assigned only on approval */
    struct AccountNode *next;
    struct AccountNode *prev;
} AccountNode;

typedef struct {
    AccountNode *head;
    AccountNode *tail;
} AccountDatabase;
```

### 7.2 Functions

```c
void       account_db_init(AccountDatabase *db);
void       account_db_destroy(AccountDatabase *db);

bool       account_db_username_exists(const AccountDatabase *db, const char *username);
StatusCode account_db_insert_pending(AccountDatabase *db, const NewAccountInput *input);

const AccountNode *account_db_get_first_pending(const AccountDatabase *db);
const AccountNode *account_db_get_next_pending(const AccountNode *current);

StatusCode account_db_approve(AccountDatabase *db, AccountNode *node);
StatusCode account_db_decline(AccountDatabase *db, AccountNode *node);
StatusCode account_db_delete(AccountDatabase *db, long account_number);

AccountNode *account_db_find_by_credentials(AccountDatabase *db,
                                             const char *username,
                                             const char *password);

bool       account_db_check_atm_credentials(const AccountDatabase *db,
                                             long account_number,
                                             int pin);

StatusCode account_db_apply_withdrawal(AccountNode *acc, Money amount);
StatusCode account_db_apply_deposit(AccountNode *acc, Money amount);
StatusCode account_db_change_pin(AccountNode *acc, int old_pin, int new_pin);
StatusCode account_db_get_balance(const AccountNode *acc, Money *out_balance);

const AccountNode *account_db_get_all_first(const AccountDatabase *db);
const AccountNode *account_db_get_all_next(const AccountNode *current);

long account_db_generate_account_number(const AccountDatabase *db);
```

### 7.3 Notes on Fixed Bugs from the Original

- **Balance now initialized to `0.00`** at `account_db_approve()` time
  (SRS-032), not a hardcoded value.
- **`account_db_delete()` correctly updates `tail`** when the last node is
  removed, and safely returns `STATUS_ERROR_NOT_FOUND` instead of
  dereferencing a `NULL` pointer when the account isn't found mid-list.
- **PIN change validates the new PIN is exactly 4 digits** (0–9999 range
  check plus digit-count validation), addressing SRS-023 and SRS-014.

**Satisfies:** SRS-001 through SRS-014, SRS-031, SRS-032.

---

## 8. `admin` Module

**Files:** `include/admin.h`, `src/admin.c`

### 8.1 Functions

```c
bool admin_check_credentials(const char *username, const char *password);
```

Credentials are compared against fixed constants defined internally in
`admin.c` (not exposed via the header), matching the original design
intent of a single hardcoded administrator account.

**Satisfies:** SRS-009.

---

## 9. `ui_screens` Module

**Files:** `include/ui_screens.h`, `src/ui_screens.c`

This module owns **all** `scanf`/`printf` calls in the system. It has two
categories of functions:

### 9.1 Display Functions (no return value, pure output)

```c
void ui_show_start_screen(const SystemTime *time);
void ui_show_end_screen(const SystemTime *time);
void ui_show_bank_menu(const SystemTime *time);
void ui_show_atm_menu(const SystemTime *time);
void ui_show_admin_menu(const SystemTime *time);
/* ... one per screen identified in the original bankEnterScreen(),
   screenBank(), screenATMone/two(), screenAdminOne/Two(), etc. */
```

### 9.2 Collection Functions (fill an Input struct, return success/failure)

```c
bool ui_collect_menu_choice(int *out_choice);
bool ui_collect_bill_request(BillRequestInput *out_input);
bool ui_collect_cash_request(CashRequestInput *out_input);
bool ui_collect_new_account_request(NewAccountInput *out_input);
bool ui_collect_login_credentials(char *out_username, size_t username_cap,
                                   char *out_password, size_t password_cap);
bool ui_collect_atm_login(long *out_account_number, int *out_pin);
bool ui_confirm_yes_no(const char *prompt_message);
```

Every `ui_collect_*` function returns `false` if the user's raw input fails
basic validation (e.g., non-numeric where a number was expected) — this is
where SRS-027 (reject invalid numeric input rather than proceeding with an
indeterminate value) is enforced.

**Satisfies:** No direct SRS (presentation-layer support for all functional
SRS); enforces SRS-027 at the point of input collection.

---

## 10. `main` Module

**Files:** `src/main.c`

### 10.1 Responsibility

Replaces every `goto`-based menu loop from the original with structured
`do { ... } while (...)` loops. Owns the top-level orchestration: calls a
`ui_collect_*` function, passes the result to the relevant module's logic
function, then calls a `ui_show_*`/display function with the outcome.

### 10.2 Example Flow (Bill Payment Submission)

```c
BillRequestInput input;
if (ui_collect_bill_request(&input)) {
    StatusCode result = bill_queue_enqueue(&bill_queue, &input);
    ui_show_bill_enqueue_result(result);
}
```

No business logic (validation, storage) lives in `main.c` — it purely
sequences calls between `ui_screens` and the business-logic modules.

**Satisfies:** No direct SRS (orchestration/integration layer).

---

## 11. Traceability Summary

| Module | Struct(s) | Key Functions | SRS IDs |
|---|---|---|---|
| `common` | `Money`, `StatusCode` | (types only) | supports SRS-026 |
| `platform` | `SystemTime` | `platform_clear_screen`, `platform_wait_for_keypress`, `platform_get_current_time` | SRS-025 |
| `address` | `Address` | `address_display`, `address_get_*` | supports SRS-001 |
| `bill_queue` | `BillRequestInput`, `BillNode`, `BillQueue` | `bill_queue_enqueue/dequeue/peek_front` | SRS-018–022 |
| `cash_queue` | `CashRequestInput`, `CashNode`, `CashQueue` | `cash_queue_enqueue/dequeue/peek_front` | SRS-015–017, SRS-029, SRS-030 |
| `account` | `NewAccountInput`, `AccountNode`, `AccountDatabase` | `account_db_insert_pending/approve/decline/delete`, `account_db_apply_withdrawal/deposit`, `account_db_change_pin` | SRS-001–014, SRS-031, SRS-032 |
| `admin` | — | `admin_check_credentials` | SRS-009 |
| `ui_screens` | (Input structs from other modules) | `ui_collect_*`, `ui_show_*` | enforces SRS-027 |
| `main` | — | orchestration only | — |

---

## 12. Open Items for Coding Phase

- Exact numeric bounds for `debit_card_pin` validation (4-digit range:
  `0`–`9999` inclusive, per SRS-023).
- Confirm whether `account_db_generate_account_number()` should account for
  declined/deleted accounts when computing the next sequential number
  (currently: counts only `ACCOUNT_STATUS_COMPLETED` accounts, matching
  original behavior).
- Finalize exact wording of all `ui_show_*` menu text (cosmetic, does not
  block coding start).
