# TODO

## Unambiguous sexpr-like syntax (for linearize / a future text format)

### The problem

prio's data model has three different compound shapes:

| Shape | Meaning |
|-------|---------|
| **Object** | A *named* node with a property mapping (`ReaderObject`) |
| **Mapping** | A bag of key → value pairs (`ReaderMapping`) |
| **Collection** | An ordered list of objects (`ReaderCollection`) |

Classic sexpr uses `(…)` for almost everything:

```sexpr
(server
  (host "example.com")
  (port 8080)
  (tls
    (enabled #t)
    (certificate "cert.pem"))
  (items
    (wall (texture "brick.png"))
    (floor (texture "wood.png"))))
```

Here `(tls …)` and `(items …)` look the same, but one is a nested
mapping of properties and the other is a collection of named objects.
Whether `(wall (texture …))` is “a mapping key `wall`” or “an object
named `wall`” is not visible in the parentheses alone — the reader
decides by *how* the value is requested (`read(key, mapping)` vs
`read(key, collection)` vs `read(key, object)`).

That is fine for the C++ API. It is not fine for a greppable
path = value dump, or for a text format that should be readable without
the API's intent.

**Constraint:** avoid lots of visual noise. Most documents are mostly
mappings of scalars; any scheme that marks *every* key or *every* map
taxes the common case for the sake of rare shapes.

**Requirement for a “full” solution:** unambiguous from the syntax
alone, even if that means **not** being plain sexpr anymore.

---

### Is “only mark collections” enough?

Idea: leave property trees as today’s sexpr; use `[]` only for
collections.

```sexpr
(server
  (host "example.com")
  (tls
    (enabled #t))
  (items
    [ (wall (texture "brick.png"))
      (floor (texture "wood.png")) ]))
```

| Case | Solved? |
|------|---------|
| Collection vs nested mapping | **Yes** — `[]` vs `(key …)` |
| Empty collection | **Yes** — `[]` |
| Common scalar properties | **Yes** — no extra noise |
| Nested mapping vs **single object under a key** | **No** |
| Object name vs mapping key (lexically) | **No** — both are symbols |
| Empty mapping | Convention only (omit key / empty form) |

Example that stays ambiguous without further marks:

```sexpr
(submap (nested (a 1) (b 2)))           ; mapping with one nested key?
(object (realthing (prop1 5) (prop2 7))) ; one named object?
```

Same shape: one child list headed by a symbol. So **marking only
collections is a strong, low-noise improvement, not a complete
solution.**

Linearize can still use `[N]` when the source used a collection:

```text
server.host = example.com
server.tls.enabled = true
server.items[0].wall.texture = brick.png
```

`[0]` is honest; `wall` vs mapping key remains soft unless paths gain
another marker.

---

### Brainstorm: low-noise levers (and their cost)

#### 1. `[]` only for collections (minimum change)

- **Noise:** only on collections.
- **Pros:** small diff from today’s sexpr; empty collection is clear.
- **Cons:** object-under-key vs nested mapping still ambiguous.
- **Verdict:** best first step if we optimize for little noise.

#### 2. Keywords for mapping keys (`:host`)

```sexpr
(server
  (:host "example.com")
  (:tls
    (:enabled #t))
  (:items
    [ (wall (:texture "brick.png")) ]))
```

- **Rule of thumb:** keyword head → mapping entry; symbol head → object name.
- **Pros:** key vs object name is explicit; familiar from Scheme/Clojure.
- **Cons:** **every** property pays a `:`; high noise on the common case.
- **Empty collection vs empty mapping:** still needs `[]` or a convention.
- **Verdict:** powerful but noisy if applied globally. Optional hybrid:
  keywords only where needed is inconsistent and hard to teach.

#### 3. Full three-delimiter split: `()` / `{}` / `[]`

```sexpr
(server {
  host "example.com"
  tls { enabled #t }
  items [ (wall { texture "brick.png" }) ]
})
```

- **Pros:** fully unambiguous; linearize can mirror
  `(server){host}`, `(server){items}[0](wall){texture}`.
- **Cons:** braces on every mapping level — noticeable noise.
- **Verdict:** right answer if we accept a new dialect and drop plain
  sexpr compatibility.

