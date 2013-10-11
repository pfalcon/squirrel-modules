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
    local modenv = {};
    _import_(module, modenv)
    getroottable()[as ? as : module] = modenv;
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
__modules = []
function import_from(module, what, as = null) {
    if (what == "*") {
        _import_(module)
    } else {
        local save = getroottable();
        local modenv = {};
        setroottable(modenv);
        _import_(module, modenv);
        setroottable(save);
        __modules.append(modenv);
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
