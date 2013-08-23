fin = file(vargv[1], "r")
fout = file(vargv[2], "w")

prefix = "sq"
while (l = fin.readline()) {
    if (l.find("/*stdlib*/"))
        prefix = "sqstd"

    if (!l.find("(*"))
        continue

    toks = split(l, "()")
    sym = toks[1].slice(1)
    fout.write("#define " + prefix + "_" + sym + " (sqapi->" + sym + ")\n")
}