#### 4. Scheme dotted pairs `( key . value )`

```sexpr
(server
  (host . "example.com")
  (tls . ((enabled . #t)
          (certificate . "cert.pem")))
  (items . [ (wall . ((texture . "brick.png"))) ]))
```

- **Pros:** already in Scheme; pairs are not “just another list”; a
  true alist is a list of pairs, which is a known pattern.
- **Cons:**
  - Very noisy: a `.` on every association.
  - Nested alists become hard to read quickly.
  - Collections still need a separate rule (`[]` or list of objects).
  - Improper lists interact poorly with “rest is property list” habits.
- **Verdict:** theoretically nice, practically too much punctuation for
  config-like files.

#### 5. Quoting (`'host` or `' (wall …)`)

- **Pros:** Scheme already has quote; could mark “this is data, not a
  call” or “this is a key.”
- **Cons:** quote means something else to Lisp readers; easy to confuse
  with strings; does not by itself separate mapping vs collection vs
  object without a second convention.
- **Verdict:** poor fit as the primary disambiguator; at best a minor
  tool for awkward symbols.

#### 6. String vs symbol abuse

```sexpr
(server
  ("host" "example.com")      ; string key → mapping entry?
  (wall ("texture" "brick"))  ; symbol head → object?
)
```

or the inverse (symbols for keys, strings for object type names).

- **Pros:** no new delimiters; only a lexical category shift.
- **Cons:**
  - **Abusive:** strings stop meaning “text values” only; keys that look
    like words become quoted noise (`"host"` everywhere).
  - Object names as strings (`"wall"`) fight the usual “type is a symbol”
    intuition.
  - Values that are strings are already `"…"`; overloading strings for
    structure is confusing when reading.
  - Homogeneous string arrays vs list of keys become murkier.
- **Verdict:** low *new* syntax noise, high *semantic* noise. Prefer not
  to overload strings.

#### 7. Hybrid: quiet default + rare marks

Goal: pay noise only where shapes compete.

| Construct | Syntax | Noise |
|-----------|--------|--------|
| Object | `(name …)` as now | none |
| Scalar / nested mapping properties | `(key value)` as now | none |
| Collection | `[ (obj …) … ]` | only here |
| Object under a key (rare) | keep `(key (Name …))` with a documented rule, **or** a rare marker e.g. `(key #(Name …))` / `(key @Name …)` only in that position | rare |

Optional refinement for the remaining object-vs-mapping hole without
taxing every key:

- **Arity / structure heuristic (no new syntax):**  
  - one symbol-headed child whose *rest* is property-like → object  
  - several `(key atom)` children → mapping  
  - fragile when a mapping has a single nested mapping value
- **Tagged object only under keys:**  
  `(object # (realthing (prop1 5)))` or `(object (realthing . props))`  
  still rare if collections already use `[]`.

#### 8. Linearize-only dialects (dump, not document format)

If the goal is grep, not a new file format:

- Paths with `[N]` for collections (from `[]` in source or from the API).
- Avoid global `@` on every object name if possible; prefer structure that
  was already explicit in the file.
- A linearize-only grammar can be stricter than the writer (e.g. always
  emit `(name)` / `{key}` / `[N]`) without forcing authors to type that
  in source files — but then dump and source dialect diverge.

---

### Comparison (noise vs unambiguity)

| Approach | Noise | Collection vs map | Object vs map under key | Notes |
|----------|-------|-------------------|-------------------------|--------|
| Status quo sexpr | none | no | no | API decides |
| `[]` for collections only | low | yes | no | Best low-noise step |
| Keywords on all keys | high | needs `[]` too | yes (with rules) | Noisy common case |
| `()` / `{}` / `[]` | medium–high | yes | yes | Clean model, new dialect |
| Dotted pairs | high | needs extra rule | partial | Scheme-native, heavy |
| Quote | medium | no | weak | Wrong tool |
| String-as-key / string-as-name | medium | weak | forced | Overloads strings |
| Hybrid quiet + `[]` + rare object tag | low | yes | yes if tag used | Preferable compromise |

---

### Why not just JSON or YAML?

