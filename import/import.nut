/*
  Import module into a separate namespace.

  Returns this namespace.
*/
__modules = {}
function require(module) {
    if (module in __modules)
        return __modules[module];

    local save = getroottable()
    // Let loaded module reuse already loaded modules
    __pristine_env.__modules = __modules
    local modenv = clone __pristine_env;
    setroottable(modenv)
    _import_(module)
    setroottable(save)
    __modules[module] = modenv;
    return modenv;
}

/*
  Import module into a separate namespace.

  Examples:

  import("foo")

    Imports module "foo" into namespace "foo".

  import("foo", "bar")

    Imports module "foo" into namespace "bar" (i.e. all symbols
    of module "foo" will be available in namespace "bar", this
    helps to avoid module name clashes or to use shorter names).
*/

function import(module, as = null) {
    getroottable()[as ? as : module] = require(module);
}


/*
  Import some/all module symbold into a current namespace.

  Examples:

  import_from("foo", "*")

    Imports all symbols from module "foo" into current namespace.

  import_from("foo", "bar")

    Imports "bar" from module "foo" into current namespace.

  import_from("foo", ["bar", "baz"])

    Imports "bar", "baz" from module "foo" into current namespace.

  import_from("foo", "bar", "bar2")

    Imports "bar" from module "foo" into current namespace as "bar2".
    This works with single symbol only.
*/
function import_from(module, what, as = null) {
    if (what == "*") {
        // Shortcut implementation for all symbols -
        // this doesn't run module with separate roottable,
        // to avoid copying all (including standard symbols)
        // Thus, it exposes current running environment to the module
        _import_(module)
    } else {
        local modenv = require(module);
        if (as) {
            getroottable()[as] = modenv[what];
            return;
        }

        if (typeof(what) != "array")
            what = [what]
        foreach (n in what)
            getroottable()[n] = modenv[n];
    }
}


// Capture pristine environment with just import functions added
__pristine_env = clone getroottable();
