#
# This is simple(?) convertor for "Squirrel server pages" to Squirrel code.
# Supported syntax:
# <? code ?> - inline Squirrel code
# <?= value ?> - output Squirrel value (equivalent to <? print(value) ?>)
# The page received parameters from caller in dictionary named "d".
# Simple example:
#
# Hello <?= d.name ?>!
# <? for (i = 0; i < 5; i++) { ?>
# i = <?= i ?>
# <? } ?>
#

const TEXT = 1
const CODE = 2
const PRINT = 3
# Character to use after < and before > for inline code
const MARKER = "?"

function start_page() {
    print("function render(d) {\n");
}
function stop_page() {
    print("}\n");
    print("render({\"name\": \"test\"})\n");
}
function start_text() {
    print("print(@\"");
}
function stop_text() {
    print("\")\n");
}
function start_print() {
    print("print(");
}
function stop_print() {
    print(")\n");
}

function process(f) {
    local c;
    local state = TEXT

    start_page()
    start_text()
    while (c = f.read(1)) {
        if (c == "<") {
            c2 = f.read(1)
            if (c2 == MARKER) {
                stop_text()
                state = CODE
                c = f.read(1)
                if (c == "=") {
                    state = PRINT
                    start_print()
                    continue
                }
                print(c)
                continue
            }
            print(c)
            c = c2
        } else if (c == MARKER) {
            c2 = f.read(1)
            if (c2 == ">") {
                local old_state = state;
                if (old_state == PRINT)
                    stop_print()
                start_text()
                state = TEXT
                // Eat newline after closing code bracket, but not print bracket
                if (old_state == PRINT)
                    continue
                c = f.read(1)
                if (c == "\n")
                    continue
            }
            print(c)
            c = c2
        }
        print(c);
    }
    if (state == TEXT)
        stop_text()
    stop_page()
}

f = file(vargv[1], "r")
process(f)
f.close()