Schema-free tooling (linearize, sexpr ↔ JSON conversion, reformatting,
validation, diffing) is the goal, for usability as much as for
conceptual elegance. JSON and YAML do not solve it for free:

- **Typed objects.** prio's data model has records with a type
  (`(wall (texture "brick.png"))`). JSON has no such concept; prio fakes
  it as `{"wall": {...}}` (`JsonWriterImpl::begin_object`), which is
  indistinguishable from a one-key nested mapping — JSON has the
  object-vs-mapping ambiguity too. YAML has tags (`!wall {...}`), but
  they are rarely used and unevenly supported.
- **Comments and no comma rules.** JSON has neither comments nor
  trailing commas (`test/data/simple.json` has a trailing comma that
  strict JSON rejects).
- **Small grammar.** YAML's spec is huge, has implicit-typing traps
  (`no` → `false`), significant indentation, and divergent
  implementations. A sexpr reader is a few hundred lines.
- **Existing data.** Twenty years of files in this format family.

So the goal is not to replace sexpr but to make it self-describing,
while staying within standard Scheme datum syntax (R7RS). Breaking
Scheme/Lisp syntax is only justified if the result is substantially
more elegant — which rules out `[]` (in R6RS/Racket it is just `()`),
`{}`, `key:` and similar.

---

### Lens: floating vs grounded nesting (from array theory)

Array languages distinguish:

- **Grounded** (SHARP APL, J): every box adds a level; `<5` ≠ `5`.
- **Floating** (APL2, Nial): some wrappings collapse; `⊂5` ≡ `5`,
  Nial's `single 5 = 5`.

Classic prio sexpr floats *everywhere*: a key's value is spliced into
the key's list, so nesting levels collapse, and `(k (foo (a 1)))` may be

- the open contents: a mapping with entry `foo`,
- one box: an object of type `foo`,
- a list of one box: a collection with one object.

JSON is fully grounded (`{}`/`[]` per level), which is why it is
unambiguous and also why it is noisy. The design question is therefore:
**which levels may float, and which must be grounded?**

A level may float when nothing is lost by collapsing it, or when the
next token still identifies what it was.

---

### Lesson from APL/K: scalar vs one-element array

APL distinguishes scalar (rank 0) from one-element vector (rank 1), but
notation keeps it out of the way:

| | APL | K/q display |
|---|---|---|
| scalar | `5` | `5` |
| vector, n ≥ 2 | `1 2 3` (strand notation) | `1 2 3` |
| one-element vector | `,5` (ravel) | `,5` |
| empty vector | `⍬` | `!0`, typed `0#0` |

Only the degenerate lengths get a marker. prio *already* uses strand
notation: `(intvalues 1 2 3 4)`. The ambiguity only exists for 0 and 1
elements.

Two ways to apply this:

1. **Mark the degenerate cases** (APL/K): `(intvalues #(5))`,
   `(intvalues #())`; everything else unchanged. Canonical, but the
   spelling depends on the length.
2. **Define the distinction away** (floating array theory, APL scalar
   extension, RDF, JSON-LD — whose spec says a single value and a
   one-element array are equivalent, and whose compaction collapses
   `[5]` to `5`): *every property holds a list of atoms*.
   `read(key, int)` means "exactly one element", `read(key,
   vector<int>)` accepts any length. `(n 5)` is unambiguous by
   definition, the syntax stays exactly as today, and the JSON writer
   collapses length-1 lists. Cost: JSON `[5]` round-trips to `5`.

Option 2 is preferred: nothing is lost, the concept of a "one-element
array" simply stops existing.

Typed empties (q's `0#0` vs `""`) are where schema-free inference
fails; probably overkill here — an untyped empty is fine, as `[]` is in
JSON.

---

### Proposal: Scheme-compatible, self-describing sexpr

Combine the above into one grammar that is valid R7RS datum syntax:

```
value  := atom                   ; scalar
        | #( value* )            ; sequence of values (collection)
        | ( [type] entry* )      ; object if type present, else mapping
entry  := ( key atom* )          ; property: zero or more atoms (strand)
        | ( key #( value* ) )    ; collection
        | ( key [type] entry* )  ; object or mapping, spliced into the entry
```

Core rule: **the head of an entry is a key; a bare symbol in value
position is a type.** This costs nothing today: string values must
already be quoted (`GET_VALUE_MACRO("string", is_string, ...)` rejects
symbols), so a bare symbol has never been a valid scalar.

Which levels float and which are grounded:

| Level | Treatment | Why it is safe |
|---|---|---|
| atom strands | float | scalar ≡ one-element list by definition |
| mappings, objects | float (spliced) | first token after the key distinguishes them |
| collections | grounded with `#()` | the only sequence that holds boxes |

Example:

```scheme
(server
  (host "example.com")
  (port 8080)
  (tags "web" "prod")
  (tls
    (enabled #t)
    (certificate "cert.pem"))
  (logger file-logger            ; object of type file-logger under "logger"
    (path "/var/log/x"))
  (items #((wall (texture "brick.png"))
           (floor (texture "wood.png")))))
```

Compared to classic sexpr: scalars, arrays, nested mappings and the
document root are **unchanged**; collections gain `#( )`; an object
under a key gets *simpler* (`(logger (file-logger ...))` →
`(logger file-logger ...)`).

Every former ambiguity is decided by the next token alone:

| Source | Meaning |
|---|---|
| `(n 5)` / `(n 1 2 3)` | property with one / three values |
| `(k (a 1) (b 2))` / `(k #((a) (b)))` | mapping / collection |
| `(k (foo (a 1)))` / `(k foo (a 1))` | nested mapping / object of type `foo` |
| `(k foo)` | empty object of type `foo` |
| `(k #())` | empty collection |
| `((a 1))` inside `#()` | anonymous mapping as a collection element |

#### Walkthrough: how `(tls (enabled #t) (certificate "cert.pem"))` is read

1. It is a list inside a mapping, so its head `tls` is a **key**.
2. The next token is a list, not an atom, `#(` or a bare symbol: no
   type, so the value is a **mapping**.
3. Each remaining element is an entry, read the same way:
   `(enabled #t)` → key `enabled`, one atom → `true`.

Linearized: `server.tls.enabled = #t`.

#### The model: an entry is the key consed onto the value's list form

| Value | List form `V` | Entry `(key . V)` |
|---|---|---|
| mapping | `((enabled #t) (certificate "cert.pem"))` | `(tls (enabled #t) (certificate "cert.pem"))` |
| object | `(file-logger (path "x"))` | `(logger file-logger (path "x"))` |
| atoms | `(#t)` / `("web" "prod")` | `(enabled #t)` / `(tags "web" "prod")` |
| collection | `(#((wall) (floor)))` | `(items #((wall) (floor)))` |

A mapping is a list of entries (an alist); an object is a type followed
by entries, exactly like today's document root. `(cdr entry)` is always
the value's list form, and its first element says what kind of value it
is — at every level, root included.

With multi-valued properties the classic wart (scalars wrapped as
`(#t)` while mappings are spliced) mostly dissolves: a property's list
form *is* its list of atoms. A purist alist would write `(enabled . #t)`,
but dots on every scalar line are the noise rejected in lever 4.

#### Linearize

- `.key` — mapping step
- `[n]` — collection index
- `(type)` — object tag

```text
server.tls.enabled = #t
server.tags = "web" "prod"
server.logger(file-logger).path = "/var/log/x"
server.items[0](wall).texture = "brick.png"
```

#### JSON mapping

- Property with one atom → scalar; with n ≠ 1 atoms → array.
- Mapping → `{}`; collection → `[]`.
- Object → needs one convention, e.g. `{"@type": "wall", ...}`, which
  (unlike today's `{"wall": {...}}`) is lossless.

#### Open points

- **`(k)`:** a property with zero values, or an empty mapping? Cleanest
  is to declare them the same ("nothing here"); the JSON writer must
  then pick one of `[]` / `{}`.
- **Keys that are not plain symbols:** `|odd key|` (R7RS symbol
  syntax).
- **Duplicate keys** can be rejected at parse time instead of in
  `get_subsection` (cf. `FIXME_WRONG_PLACE_TO_VALIDATE`).
- **Mixed collections** (`#(1 (wall))`) are syntactically fine;
  whether to allow them is a validation choice.
- **Migration:** classic files cannot be converted without a schema,
  but every prio application can: load with the classic reader, save
  with the new writer.

---

### Variant: repeated keys instead of `#()` (one rule for everything)

Stolen from protobuf text format (`tag: "a" tag: "b"`), HCL (repeated
blocks form a list) and TOML (`[[items]]`): there is no collection
syntax at all — a collection is the same key appearing several times.

Combined with the floating rule (scalar ≡ one-element list), the whole
data model collapses into one rule:

> **Every key holds a list of values. A value is an atom, an object, or
> a mapping.**

```
value  := atom
        | ( type atom* )         ; typed tuple (see below)
        | ( [type] entry* )      ; object if type present, else mapping
entry  := ( key atom* )          ; atoms, written as a strand
        | ( key type atom* )     ; typed tuple
        | ( key [type] entry* )  ; object or mapping
```

A key's values are all its entries in document order. The grammar is
pure lists and atoms — no `#()`, nothing beyond R7RS lists.

```scheme
(level
  (name "Forest")
  (tags "outdoor" "day")
  (spawn-point vec2 10 20)
  (object wall  (texture "brick.png"))
  (object floor (texture "wood.png"))
  (object door  (target "cave") (pos vec2 3 4)))
```

Reading:

| Call | Meaning |
|---|---|
| `read(key, int)` / `read(key, object)` | exactly one value |
| `read(key, vector<int>)` / `read(key, collection)` | all values, any count |
| missing key | empty list (empty collection, no values) |

Properties:

- **Consistent floating:** one object ≡ a collection of one, exactly as
  a scalar ≡ a one-element list. Object-under-key vs single-element
  collection stops being a distinction.
- **Every collection element is a self-contained line:** good for grep,
  diff and linearize (`level.object[1](floor).texture = "wood.png"`).
  Matches how SuperTux-style level files tend to look anyway.
- **Atoms strand, compound values repeat the key.** `(k 1 2)` and
  `(k 1) (k 2)` mean the same; the canonical writer emits the strand for
  atoms and one entry per compound value. Justified (atoms are not
  lists), but it is two spellings.

Costs:

- **Mappings become multimaps.** A duplicate key is no longer a parse
  error, so a typo'd or accidentally repeated key cannot be caught by
  the reader. That check moves to (optional) schema validation.
- **Order across different keys** is not meaningful; only order within
  one key is. A collection whose elements are interleaved with other
  keys is legal; the canonical writer groups them.
- **Legacy collections change shape:** `(objects (wall …) (floor …))`
  becomes `(object wall …) (object floor …)` — one extra symbol per
  element compared to `#()`, in exchange for line-local elements.

#### Typed tuples (from KDL / Rebol)

`( type atom* )` gives custom types (`write_custom`) a natural form,
using a shape the grammar did not use yet:

```scheme
(pos vec2 10 20)
(color rgba 1 0 0 1)
(timeout seconds 5)
```

`(k foo)` is both "empty object of type `foo`" and "empty tuple of type
`foo`" — the same thing, so no ambiguity.

#### JSON mapping for the variant

- Key with exactly one value → that value; n ≠ 1 values → array
  (JSON-LD-style compaction).
- Mapping → `{}`; object → `{"@type": "wall", ...}`.
- Typed tuple → `{"@type": "vec2", "@args": [10, 20]}` or similar.
- Missing key ↔ absent; the `(k)` open point becomes "a key present
  with zero values", which can simply be normalised away.

---

### Alternative: read the document as executable Scheme code

Different angle: pretend the file is a Scheme program. Every list is a
call, every head a procedure, and the **schema is the environment**
(the set of definitions). Classic sexpr needs a schema for the same
reason code needs bindings. "Schema-free" then means: find **one fixed,
generic environment** that can evaluate every document.

Real Lisp config languages already show what such code looks like,
e.g. Guix records:

```scheme
(package
  (name "hello")
  (version "2.10")
  (source (origin
            (method url-fetch)
            (uri "mirror://gnu/hello/hello-2.10.tar.gz")))
  (inputs (list gawk)))
```

`(package …)` is a constructor call, `(name "hello")` a field clause,
and a field's value is an expression: an atom or another constructor
call. Constructor and field alternate at every level, and that
alternation is all a generic environment needs:

> **Inside a call, a list is a clause. Inside a clause, a list is a
> call.**

The root is a call (the document type), so whether a list's head is a
type or a key follows from depth alone.

```
call   := ( type atom* clause* )   ; object: positional atoms, then fields
clause := ( key value* )           ; field: zero or more values
value  := atom | symbol | call
```

The complete schema-free reader, in Scheme:

```scheme
(define (read-call form)              ; (type arg ...)
  (make-object (car form)
               (filter (lambda (x) (not (pair? x))) (cdr form))  ; positional
               (map read-clause (filter pair? (cdr form)))))     ; fields

(define (read-clause form)            ; (key value ...)
  (cons (car form) (map read-value (cdr form))))

(define (read-value x)
  (if (pair? x) (read-call x) x))     ; atom, enum symbol, or object
```

Example:

```scheme
(server
  (host "example.com")
  (port 8080)
  (tags "web" "prod")
  (mode fast)                          ; bare symbol: enum constant
  (tls (tls-config
         (enabled #t)
         (certificate "cert.pem")))
  (logger (file-logger (path "/var/log/x")))
  (spawn (vec2 10 20))                 ; positional args: typed tuple
  (items (wall  (texture "brick.png"))
         (floor (texture "wood.png"))))
```

What falls out:

- **Collections stay as in classic sexpr:** `(items (wall …) (floor …))`
  is a field with two values, each a call.
- **Object under a key stays as in classic sexpr:**
  `(logger (file-logger …))`.
- **Scalars and arrays stay as in classic sexpr:** a field holds a list
  of values. That is the floating rule again, here as plain
  function-call semantics (arguments are a list).
- **Typed tuples for free:** `(vec2 10 20)` is a call with positional
  args.
- **Enums are bare symbols**, i.e. constants, like `gawk` above.
- **The one change: anonymous mappings don't exist.** A nested mapping
  must be a call, so it needs a type: `(tls (tls-config …))` instead of
  `(tls (enabled #t))`. Read under the new rule, classic
  `(tls (enabled #t))` is an object of type `enabled` with positional
  arg `#t`: valid but wrong, so this must be an opt-in dialect.

It is the mirror image of the proposal above:

| | Self-describing proposal | Executable view |
|---|---|---|
| nested mapping `(tls (enabled #t))` | unchanged | needs a type: `(tls (tls-config …))` |
| collection `(items (wall) (floor))` | changes: `#()` or repeated keys | unchanged |
| object under key | changes: `(k foo …)` | unchanged: `(k (foo …))` |
| anonymous mappings | exist | don't exist; every mapping is typed |

Which one breaks fewer existing files depends on whether the data has
more anonymous nested mappings or more collections/objects under keys.
For game data the latter is likely. Many nested mappings arguably
should have had a type anyway: `(position (x 1) (y 2))` becomes
`(position (vec2 1 2))`. The resulting model is the one XML, KDL and
Guix records share: typed nodes with positional args and named fields.

Running it:

- **Schema-free:** the generic reader above, or equivalently an
  environment where every unbound head means "make a generic
  object/field".
- **With a schema:** bind each type to a real constructor (e.g. a
  `define-prio-type` macro over `define-record-type` listing the allowed
  fields). Evaluation then *is* validation: unknown fields, wrong arity
  and type errors surface as ordinary evaluation errors. A schema is
  just a library.
- **Escape hatch:** `'(…)` quotes raw, uninterpreted data, which is
  what quote means in Lisp, not quote-as-structure (lever 5).
- **Temptation to resist:** `let`, `include` and arithmetic are one
  step away, and down that road lie Nix, Jsonnet and Guix (Turing-
  complete config). Keep the generic environment to constructors only;
  anything more is an explicit opt-in extension.

Why it's appealing: the rule is not a convention we invented; it's
"what the code would mean". The generic reader is small enough to put
in the spec. The price is that every mapping needs a type, which
arguably helps, since types document intent and give schemas something
to attach to.

---

### Other prior art worth stealing

- **SXML `(tag (@ (attr "v")) child ...)`:** a reserved head symbol
  separates attributes (mapping) from children (collection). XML itself
  has exactly prio's split. Scheme-native precedent for marking a
  structural level with a reserved symbol, should one ever be needed
  (e.g. `(items (* (wall …) (floor …)))` as a fallback to `#()`).
- **KDL / SDLang:** nodes with positional args, `key=value` properties
  and children; `(type)` prefix annotations. Source of typed tuples;
  confirms `(type)` as the linearize segment for object tags.
- **EDN tagged literals (`#app/Wall {...}`), Racket prefab structs
  (`#s(wall ...)`), Preserves (`<wall "x">`):** records as first-class
  values. Preserves is the closest existing design to prio's model;
  worth reading for its canonical form, equality, and a binary encoding
  of the same data model. Its syntax is not Scheme, so do not take that.
- **Dotted path keys (TOML, Nix):** `tls.enabled = true` in source, not
  just in dumps. Accepting `(server.tls.enabled #t)` as input would make
  linearize output valid input and suit override files
  (`override_reader_mapping`). Cost: `.` becomes reserved in keys.
- **Typed empties (GVariant `@as []`, q `0#0`):** an optional annotation
  if empty values ever need a type.
- **References (R7RS datum labels `#0=` / `#0#`, STON `@1`):** standard
  Scheme syntax for sharing a subtree; free to add later if needed.
- **Schema as data (CUE):** stay schema-free for reading, but allow an
  optional schema in the same syntax for validation — the natural home
  for duplicate-key and typo checks once keys are multi-valued.

Anti-patterns:

- **Lua tables:** one structure for array and map, hence the "is `{}`
  an array or a map?" bug when encoding to JSON — the same mistake as
  classic sexpr.
- **YAML implicit typing:** keep scalars lexically explicit (quoted
  strings, `#t`/`#f`), as prio already does.

---

### Alternatives considered for the proposal

- **`key:` / `[]` plist dialect** (`(server host: "x" items: [...])`):
  fewer parens, but breaks Scheme syntax for no substantial gain; if
  anything, `:keywords` and `#()` would be the Lisp way.
- **Always `#()` for atom arrays** (`(intvalues #(1 2 3))`): unambiguous
  but taxes every array; multi-valued properties make it unnecessary.
- **APL-style markers only for 0/1-element arrays:** canonical, but the
  spelling depends on length; defining scalar ≡ singleton is simpler.

---

### Suggested direction

1. Adopt the **Scheme-compatible, self-describing sexpr** proposal
   above: multi-valued atom properties, type symbol after a key for
   objects. Decide between `#()` for collections and the
   **repeated-keys variant** (every key holds a list of values; no
   `#()`, plus typed tuples). The variant is the more uniform model;
   its main cost is losing duplicate-key detection in the reader.
   Alternatively, the **executable-code view** keeps collections and
   objects-under-key as they are today and instead requires every
   nested mapping to be typed (call/clause alternation by depth).
2. Keep classic sexpr as default input; new dialect opt-in
   (`Format::…` / flag) until migration tooling exists.
3. Align `priotool --linearize` with the proposal's path segments.
4. Superseded: `[]`-only collections (non-portable, leaves the
   object-under-key gap), keywords everywhere, dotted pairs everywhere,
   string/symbol abuse, quote-as-structure.

---

### Possible next steps

- [ ] Decide `#()` collections vs repeated keys vs the executable view
      (typed mappings, call/clause alternation).
- [ ] Survey existing data files: count anonymous nested mappings vs
      collections/objects under keys to see which dialect breaks less.
- [ ] Settle the open points (`(k)` semantics, JSON object convention).
- [ ] Decide on typed tuples `(type atom*)` for custom types.
- [ ] Make `read(key, int)` / `read(key, vector<int>)` treat scalar and
      one-element list as the same (multi-valued properties).
- [ ] Prototype the new dialect in the sexpr reader/writer (opt-in).
- [ ] Switch the JSON object encoding to a lossless convention.
- [ ] Align `priotool --linearize` path segments with the proposal.
- [ ] Load-and-save migration path for classic files.
